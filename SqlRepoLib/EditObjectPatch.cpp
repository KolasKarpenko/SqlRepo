#include "EditObjectPatch.h"

#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

EditObjectPatch::EditObjectPatch(const std::string& tableName, const std::string& id)
	: m_tableName(tableName),
	m_id(id)
{
}

EditObjectPatch::EditObjectPatch(int64_t schemaVersion, const std::string& tableName, const std::string& id)
	: repo::Patch(schemaVersion),
	m_tableName(tableName),
	m_id(id)
{
}

EditObjectPatch::EditObjectPatch()
{
}

void EditObjectPatch::SetValue(const std::string &field, const ISqlValue& value)
{
	m_fieldValues[field] = value.ToSqlValue();
}

void EditObjectPatch::ResetValue(const std::string& field)
{
	m_fieldValues.erase(field);
}

void EditObjectPatch::Clear()
{
	m_fieldValues.clear();
}

std::string EditObjectPatch::GetObjectId() const
{
	return m_id;
}

void EditObjectPatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;
	json["id"] = m_id;

	Json::Value fields;
	for (const auto& field : m_fieldValues) {
		fields[field.first] = field.second;
	}

	json["fields"] = fields;
}

void EditObjectPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();
	m_id = json["id"].asString();

	Json::Value fields = json["fields"];
	if (!fields.isObject()) {
		return;
	}

	m_fieldValues.clear();
	for (const auto& field : fields.getMemberNames()) {
		m_fieldValues[field] = fields[field].asString();
	}
}

std::string EditObjectPatch::GetType() const
{
	return typeid(EditObjectPatch).name();
}

bool EditObjectPatch::HasChanges() const
{
	return !m_fieldValues.empty();
}

void EditObjectPatch::Apply(ISession& session) const
{
	std::stringstream query;

	query << "update " << m_tableName;
	query << " set ";

	bool first = true;
	for (const auto& field : m_fieldValues) {
		if (!first) {
			query << ", ";
		}
		query << field.first << "=" << field.second;
		first = false;
	}

	query << ";";

	session.ExecSql(query.str());
}

}

REGISTER_PATCH(repo::EditObjectPatch)
