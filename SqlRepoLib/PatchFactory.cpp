#include "PatchFactory.h"

namespace repo
{

std::shared_ptr<Patch> CreatePatch(const Json::Value& json)
{
	if (!json["type"].isString()) {
		return nullptr;
	}

	const std::string type = json["type"].asString();
	const PatchRegistry& reg = getPatchRegistry();
	const auto it = reg.find(type);

	if (it == reg.end()) {
		return nullptr;
	}

	CreatePatchFunc func = it->second;
	return func(json);
}

}
