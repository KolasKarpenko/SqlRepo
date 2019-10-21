#pragma once

#include <map>

#include "Patch.h"
#include "TableColumnStruct.h"

namespace repo
{

class RenameColumnPatch : public repo::Patch
{
public:
	RenameColumnPatch(int64_t schemaVersion, const std::string& tableName);
	RenameColumnPatch(const std::string& tableName);
	RenameColumnPatch();

	void RenameColumn(const std::string& oldColumnName, const std::string& newColumnName);

	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_tableName;
	std::map<std::string, std::string> m_columnNames;
};

}
