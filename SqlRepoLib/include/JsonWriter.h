#pragma once

#include <json/json.h>

class JsonWriter
{
public:
	static std::string Write(const Json::Value& json);
private:
	JsonWriter();
	~JsonWriter();
	Json::StreamWriter *m_writer;
	static JsonWriter m_instance;
};

