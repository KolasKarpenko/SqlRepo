#pragma once

#include <json/json.h>

namespace repo
{

struct TableColumnStruct
{
	struct Type
	{
		static const char* text;
		static const char* integer;
		static const char* bigint;
		static const char* real;
		static const char* timestamp;
		static const char* blob;
	};

	TableColumnStruct(
		const std::string& name,
		const std::string& type,
		const std::string& defaultValue,
		bool isNotNull = false,
		bool isPrimaryKey = false,
		bool isUnique = false
	);

	TableColumnStruct(const Json::Value& json);

	Json::Value ToJson() const;

	std::string GetSqlDefinition() const;

	std::string name;
	std::string type;
	std::string defaultValue;
	bool isNotNull;
	bool isPrimaryKey;
	bool isUnique;
};

}
