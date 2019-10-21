#pragma once

#include <vector>
#include "Patch.h"

namespace repo
{

class TransactionPatch : public repo::Patch
{
public:
	TransactionPatch();
	TransactionPatch(int64_t schemaVersion);

	void AddPatch(const repo::Patch& patch);
	void Clear();

	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;
	virtual bool HasChanges() const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

private:
	std::vector<Json::Value> m_patchJsonList;
};

}