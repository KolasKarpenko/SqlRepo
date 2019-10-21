#pragma once

#include "Patch.h"
#include <map>

namespace repo
{

typedef std::shared_ptr<Patch>(*CreatePatchFunc)(const Json::Value& json);
typedef std::map<std::string, CreatePatchFunc> PatchRegistry;

inline PatchRegistry& getPatchRegistry()
{
	static PatchRegistry reg;
	return reg;
}

template<class T>
std::shared_ptr<Patch> createPatch(const Json::Value& json) {
	std::shared_ptr<Patch> patchPtr(new T);
	patchPtr->FromJson(json);
	return patchPtr;
}

template<class T>
struct RegistryEntry
{
public:
	static RegistryEntry<T>& Instance()
	{
		static RegistryEntry<T> inst;
		return inst;
	}

private:
	RegistryEntry()
	{
		PatchRegistry& reg = getPatchRegistry();
		CreatePatchFunc func = createPatch<T>;
		const std::string type = Patch::GetType<T>();
		reg.insert(PatchRegistry::value_type(type, func));
	}

	RegistryEntry(const RegistryEntry<T>&) = delete;
	RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
};

std::shared_ptr<Patch> CreatePatch(const Json::Value& json);

}

#define REGISTER_PATCH(TYPE)\
namespace\
{\
	template<class T>\
	class PatchRegistration;\
	\
	template<>\
	class PatchRegistration<TYPE>\
	{\
		static const repo::RegistryEntry<TYPE>& reg;\
	};\
	\
	const repo::RegistryEntry<TYPE>&\
		PatchRegistration<TYPE>::reg =\
			repo::RegistryEntry<TYPE>::Instance();\
}
