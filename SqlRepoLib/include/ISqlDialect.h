#pragma once

#include <string>

namespace repo
{

class ISqlDialect
{
public:
	virtual ~ISqlDialect() {}

	virtual std::string BeginTransaction() const
	{
		return "begin transaction;";
	}

	virtual std::string CommitTransaction() const
	{
		return "commit transaction;";
	}

	virtual std::string RollbackTransaction() const
	{
		return "rollback transaction;";
	}

	virtual std::string TextKeyType() const
	{
		return "text";
	}
};

typedef std::shared_ptr<const ISqlDialect> ISqlDialectPtr;

}

