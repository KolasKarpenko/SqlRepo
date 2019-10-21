#pragma once

#include <ISession.h>

struct sqlite3;

namespace repo
{
namespace sqlite
{

class Session : public repo::ISession
{
public:
	~Session();

	static repo::sqlite::Session Get(const std::string& dbName, const std::string& contextId = "");

	virtual void ExecSql(const std::string& query, const RowCallback& cb) override;
	virtual void ExecSql(const std::string& query) override;

	virtual const std::string& GetContextId() const override;

private:
	Session(const std::string& dbPath, const std::string& contextId);
	sqlite3* m_db;
	const std::string m_dbPath;
	const std::string m_contextId;
};

}
}

