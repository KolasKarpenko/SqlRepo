#pragma once

#include <functional>

#include "IRow.h"
#include "ISqlDialect.h"

namespace repo
{

typedef std::function<void (const IRow& row)> RowCallback;

class ISession
{
public:
	virtual ~ISession() {}

	virtual void ExecSql(const std::string& query, const RowCallback& cb) = 0;
	virtual void ExecSql(const std::string& query) = 0;
	virtual const std::string& GetContextId() const = 0;

	std::string BeginTransaction() const
	{
		return m_dialect->BeginTransaction();
	}

	std::string CommitTransaction() const
	{
		return m_dialect->CommitTransaction();
	}

	std::string RollbackTransaction() const
	{
		return m_dialect->RollbackTransaction();
	}

	ISqlDialectPtr SqlDialect() const
	{
		return m_dialect;
	}
	
protected:
	ISession(const ISqlDialect* dialect) :m_dialect(dialect) {}

	ISqlDialectPtr m_dialect;
};

}

