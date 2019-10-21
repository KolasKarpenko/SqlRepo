#include "SqlTools.h"

#include <regex>
#include <sstream>

std::string SqlTools::GetTextValue(const std::string & str)
{
	return "'" + EscapeQuotes(str) + "'";
}

std::string SqlTools::GetCppTextValue(const std::string & str)
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

std::string SqlTools::EscapeQuotes(const std::string & str) {
	std::regex reg("'");
	return std::regex_replace(str, reg, "''");
}

std::string SqlTools::ToLower(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

std::vector<std::string> SqlTools::Split(const std::string& s, char delimiter)
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
