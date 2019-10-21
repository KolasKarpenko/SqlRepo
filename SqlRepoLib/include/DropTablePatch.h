#pragma once

#include "Patch.h"

namespace repo
{

class DropTablePatch: public repo::Patch
{
public:
	DropTablePatch(int64_t schemaVersion, const std::string& tableName);
	DropTablePatch(const std::string& tableName);
	DropTablePatch();

	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_tableName;
};

}
