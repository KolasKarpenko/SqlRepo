#include "DataSchemaDeploy.h"

#include <set>
#include <sstream>

#include "PatchFactory.h"

namespace repo
{

void DataSchemaDeploy::Register(const Patch& patch) {
	const repo::SqlBigInt& version = patch.GetSchemaVersion();
	if (!version.IsNull()) {
		m_patches[version.Data()] = patch.ToJson();
	}
}

void DataSchemaDeploy::CreateSchema(repo::ISession& session) {
	for (const auto& pair : m_patches) {
		std::shared_ptr<Patch> patchPtr = CreatePatch(pair.second);
		if (patchPtr) {
			patchPtr->ApplyJournal(session);
		}
	}
}

void DataSchemaDeploy::UpdateSchema(repo::ISession& session) {
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

}

