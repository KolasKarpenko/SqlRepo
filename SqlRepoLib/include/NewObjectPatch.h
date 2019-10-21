#pragma once

#include "EditObjectPatch.h"

namespace repo
{

class NewObjectPatch : public repo::EditObjectPatch
{
public:
	NewObjectPatch(const std::string& tableName);
	NewObjectPatch(int64_t schemaVersion, const std::string& tableName);
	NewObjectPatch();

	virtual void Clear() override;

	virtual std::string GetType() const override;
	virtual bool HasChanges() const override;
	virtual void Apply(ISession& session) const override;
protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;
};

}