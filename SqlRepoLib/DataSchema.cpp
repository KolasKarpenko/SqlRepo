#include "DataSchema.h"

#include <set>
#include <sstream>

#include "PatchFactory.h"

namespace repo
{
DataSchema::DataSchema(const std::string& name) : m_name(name)
{
}

void DataSchema::Register(const Patch& patch) {
	const repo::SqlBigInt& version = patch.GetSchemaVersion();
	if (!version.IsNull()) {
		m_patches[version.Data()] = patch.ToJson();
	}
}

void DataSchema::CreateSchema(repo::ISession& session) const {
	for (const auto& pair : m_patches) {
		std::shared_ptr<Patch> patchPtr = CreatePatch(pair.second);
		if (patchPtr) {
			patchPtr->ApplyJournal(session);
		}
	}
}

void DataSchema::UpdateSchema(repo::ISession& session) const {
	std::stringstream ss;
	ss << "select distinct schemaVersion, id";
	ss << " from private_Journal where schemaVersion is not null order by id;";

	std::set<int64_t> isApplied;
	session.ExecSql(ss.str().c_str(), [&](const repo::IRow& row) {
 		isApplied.insert(row.GetBigInt("schemaVersion").Data());
	});

	for (const auto& pair : m_patches) {
		if (isApplied.count(pair.first) < 1) {
			std::shared_ptr<Patch> patchPtr = CreatePatch(pair.second);
			if (patchPtr) {
				patchPtr->ApplyJournal(session);
			}
		}
	}
}

const std::string& DataSchema::GetName() const
{
	return m_name;
}

}

