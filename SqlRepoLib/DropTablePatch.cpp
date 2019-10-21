#include "DropTablePatch.h"

#include "SqlTools.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

DropTablePatch::DropTablePatch(int64_t schemaVersion, const std::string& tableName)
	: repo::Patch(schemaVersion)
	, m_tableName(tableName)
{
}

DropTablePatch::DropTablePatch(const std::string& tableName)
	: repo::Patch()
	, m_tableName(tableName)
{
}

DropTablePatch::DropTablePatch() : repo::Patch()
{
}

void DropTablePatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;
}

void DropTablePatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();
}

std::string DropTablePatch::GetType() const
{
	return typeid(DropTablePatch).name();
}

void DropTablePatch::Apply(ISession& session) const
{
	std::stringstream deleteTable;
	deleteTable << "drop table " << m_tableName << ";";

	std::stringstream updateChildParent;
	updateChildParent << "delete from private_ChildToParentNames where child = " << SqlTools::GetTextValue(m_tableName) << ";";


	session.ExecSql(deleteTable.str());
	session.ExecSql(updateChildParent.str());
}

}

REGISTER_PATCH(repo::DropTablePatch)
