#pragma once

#include <json/json.h>

class JsonReader
{
public:
	static Json::Value Read(const std::string& str);
private:
	JsonReader();
	~JsonReader();
	Json::CharReader *m_reader;
	static JsonReader m_instance;
};

