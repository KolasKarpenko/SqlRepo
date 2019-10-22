#pragma once

#include <map>
#include "Patch.h"

namespace repo
{

class DataSchema
{
public:
	DataSchema(const std::string& name = "");

	void Register(const Patch& patch);
	void CreateSchema(repo::ISession& session) const;
	void UpdateSchema(repo::ISession& session) const;

	const std::string& GetName() const;
private:
	std::map<int64_t, Json::Value> m_patches;
	std::string m_name;
};

}