#pragma once

#include <functional>

#include "IRow.h"

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

	virtual std::string BeginTransaction()
	{
		return "begin transaction;";
	}

	virtual std::string CommitTransaction()
	{
		return "commit transaction;";
	}

	virtual std::string RollbackTransaction()
	{
		return "rollback transaction;";
	}

	virtual std::string TextKeyType()
	{
		return "text";
	}
};

}

