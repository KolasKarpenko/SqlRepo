#include "Patch.h"

#include "Tools.h"
#include "JsonWriter.h"

namespace repo
{
Json::Value Patch::ToJson() const
{
	Json::Value json;
	json["type"] = GetType();

	if (!m_schemaVersion.IsNull()) {
		json["schemaVersion"] = m_schemaVersion.Data();
	}

	ToJsonImpl(json);

	return json;
}

void Patch::FromJson(const Json::Value& json)
{
	if (json["schemaVersion"].isInt64()) {
		m_schemaVersion = json["schemaVersion"].asInt64();
	}

	FromJsonImpl(json);
}

bool Patch::HasChanges() const
{
	return true;
}

const repo::SqlBigInt& Patch::GetSchemaVersion() const
{
	return m_schemaVersion;
}

void Patch::ApplyJournal(ISession& session) const
{
	if (!HasChanges()) {
		return;
	}

	session.ExecSql(session.BeginTransaction().c_str());

	try {
		Apply(session);

		const std::string& contextId = session.GetContextId();
		std::stringstream insertJournal;
		insertJournal << "insert into private_Journal";
		insertJournal << " (patchJson";
		if (!contextId.empty()) {
			insertJournal << ", contextId";
		}
		if (!m_schemaVersion.IsNull()) {
			insertJournal << ", schemaVersion";
		}
		insertJournal << ")";
		insertJournal << " values (";
		insertJournal << Tools::GetTextValue(JsonWriter::Write(ToJson()));
		if (!contextId.empty()) {
			insertJournal << ", " << Tools::GetTextValue(contextId);
		}
		if (!m_schemaVersion.IsNull()) {
			insertJournal << ", " << m_schemaVersion.Data();
		}
		insertJournal << ");";

		session.ExecSql(insertJournal.str().c_str());
	}
	catch (const std::exception&) {
		session.ExecSql(session.RollbackTransaction().c_str());
		throw;
	}

	session.ExecSql(session.CommitTransaction().c_str());
}

Patch::Patch()
{
}

Patch::Patch(int64_t schemaVersion) : m_schemaVersion(schemaVersion)
{
}

}