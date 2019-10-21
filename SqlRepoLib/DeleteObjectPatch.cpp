#include "DeleteObjectPatch.h"

#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

DeleteObjectPatch::DeleteObjectPatch(const std::string& tableName, const std::string& id)
	: m_tableName(tableName),
	m_id(id)
{
}

DeleteObjectPatch::DeleteObjectPatch(int64_t schemaVesion, const std::string& tableName, const std::string& id)
	: repo::Patch(schemaVesion),
	m_tableName(tableName),
	m_id(id)
{
}

DeleteObjectPatch::DeleteObjectPatch()
{
}

std::string DeleteObjectPatch::GetType() const
{
	return typeid(DeleteObjectPatch).name();
}

void DeleteObjectPatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;
	json["id"] = m_id;
}

void DeleteObjectPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();
	m_id = json["id"].asString();
}

void DeleteObjectPatch::Apply(ISession& session) const
{
	std::stringstream query;

	query << "delete from " << m_tableName;
	query << " where id = ";
	query << SqlTools::GetTextValue(m_id);
	query << ";";

	session.ExecSql(query.str());
}

}

REGISTER_PATCH(repo::DeleteObjectPatch)
