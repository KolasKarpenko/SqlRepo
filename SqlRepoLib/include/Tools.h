#pragma once

#include <string>
#include <vector>
#include <set>
#include <json/json.h>
#include <uuid.h>

namespace uuids {
	struct uuid;
}

namespace repo
{

class Tools
{
public:
	static std::string GetTextValue(const std::string& str);
	static std::string GetCppTextValue(const std::string& str);
	static std::string ToLower(const std::string& str);
	static std::vector<std::string> Split(const std::string& s, char delimiter);
	static void IncrementUuids(Json::Value& obj, const uuids::uuid& increment, const std::set<uuids::uuid>& includes = {});
private:
	static std::string EscapeQuotes(const std::string& str);
};

}

