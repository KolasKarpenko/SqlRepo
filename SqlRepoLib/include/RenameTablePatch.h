#pragma once

#include "Patch.h"
#include "TableColumnStruct.h"

namespace repo
{

class RenameTablePatch : public repo::Patch
{
public:
	RenameTablePatch(int64_t schemaVersion, const std::string& oldTableName, const std::string& newTableName);
	RenameTablePatch(const std::string& oldTableName, const std::string& newTableName);
	RenameTablePatch();

	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_oldTableName;
	std::string m_newTableName;
};

}
