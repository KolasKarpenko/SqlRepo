#include "TransactionPatch.h"

#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

TransactionPatch::TransactionPatch()
{
}

TransactionPatch::TransactionPatch(int64_t schemaVersion) : repo::Patch(schemaVersion)
{
}

void TransactionPatch::AddPatch(const repo::Patch& patch)
{
	if (patch.HasChanges()) {
		m_patchJsonList.push_back(patch.ToJson());
	}
}

void TransactionPatch::Clear()
{
	m_patchJsonList.clear();
}

void TransactionPatch::ToJsonImpl(Json::Value& json) const
{
	if (m_patchJsonList.size() == 1) {
		Json::Value copy = json;
		json = m_patchJsonList[0];

		for (const auto& m : copy.getMemberNames()) {
			if (!json.isMember(m)) {
				json[m] = copy[m];
			}
		}
	}
	else {
		Json::Value list(Json::arrayValue);
		int i = 0;
		for (const auto& p : m_patchJsonList) {
			list[i++] = p;
		}
		json["list"] = list;
	}
}

void TransactionPatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	const Json::Value list = json["list"];
	if (!list.isArray()) {
		return;
	}

	m_patchJsonList.clear();
	for (Json::ArrayIndex i = 0; i < list.size(); ++i) {
		m_patchJsonList.push_back(list[i]);
	}
}

std::string TransactionPatch::GetType() const
{
	return typeid(TransactionPatch).name();
}

void TransactionPatch::Apply(ISession& session) const
{
	for (const auto& patchJson : m_patchJsonList) {
		const auto patch = CreatePatch(patchJson);
		patch->Apply(session);
	}
}

bool TransactionPatch::HasChanges() const
{
	return !m_patchJsonList.empty();
}

}

REGISTER_PATCH(repo::TransactionPatch)
