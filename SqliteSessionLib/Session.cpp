#include "sqlitesession/Session.h"

#include <sqlite3.h>
#include <exception>
#include <fstream>
#include <DataSchemaDeploy.h>
#include <IRow.h>
#include "sqlitesession/ZeroSchemaPatch.h"

namespace repo
{
namespace sqlite
{

namespace
{

class Row : public repo::IRow
{
public:
	Row(int argc, char** argv, char** azColName)
	{
		m_columns.reserve(argc);
		for (int i = 0; i < argc; ++i) {
			m_columns.push_back(azColName[i]);
			m_data.insert(
				std::make_pair(
					azColName[i],
					argv[i] ? SqlString::FromData(argv[i]) : SqlString::Null()
				)
			);
		}
	}

	const std::vector<std::string>& Columns() const
	{
		return m_columns;
	}

	virtual SqlString Data(const std::string& columnName) const override
	{
		std::map<std::string, SqlString>::const_iterator it = m_data.find(columnName);
		if (it != m_data.end()) {
			return it->second;
		}

		throw std::runtime_error("Column " + columnName + " was not found.");
	}

	virtual SqlDouble GetReal(const std::string& columnName) const override
	{
		const SqlString& data = Data(columnName);
		return data.IsNull() ? SqlDouble::Null() : SqlDouble::FromData(data.Data().c_str());
	}

	virtual SqlInteger GetInteger(const std::string& columnName) const override
	{
		const SqlString& data = Data(columnName);
		return data.IsNull() ? SqlInteger::Null() : SqlInteger::FromData(data.Data().c_str());
	}

	virtual SqlBigInt GetBigInt(const std::string& columnName) const override
	{
		const SqlString& data = Data(columnName);
		return data.IsNull() ? SqlBigInt::Null() : SqlBigInt::FromData(data.Data().c_str());
	}

private:
	std::map<std::string, SqlString> m_data;
	std::vector<std::string> m_columns;
};

int callback(void* ctx, int argc, char** argv, char** azColName)
{
	if (ctx == nullptr)
		return 0;

	RowCallback* cb = static_cast<RowCallback*>(ctx);
	(*cb)(repo::sqlite::Row(argc, argv, azColName));
	return 0;
}

void execQuery(sqlite3* db, const char* query, void* ctx = nullptr)
{
	char* zErrMsg = 0;
	int rc = sqlite3_exec(db, query, callback, ctx, &zErrMsg);

	if (rc != SQLITE_OK) {
		std::string msg = std::string("SQL error: ") + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw std::runtime_error(msg);
	}
}

}

Session::Session(const std::string& dbPath, const std::string& contextId) :
	m_db(nullptr),
	m_dbPath(dbPath),
	m_contextId(contextId)
{
	bool dbExists = false;
	{
		std::ifstream fileStream(dbPath.c_str());
		dbExists = fileStream.good();
	}

	sqlite3* db;
	int rc = sqlite3_open(dbPath.c_str(), &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		throw std::runtime_error(std::string("Can not open database: ") + dbPath);
	}

	m_db = db;

	if (!dbExists) try {
		repo::sqlite::ZeroSchemaPatch zeroPatch;
		DataSchemaDeploy::Register(zeroPatch);
		repo::DataSchemaDeploy::CreateSchema(*this);
	}
	catch (const std::exception&) {
		sqlite3_close(db);
		throw;
	}
}

Session::~Session()
{
	if (m_db != nullptr)
		sqlite3_close(m_db);
}

repo::sqlite::Session Session::Get(const std::string& dbPath, const std::string& contextId)
{
	return repo::sqlite::Session(dbPath, contextId);
}

void Session::ExecSql(const std::string& query, const RowCallback& cb)
{
	RowCallback cp = cb;
	execQuery(m_db, query.c_str(), &cp);
}

void Session::ExecSql(const std::string& query)
{
	execQuery(m_db, query.c_str());
}

const std::string& Session::GetContextId() const
{
	return m_contextId;
}

}
}
