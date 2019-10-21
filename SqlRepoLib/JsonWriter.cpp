#include "JsonWriter.h"

JsonWriter JsonWriter::m_instance;

std::string JsonWriter::Write(const Json::Value& json)
{
	Json::OStringStream ss;
	m_instance.m_writer->write(json, &ss);
	return ss.str();
}

JsonWriter::JsonWriter()
{
	Json::StreamWriterBuilder fact;
	m_writer = fact.newStreamWriter();
}

JsonWriter::~JsonWriter()
{
	delete m_writer;
}
