#include "Tools.h"

#include <regex>
#include <sstream>

std::string Tools::GetTextValue(const std::string & str)
{
	return "'" + EscapeQuotes(str) + "'";
}

std::string Tools::GetCppTextValue(const std::string & str)
{
	std::string result = str;

	if (result.front() == '\'') {
		result.erase(result.begin());
	}

	if (result.back() == '\'') {
		result.pop_back();
	}

	std::regex reg("\"");
	return "\"" + std::regex_replace(result, reg, "\\\"") +"\"";
}

std::string Tools::EscapeQuotes(const std::string & str) {
	std::regex reg("'");
	return std::regex_replace(str, reg, "''");
}

std::string Tools::ToLower(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

std::vector<std::string> Tools::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void Tools::IncrementUuids(Json::Value& obj, const uuids::uuid& increment)
{
	if (obj.isString()) {
		uuids::uuid uuid = uuids::uuid::from_string(obj.asString());

		if (!uuid.is_nil()) {
			uuid = uuid + increment;
			obj = uuid.to_string();
		}

		return;
	}

	if (obj.isObject()) {
		for (auto& field : obj) {
			IncrementUuids(field, increment);
		}
	}
}
