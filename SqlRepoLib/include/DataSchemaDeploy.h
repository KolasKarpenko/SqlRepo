#pragma once

#include <map>
#include "Patch.h"

namespace repo
{

class DataSchemaDeploy
{
public:
	void Register(const Patch& patch);
	void CreateSchema(repo::ISession& session);
	void UpdateSchema(repo::ISession& session);
private:
	std::map<int64_t, Json::Value> m_patches;
};

}