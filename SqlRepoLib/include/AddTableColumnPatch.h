#pragma once

#include <vector>

#include "Patch.h"
#include "TableColumnStruct.h"

namespace repo
{

class AddTableColumnPatch : public repo::Patch
{
public:
	AddTableColumnPatch(int64_t schemaVersion, const std::string& tableName);
	AddTableColumnPatch(const std::string& tableName);
	AddTableColumnPatch();

	virtual std::string GetType() const override;

	void AddColumn(
		const std::string& name,
		const std::string& type,
		const std::string& defaultValue = "",
		bool isNotNull = false,
		bool isPrimaryKey = false,
		bool isUnique = false
	);

	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_tableName;
	std::vector<repo::TableColumnStruct> m_columns;
};

}
