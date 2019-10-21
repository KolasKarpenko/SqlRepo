#include "AddTableColumnPatch.h"

#include "SqlTools.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

AddTableColumnPatch::AddTableColumnPatch(int64_t schemaVersion, const std::string& tableName)
	: repo::Patch(schemaVersion)
	, m_tableName(tableName)
{
}

AddTableColumnPatch::AddTableColumnPatch(const std::string& tableName)
	: repo::Patch()
	, m_tableName(tableName)
{
}

AddTableColumnPatch::AddTableColumnPatch() : repo::Patch()
{
}

std::string AddTableColumnPatch::GetType() const
{
	return typeid(AddTableColumnPatch).name();
}

void AddTableColumnPatch::AddColumn(
	const std::string& name,
	const std::string& type,
	const std::string& defaultValue,
	bool isNotNull,
	bool isPrimaryKey,
	bool isUnique
) {
	TableColumnStruct column(name, type, defaultValue, isNotNull, isPrimaryKey, isUnique);
	m_columns.emplace_back(column);
}

void AddTableColumnPatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;

	Json::Value columns;
	int i = 0;
	for (const auto& c : m_columns) {
		columns[i++] = c.ToJson();
	}
	json["columns"] = columns;
}

void AddTableColumnPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();

	Json::Value columns = json["columns"];
	if (!columns.isArray()) {
		return;
	}

	m_columns.clear();
	for (Json::ArrayIndex i = 0; i < columns.size(); ++i) {
		repo::TableColumnStruct c(columns[i]);
		m_columns.emplace_back(c);
	}
}

void AddTableColumnPatch::Apply(ISession& session) const
{
	std::stringstream alterTable;
	for (const auto& column : m_columns) {
		alterTable << "alter table " << m_tableName;
		alterTable << " add column ";
		alterTable << column.GetSqlDefinition() << ";";
	}

	session.ExecSql(alterTable.str());
}

}

REGISTER_PATCH(repo::AddTableColumnPatch)
