#pragma once

#include <ISession.h>

struct MYSQL;

namespace repo
{
namespace mysql
{

class Session : public repo::ISession
{
public:
	~Session();

	static repo::mysql::Session Get(
		const char* host,
		const char* user, const char* passwd,
		const char* db, unsigned int port,
		const std::string& contextId = ""
	);

	virtual void ExecSql(const std::string& query, const RowCallback& cb) override;
	virtual void ExecSql(const std::string& query) override;

	virtual const std::string& GetContextId() const override;

	virtual std::string BeginTransaction() override;
	virtual std::string CommitTransaction() override;
	virtual std::string RollbackTransaction() override;
	virtual std::string TextKeyType() override;

private:
	Session(MYSQL* connection, const std::string& dbName, const std::string& contextId);
	MYSQL* m_connection;
	const std::string m_contextId;
};

}
}

