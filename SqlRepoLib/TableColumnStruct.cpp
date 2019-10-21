#include "TableColumnStruct.h"

#include <algorithm>
#include <sstream>

#include "SqlTools.h"

namespace repo
{

const char* TableColumnStruct::Type::text = "text";
const char* TableColumnStruct::Type::integer = "integer";
const char* TableColumnStruct::Type::bigint = "bigint";
const char* TableColumnStruct::Type::real = "real";
const char* TableColumnStruct::Type::timestamp = "timestamp";
const char* TableColumnStruct::Type::blob = "blob";

TableColumnStruct::TableColumnStruct(
	const std::string& name,
	const std::string& type,
	const std::string& defaultValue,
	bool isNotNull,
	bool isPrimaryKey,
	bool isUnique
) : name(name),
	type(SqlTools::ToLower(type)),
	defaultValue(defaultValue),
	isNotNull(isNotNull),
	isPrimaryKey(isPrimaryKey),
	isUnique(isUnique)
{
}

TableColumnStruct::TableColumnStruct(const Json::Value& json):
	TableColumnStruct(
		json["name"].asString()
		, json["type"].asString()
		, json["defaultValue"].asString()
		, json["isNotNull"].asBool()
		, json["isPrimaryKey"].asBool()
		, json["isUnique"].asBool()
	)
{
}

Json::Value TableColumnStruct::ToJson() const
{
	Json::Value val;
	val["name"] = name;
	val["type"] = type;
	val["defaultValue"] = defaultValue;
	val["isNotNull"] = isNotNull;
	val["isPrimaryKey"] = isPrimaryKey;
	val["isUnique"] = isUnique;
	return val;
}

std::string TableColumnStruct::GetSqlDefinition() const
{
	std::stringstream ss;
	ss << name;
	ss << " " << type;

	if (isPrimaryKey) {
		ss << " primary key";
	}

	if (isNotNull) {
		ss << " not null";
	}

	if (isUnique) {
		ss << " unique";
	}

	/* todo mysql default text
	if (!defaultValue.empty()) {
		ss << " default ";
		if (type == Type::text) {
			ss << SqlTools::GetTextValue(defaultValue);
		}
		else {
			ss << defaultValue;
		}
	}
	*/

	return ss.str();
}

}