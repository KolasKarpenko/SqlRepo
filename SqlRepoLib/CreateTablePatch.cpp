#include "CreateTablePatch.h"

#include "Tools.h"
#include "JsonWriter.h"
#include "JsonReader.h"
#include "PatchFactory.h"

namespace repo
{

CreateTablePatch::CreateTablePatch(int64_t schemaVersion, const std::string& tableName, const std::string& parentTableName)
	: repo::Patch(schemaVersion)
	, m_tableName(tableName)
	, m_parentTableName(parentTableName)
{
}

CreateTablePatch::CreateTablePatch(const std::string& tableName, const std::string& parentTableName)
	: repo::Patch()
	, m_tableName(tableName)
	, m_parentTableName(parentTableName)
{
}

CreateTablePatch::CreateTablePatch() : repo::Patch()
{
}

std::string CreateTablePatch::GetType() const
{
	return typeid(CreateTablePatch).name();
}

void CreateTablePatch::AddColumn(
	const std::string& name, 
	const std::string& type, 
	const std::string& defaultValue, 
	bool isNotNull, 
	bool isPrimaryKey,
	bool isUnique
) {
	TableColumnStruct column(name, type, defaultValue, isNotNull, isPrimaryKey, isUnique);
	m_columns.emplace_back(column);
}

void CreateTablePatch::ToJsonImpl(Json::Value& json) const
{
	json["tableName"] = m_tableName;

	if (!m_parentTableName.empty()) {
		json["parentTableName"] = m_parentTableName;
	}

	Json::Value columns;
	int i = 0;
	for (const auto& c : m_columns) {
		columns[i++] = c.ToJson();
	}
	json["columns"] = columns;
}

void CreateTablePatch::FromJsonImpl(const Json::Value& json)
{
	if (!json.isObject()) {
		return;
	}

	m_tableName = json["tableName"].asString();

	if (json["parentTableName"].isString()) {
		m_parentTableName = json["parentTableName"].asString();
	}

	Json::Value columns = json["columns"];
	if (!columns.isArray()) {
		return;
	}

	m_columns.clear();
	for (Json::ArrayIndex i = 0; i < columns.size(); ++i) {
		repo::TableColumnStruct c(columns[i]);
		m_columns.emplace_back(c);
	}
}

void CreateTablePatch::Apply(ISession& session) const
{
	std::stringstream createTable;
	createTable << "create table if not exists " << m_tableName;
	createTable << " (";
	createTable << "id " << session.SqlDialect()->TextKeyType() <<" primary key not null unique";

	if (!m_parentTableName.empty()) {
		createTable << ", parentId " << session.SqlDialect()->TextKeyType() << " not null";
	}

	for (const auto& column : m_columns) {
		createTable << ", ";
		createTable << column.GetSqlDefinition();
	}

	createTable << ");";

	std::stringstream updateChildParent;
	updateChildParent << "insert into private_ChildToParentNames";
	updateChildParent << " (child";

	if (!m_parentTableName.empty()) {
		updateChildParent << ", parent";
	}

	updateChildParent << ") values (";
	updateChildParent << Tools::GetTextValue(m_tableName);

	if (!m_parentTableName.empty()) {
		updateChildParent << ", " << Tools::GetTextValue(m_parentTableName);
	}

	updateChildParent << ");";

	session.ExecSql(createTable.str());
	session.ExecSql(updateChildParent.str());

	std::stringstream createIndex;
	createIndex << "create index idx_" << m_tableName << "_id";
	createIndex << " on " << m_tableName;
	createIndex << " (id);";

	session.ExecSql(createIndex.str());

	if (!m_parentTableName.empty()) {
		std::stringstream parentIndex;
		parentIndex << "create index idx_" << m_tableName << "_parentId";
		parentIndex << " on " << m_tableName;
		parentIndex << " (parentId);";
		session.ExecSql(parentIndex.str());
	}

}

}

REGISTER_PATCH(repo::CreateTablePatch)
