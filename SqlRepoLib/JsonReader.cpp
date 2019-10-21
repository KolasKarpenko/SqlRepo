#include "JsonReader.h"

JsonReader JsonReader::m_instance;

Json::Value JsonReader::Read(const std::string& str)
{
	Json::Value result;
	Json::String errors;
	m_instance.m_reader->parse(&str.front(), &str.back() + 1, &result, &errors);

	if (!errors.empty()) {
		throw Json::Exception(errors);
	}

	return result;
}

JsonReader::JsonReader()
{
	Json::CharReaderBuilder fact;
	m_reader = fact.newCharReader();
}

JsonReader::~JsonReader()
{
	delete m_reader;
}
