#include "RenameColumnPatch.h"

#include "SqlTools.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

RenameColumnPatch::RenameColumnPatch(int64_t schemaVersion, const std::string& tableName)
	: repo::Patch(schemaVersion)
	, m_tableName(tableName)
{
}

RenameColumnPatch::RenameColumnPatch(const std::string& tableName)
	: repo::Patch()
	, m_tableName(tableName)
{
}

RenameColumnPatch::RenameColumnPatch() : Patch()
{
}

void RenameColumnPatch::RenameColumn(const std::string & oldColumnName, const std::string & newColumnName)
{
	m_columnNames[oldColumnName] = newColumnName;
}

void RenameColumnPatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;

	Json::Value columns;
	for (const auto& name : m_columnNames) {
		columns[name.first] = name.second;
	}
	json["columns"] = columns;
}

void RenameColumnPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();
	
	const Json::Value columns = json["columns"];
	if (!columns.isObject()) {
		return;
	}

	for (const auto& name : columns.getMemberNames()) {
		m_columnNames[name] = columns[name].asString();
	}
}

std::string RenameColumnPatch::GetType() const
{
	return typeid(RenameColumnPatch).name();
}

void RenameColumnPatch::Apply(ISession& session) const
{
	std::stringstream alterTable;

	for (const auto& name : m_columnNames) {
		alterTable << "alter table " << m_tableName;
		alterTable << " rename column " << name.first;
		alterTable << " to " << name.second << ";";
	}

	session.ExecSql(alterTable.str());
}

}

REGISTER_PATCH(repo::RenameColumnPatch)
