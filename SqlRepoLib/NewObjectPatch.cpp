#include "NewObjectPatch.h"

#include "Tools.h"
#include "uuid.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

NewObjectPatch::NewObjectPatch(const std::string& tableName)
	: EditObjectPatch(tableName, uuids::create().to_string())
{
}

NewObjectPatch::NewObjectPatch(int64_t schemaVersion, const std::string& tableName)
	: EditObjectPatch(schemaVersion, tableName, uuids::create().to_string())
{
}

NewObjectPatch::NewObjectPatch() : repo::EditObjectPatch()
{
}

void NewObjectPatch::Clear()
{
	m_id.clear();
	EditObjectPatch::Clear();
}

void NewObjectPatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;
	json["id"] = m_id;

	Json::Value fields;
	for (const auto& field : m_fieldValues) {
		fields[field.first] = field.second;
	}

	json["fields"] = fields;
}

void NewObjectPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();
	m_id= json["id"].asString();

	Json::Value fields = json["fields"];
	if (!fields.isObject()) {
		return;
	}

	m_fieldValues.clear();
	for (const auto& field : fields.getMemberNames()) {
		m_fieldValues[field] = fields[field].asString();
	}
}

std::string NewObjectPatch::GetType() const
{
	return typeid(NewObjectPatch).name();
}

bool NewObjectPatch::HasChanges() const
{
	return !m_id.empty();
}

void NewObjectPatch::Apply(ISession& session) const
{
	std::stringstream query;

	query << "insert into " << m_tableName;
	query << " (id";

	for (const auto& field : m_fieldValues) {
		query << ", " << field.first;
	}

	query << ") values (";
	query << Tools::GetTextValue(m_id);

	for (const auto& field : m_fieldValues) {
		query << ", " << field.second;
	}

	query << ");";

	session.ExecSql(query.str());
}

}

REGISTER_PATCH(repo::NewObjectPatch)
