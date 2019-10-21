#pragma once

#include "Patch.h"

namespace repo
{

class DeleteObjectPatch : public repo::Patch
{
public:
	DeleteObjectPatch(const std::string& tableName, const std::string& id);
	DeleteObjectPatch(int64_t schemaVesion, const std::string& tableName, const std::string& id);
	DeleteObjectPatch();

	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_tableName;
	std::string m_id;
};

}