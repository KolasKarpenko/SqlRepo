#pragma once

#include <string>
#include <vector>

class SqlTools
{
public:
	static std::string GetTextValue(const std::string& str);
	static std::string GetCppTextValue(const std::string& str);
	static std::string ToLower(const std::string& str);
	static std::vector<std::string> Split(const std::string& s, char delimiter);
private:
	static std::string EscapeQuotes(const std::string& str);
};

