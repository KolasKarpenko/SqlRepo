#include "mysqlsession/Session.h"

#include <mysql.h>
#include <exception>
#include <DataSchemaDeploy.h>
#include <IRow.h>
#include "mysqlsession/ZeroSchemaPatch.h"

namespace repo
{
namespace mysql
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
					SqlTools::ToLower(azColName[i]),
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
		std::map<std::string, SqlString>::const_iterator it = m_data.find(SqlTools::ToLower(columnName));
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

}

Session::Session(MYSQL* connection, const std::string& dbName, const std::string& contextId) :
	m_connection(connection),
	m_contextId(contextId)
{
	bool exists = false;
	std::stringstream selectJournal;
	selectJournal << "select table_name from information_schema.tables where table_schema = '" << dbName << "' and table_name='private_Journal';";

	ExecSql(selectJournal.str().c_str(), [&exists](const repo::IRow& row) {
		exists = true;
	});

	if (!exists) {
		repo::mysql::ZeroSchemaPatch zeroPatch;
		DataSchemaDeploy zero;
		zero.Register(zeroPatch);
		zero.CreateSchema(*this);
	}
}

Session::~Session()
{
	mysql_close(m_connection);
}

repo::mysql::Session Session::Get(const char* host,
	const char* user, const char* passwd,
	const char* db, unsigned int port,
	const std::string& contextId
) {
	MYSQL* connection = mysql_init(NULL);

	if (!mysql_real_connect(connection, host, user, passwd, db, port, NULL, 0)) {
		mysql_close(connection);
		throw std::runtime_error(mysql_error(connection));
	}

	return repo::mysql::Session(connection, db, contextId);
}

void Session::ExecSql(const std::string& query, const RowCallback& cb)
{
	std::string oneString = query;
	std::replace(oneString.begin(), oneString.end(), '\n', ' ');
	std::replace(oneString.begin(), oneString.end(), '\t', ' ');
	if (mysql_query(m_connection, oneString.c_str())) {
		throw std::runtime_error(mysql_error(m_connection));
	}

	MYSQL_RES* result = mysql_store_result(m_connection);

	if (result == nullptr) {
		throw std::runtime_error(mysql_error(m_connection));
	}

	unsigned int num_fields = mysql_num_fields(result);
	std::vector<char*> colNames(num_fields);

	for (unsigned int i = 0; i < num_fields; ++i) {
		MYSQL_FIELD* field = mysql_fetch_field_direct(result, i);
		if (field == nullptr) {
			continue;
		}

		colNames[i] = field->name;
	}

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result))) {
		cb(repo::mysql::Row(num_fields, row, colNames.data()));
	}

	mysql_free_result(result);
}

void Session::ExecSql(const std::string& query)
{
	std::string oneString = query;
	std::replace(oneString.begin(), oneString.end(), '\n', ' ');
	std::replace(oneString.begin(), oneString.end(), '\t', ' ');
	if (mysql_query(m_connection, oneString.c_str())) {
		throw std::runtime_error(mysql_error(m_connection));
	}
}

const std::string& Session::GetContextId() const
{
	return m_contextId;
}

std::string Session::BeginTransaction()
{
	return "start transaction;";
}

std::string Session::CommitTransaction()
{
	return "commit;";
}

std::string Session::RollbackTransaction()
{
	return "rollback;";
}

std::string Session::TextKeyType()
{
	return "varchar(40)";
}

}
}
