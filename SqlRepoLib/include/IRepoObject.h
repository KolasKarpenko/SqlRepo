#pragma once

#include "TransactionPatch.h"

namespace repo
{

class IRepoObject : public ISerializable
{
public:
	virtual ~IRepoObject() {}

	virtual void Save(repo::TransactionPatch& transaction) const = 0;
	virtual void Delete(repo::TransactionPatch& transaction) const = 0;
};

typedef std::shared_ptr<IRepoObject> IRepoObjectPtr;

}

