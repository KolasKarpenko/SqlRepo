#include "RenameTablePatch.h"

#include "SqlTools.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

RenameTablePatch::RenameTablePatch(int64_t schemaVersion, const std::string& oldTableName, const std::string& newTableName)
	: repo::Patch(schemaVersion)
	, m_oldTableName(oldTableName)
	, m_newTableName(newTableName)
{
}

RenameTablePatch::RenameTablePatch(const std::string& oldTableName, const std::string& newTableName)
	: repo::Patch()
	, m_oldTableName(oldTableName)
	, m_newTableName(newTableName) {
}

RenameTablePatch::RenameTablePatch() : Patch()
{
}

void RenameTablePatch::ToJsonImpl(Json::Value& json) const
{
	json["oldTableName"] = m_oldTableName;
	json["newTableName"] = m_newTableName;
}

void RenameTablePatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_oldTableName = json["oldTableName"].asString();
	m_newTableName = json["newTableName"].asString();
}

std::string RenameTablePatch::GetType() const
{
	return typeid(RenameTablePatch).name();
}

void RenameTablePatch::Apply(ISession& session) const
{
	std::stringstream alterTable;
	alterTable << "alter table " << m_oldTableName;
	alterTable << " rename to " << m_newTableName << ";";

	std::stringstream updateChildParent;
	updateChildParent << "update private_ChildToParentNames";
	updateChildParent << " set child = " << SqlTools::GetTextValue(m_newTableName);
	updateChildParent << " where child = " << SqlTools::GetTextValue(m_oldTableName) << ";";

	updateChildParent << "update private_ChildToParentNames";
	updateChildParent << " set parent = " << SqlTools::GetTextValue(m_newTableName);
	updateChildParent << " where parent = " << SqlTools::GetTextValue(m_oldTableName) << ";";

	session.ExecSql(alterTable.str());
	session.ExecSql(updateChildParent.str());
}

}

REGISTER_PATCH(repo::RenameTablePatch)
