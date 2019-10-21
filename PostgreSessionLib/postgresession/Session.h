#pragma once

#include <ISession.h>

struct pg_conn;

namespace repo
{
namespace postgresql
{

class Session : public repo::ISession
{
public:
	~Session();

	static repo::postgresql::Session Get(const std::string& connectionString, const std::string& contextId = "");

	virtual void ExecSql(const std::string& query, const RowCallback& cb) override;
	virtual void ExecSql(const std::string& query) override;

	virtual const std::string& GetContextId() const override;

private:
	Session(pg_conn* connection, const std::string& contextId);
	const std::string m_contextId;
	pg_conn* m_connection;
};

}
}

