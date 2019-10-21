#pragma once

#include <map>
#include "Patch.h"

namespace repo
{

class DataSchemaDeploy
{
public:
	static void Register(const Patch& patch);
	static void CreateSchema(repo::ISession& session);
	static void UpdateSchema(repo::ISession& session);
private:
	static std::map<int64_t, Json::Value> ms_patches;
};

}