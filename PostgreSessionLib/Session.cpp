#include "postgresession/Session.h"

#include <libpq-fe.h>
#include <exception>
#include <DataSchema.h>
#include <IRow.h>
#include "postgresession/ZeroSchemaPatch.h"

namespace repo
{
namespace postgresql
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

static void clearResult(PGresult* res)
{
	PQclear(res);
	res = nullptr;
}

}

Session::Session(PGconn* connection, const std::string& contextId) :
	m_connection(connection),
	m_contextId(contextId)
{
	bool exists = false;
	ExecSql("select exists ( select 1 from pg_tables where schemaname = 'public' and tablename = 'private_journal');",
		[&exists](const repo::IRow& r) {
			exists = r.Data("exists").Data() == "t";
		}
	);

	if (!exists) {
		repo::postgresql::ZeroSchemaPatch zeroPatch;
		DataSchema zero;
		zero.Register(zeroPatch);
		zero.CreateSchema(*this);
	}
}

Session::~Session()
{
	PQfinish(m_connection);
}

repo::postgresql::Session Session::Get(const std::string& connectionString, const std::string& contextId)
{
	PGconn* connection = PQconnectdb(connectionString.c_str());

	if (connection == nullptr) {
		PQfinish(connection);
		throw std::runtime_error(PQerrorMessage(connection));
	}

	return repo::postgresql::Session(connection, contextId);
}

void Session::ExecSql(const std::string& query, const RowCallback& cb)
{
	PGresult* res = PQexec(m_connection, query.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		clearResult(res);
		throw std::runtime_error(PQerrorMessage(m_connection));
	}

	const int ncols = PQnfields(res);
	std::vector<char*> colNames(ncols);
	for (int c = 0; c < ncols; ++c) {
		colNames[c] = PQfname(res, c);
	}

	const int nrows = PQntuples(res);
	std::vector<char*> rowData(ncols);
	for (int r = 0; r < nrows; ++r) {
		for (int c = 0; c < ncols; ++c) {
			rowData[c] = PQgetvalue(res, r, c);
		}
		repo::postgresql::Row row(ncols, rowData.data(), colNames.data());
		cb(row);
	}

	clearResult(res);
}

void Session::ExecSql(const std::string& query)
{
	PGresult* res = PQexec(m_connection, query.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		clearResult(res);
		throw std::runtime_error(PQerrorMessage(m_connection));
	}
	clearResult(res);
}

const std::string& Session::GetContextId() const
{
	return m_contextId;
}

}
}
