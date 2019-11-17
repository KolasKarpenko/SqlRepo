#include "TableCppClass.h"

#include "Tools.h"

namespace repo {
namespace cpp {

TableCppClass::Column::Column(const IRow& row, const std::string& tableName)
	: columnStruct(
		row.Data("name").Data(),
		row.Data("type").Data(),
		row.Data("dflt_value").Data(),
		row.GetInteger("notnull").Data() != 0,
		row.GetInteger("pk").Data() != 0
	),
	tableName(tableName)
{}

void TableCppClass::Column::Getter(std::stringstream& ss) const {
	ss << (columnStruct.isNotNull ? GetCType() : GetFullType());
	ss << " Get_" << columnStruct.name << "() const;" << std::endl;
}

void TableCppClass::Column::ImplementGetter(std::stringstream & ss) const
{
	ss << (columnStruct.isNotNull ? GetCType() : GetFullType());
	ss << " " << tableName << "::Get_" << columnStruct.name << "() const { return m_" << columnStruct.name;
	ss << (columnStruct.isNotNull ? ".Data()" : "") << "; }" << std::endl;
}

void TableCppClass::Column::Setter(std::stringstream& ss) const {
	if (columnStruct.isPrimaryKey){
		return;
	}

	ss << "void Set_" << columnStruct.name;
	ss << "(const "; 
	ss << (columnStruct.isNotNull ? GetCType() : GetFullType());
	ss << "& " << "value);" << std::endl;
}

void TableCppClass::Column::ImplementSetter(std::stringstream& ss) const {
	if (columnStruct.isPrimaryKey) {
		return;
	}

	ss << "void " << tableName << "::Set_" << columnStruct.name;
	ss << "(const ";
	ss << (columnStruct.isNotNull ? GetCType() : GetFullType());
	ss << "& " << "value)";
	ss << "{" << std::endl;
	ss << "\tm_" << columnStruct.name << " = value;" << std::endl;
	ss << "\tm_patch->SetValue(\"" << columnStruct.name << "\", m_" << columnStruct.name << ");" << std::endl;
	ss << "}" << std::endl;
}

void TableCppClass::Column::Member(std::stringstream& ss) const {
	ss <<  GetFullType();
	ss << " m_" << columnStruct.name << ";" << std::endl;
}

void TableCppClass::Column::DefaultValue(std::stringstream& ss) const {
	if (columnStruct.defaultValue.empty() || columnStruct.type == TableColumnStruct::Type::timestamp)
		return;

	ss << GetCType();
	ss << " Default_" << columnStruct.name << "() const;" << std::endl;
}

void TableCppClass::Column::ImplementDefaultValue(std::stringstream& ss) const {
	if (columnStruct.defaultValue.empty() || columnStruct.type == TableColumnStruct::Type::timestamp)
		return;

	ss << GetCType();
	ss << " " << tableName << "::Default_" << columnStruct.name << "() const { return ";

	if (columnStruct.type == TableColumnStruct::Type::text) {
		ss << Tools::GetCppTextValue(columnStruct.defaultValue);
	}
	else {
		ss << columnStruct.defaultValue;
	}
	ss << "; }" << std::endl;
}

std::string TableCppClass::Column::GetCType() const {
	if (columnStruct.type == TableColumnStruct::Type::text) {
		return "std::string";
	}

	if (columnStruct.type == TableColumnStruct::Type::blob) {
		return "std::string";
	}

	if (columnStruct.type == TableColumnStruct::Type::integer) {
		return "int32_t";
	}

	if (columnStruct.type == TableColumnStruct::Type::bigint) {
		return "int64_t";
	}

	if (columnStruct.type == TableColumnStruct::Type::real) {
		return "double";
	}

	if (columnStruct.type == TableColumnStruct::Type::timestamp) {
		return "std::string";
	}

	throw std::runtime_error(std::string("unsupported type: ") + columnStruct.type);
}

std::string TableCppClass::Column::GetFullType() const {
	if (columnStruct.type == TableColumnStruct::Type::text) {
		return "repo::SqlString";
	}

	if (columnStruct.type == TableColumnStruct::Type::blob) {
		return "repo::SqlString";
	}

	if (columnStruct.type == TableColumnStruct::Type::integer) {
		return "repo::SqlInteger";
	}

	if (columnStruct.type == TableColumnStruct::Type::bigint) {
		return "repo::SqlBigInt";
	}

	if (columnStruct.type == TableColumnStruct::Type::real) {
		return "repo::SqlDouble";
	}

	if (columnStruct.type == TableColumnStruct::Type::timestamp) {
		return "repo::SqlString";
	}

	throw std::runtime_error(std::string("unsupported type: ") + columnStruct.type);
}

void TableCppClass::Column::GetRowMethod(std::stringstream& ss, bool notNull) const {
	if (columnStruct.type == TableColumnStruct::Type::text) {
		ss << (columnStruct.isNotNull || notNull ? "Data(Field::" + columnStruct.name + ").Data()" : "Data(Field::" + columnStruct.name + ")");
		return;
	}

	if (columnStruct.type == TableColumnStruct::Type::blob) {
		ss << (columnStruct.isNotNull || notNull ? "Data(Field::" + columnStruct.name + ").Data()" : "Data(Field::" + columnStruct.name + ")");
		return;
	}

	if (columnStruct.type == TableColumnStruct::Type::integer) {
		ss << (columnStruct.isNotNull || notNull ? "GetInteger(Field::" + columnStruct.name + ").Data()" : "GetInteger(Field::" + columnStruct.name + ")");
		return;
	}

	if (columnStruct.type == TableColumnStruct::Type::bigint) {
		ss << (columnStruct.isNotNull || notNull ? "GetBigInt(Field::" + columnStruct.name + ").Data()" : "GetBigInt(Field::" + columnStruct.name + ")");
		return;
	}

	if (columnStruct.type == TableColumnStruct::Type::real) {
		ss << (columnStruct.isNotNull || notNull ? "GetReal(Field::" + columnStruct.name + ").Data()" : "GetReal(Field::" + columnStruct.name + ")");
		return;
	}

	if (columnStruct.type == TableColumnStruct::Type::timestamp) {
		ss << (columnStruct.isNotNull || notNull ? "Data(Field::" + columnStruct.name + ").Data()" : "Data(Field::" + columnStruct.name + ")");
		return;
	}

	throw std::runtime_error(std::string("unsupported type: ") + columnStruct.type);
}

TableCppClass::TableCppClass(ISession& session, const std::string& tableName)
	: m_session(session), m_name(tableName)
{
	std::stringstream tableQuery;
	tableQuery << "pragma table_info(" << tableName << ");";

	m_session.ExecSql(tableQuery.str().c_str(), [this, &tableName](const repo::IRow& row) {
		Column c(row, tableName);
		m_columns.emplace_back(c);
	});

	std::stringstream parentTableQuery;
	parentTableQuery << "select parent from private_ChildToParentNames";
	parentTableQuery << " where child = " << Tools::GetTextValue(tableName) << ";";

	m_session.ExecSql(parentTableQuery.str().c_str(), [this](const repo::IRow& row) {
		repo::SqlString parent = row.Data("parent");

		if (!parent.IsNull()) {
			m_parentName = parent.Data();
		}
	});

}

std::string TableCppClass::GetHeaderText() const
{
	std::stringstream ss;
	BeginClass(ss);
	Public(ss);
	Private(ss);
	EndClass(ss);
	return ss.str();
}

std::string TableCppClass::GetCppText(const std::string& headerPath) const
{
	std::stringstream ss;
	
	ImplementNames(ss);
	ImplementConstructor(ss);

	for (const auto& c : m_columns) {
		c.ImplementGetter(ss);
		c.ImplementDefaultValue(ss);

		if (!IsTablePrivate()) {
			c.ImplementSetter(ss);
		}
	}

	ImplementSave(ss);
	ImplementDelete(ss);
	ImplementToJson(ss);
	if (!IsTablePrivate()) {
		ImplementFromJson(ss);
	}
	ImplementCopy(ss);
	ImplementDeepCopy(ss);

	return ss.str();
}

void TableCppClass::BeginClass(std::stringstream& ss) const {
	ss << "class " << m_name << std::endl;
	ss << "{" << std::endl;
}

void TableCppClass::Constructor(std::stringstream& ss) const {
	ss << m_name << "(const repo::IRow& row);" << std::endl;

	if (!IsTablePrivate()) {
		ss << m_name << "();" << std::endl;
	}
}

void TableCppClass::ImplementConstructor(std::stringstream& ss) const {
	ss << m_name << "::" << m_name << "(const repo::IRow& row) :" << std::endl;

	bool first = true;
	for (const auto& c : m_columns) {
		ss << (first ? "\t" : "\t, ");
		ss << "m_" << c.columnStruct.name << "(row.";
		c.GetRowMethod(ss);
		ss << ")" << std::endl;
		first = false;
	}

	ss << "{" << std::endl;
	if (!IsTablePrivate()) {
		ss << "\tm_patch.reset(new repo::EditObjectPatch(TableName, m_id.Data()));" << std::endl;
	}
	ss << "}" << std::endl;

	if (!IsTablePrivate()) {
		ss << std::endl;
		ss << m_name << "::" << m_name << "()" << std::endl;
		ss << "{" << std::endl;
		ss << "\tm_patch.reset(new repo::NewObjectPatch(TableName));" << std::endl;
		ss << "\tm_id = m_patch->GetObjectId();" << std::endl;
		ss << "}" << std::endl;
	}
}

void TableCppClass::EndClass(std::stringstream& ss) const {
	ss << "};" << std::endl;
}

void TableCppClass::Public(std::stringstream& ss) const {
	ss << "public:" << std::endl;
	ss << "static const char* TableName;" << std::endl;

	if (!m_parentName.empty()) {
		ss << "static const char* ParentTableName;" << std::endl;
	}

	ss << std::endl;

	ss << "struct Field" << std::endl;
	ss << "{" << std::endl;
	for (const auto& c : m_columns) {
		ss << "\tstatic const char* " << c.columnStruct.name << ";" << std::endl;
	}
	ss << "};" << std::endl;

	Constructor(ss);

	for (const auto& c : m_columns) {
		c.Getter(ss);
		c.DefaultValue(ss);

		if (!IsTablePrivate()) {
			c.Setter(ss);
		}
	}

	SaveMethod(ss);
	DeleteMethod(ss);
	ToJsonMethod(ss);
	if (!IsTablePrivate()) {
		FromJsonMethod(ss);
	}
	CopyMethod(ss);
	DeepCopyMethod(ss);
}

void TableCppClass::SaveMethod(std::stringstream & ss) const
{
	if (!IsTablePrivate()) {
		ss << "void Save(repo::TransactionPatch& transaction) const;" << std::endl;
	}
}

void TableCppClass::DeleteMethod(std::stringstream & ss) const
{
	if (!IsTablePrivate()) {
		ss << "void Delete(repo::TransactionPatch& transaction) const;" << std::endl;
	}
}

void TableCppClass::ToJsonMethod(std::stringstream & ss) const
{
	ss << "Json::Value ToJson() const;" << std::endl;
}

void TableCppClass::FromJsonMethod(std::stringstream& ss) const
{
	ss << "void FromJson(const Json::Value& json);" << std::endl;
}

void TableCppClass::CopyMethod(std::stringstream & ss) const
{
	if (!IsTablePrivate()) {
		ss << m_name << " Copy() const;" << std::endl;
	}
}

void TableCppClass::DeepCopyMethod(std::stringstream& ss) const
{
	if (!IsTablePrivate()) {
		ss << m_name << " DeepCopy(repo::ISession& s) const;" << std::endl;
	}
}

void TableCppClass::Private(std::stringstream& ss) const {
	ss << "private:" << std::endl;

	if (!IsTablePrivate()) {
		ss << "mutable std::shared_ptr<repo::EditObjectPatch> m_patch;" << std::endl;
		ss << "mutable std::shared_ptr<repo::TransactionPatch> m_transaction;" << std::endl;
	}

	for (const auto& c : m_columns) {
		c.Member(ss);
	}
}

void TableCppClass::ImplementNames(std::stringstream& ss) const {
	ss << "const char* " << m_name << "::TableName = \"" << m_name << "\";" << std::endl;

	if (!m_parentName.empty()) {
		ss << "const char* " << m_name << "::ParentTableName = \"" << m_parentName << "\";" << std::endl;
	}

	for (const auto& c : m_columns) {
		ss << "const char* " << m_name << "::Field::" << c.columnStruct.name << " = \"" << c.columnStruct.name << "\";" << std::endl;
	}
}

void TableCppClass::ImplementSave(std::stringstream & ss) const
{
	if (!IsTablePrivate()) {
		ss << "void " << m_name << "::Save(repo::TransactionPatch& transaction) const" << std::endl;
		ss << "{" << std::endl;
		ss << "\ttransaction.AddPatch(*m_patch);" << std::endl;
		ss << "\tm_patch->Clear();" << std::endl;
		ss << "\tif (m_transaction) {\n";
		ss << "\t\ttransaction.AddPatch(*m_transaction);\n";
		ss << "\t\tm_transaction = nullptr;\n";
		ss << "\t}\n";
		ss << "}" << std::endl;
	}
}

void TableCppClass::ImplementDelete(std::stringstream & ss) const
{
	if (!IsTablePrivate()) {
		ss << "void " << m_name << "::Delete(repo::TransactionPatch& transaction) const" << std::endl;
		ss << "{" << std::endl;
		ss << "\trepo::DeleteObjectPatch del(TableName, m_id.Data());" << std::endl;
		ss << "\ttransaction.AddPatch(del);" << std::endl;
		ss << "}" << std::endl;
	}
}

void TableCppClass::ImplementToJson(std::stringstream & ss) const
{
	ss << "Json::Value " << m_name << "::ToJson() const" << std::endl;
	ss << "{" << std::endl;
	ss << "\t" << "Json::Value obj;" << std::endl;
	for (const auto& c : m_columns) {
		ss << "\t" << "if (m_" << c.columnStruct.name << ".IsNull()) {" << std::endl;
		ss << "\t" << "\t" << "obj[\"" << c.columnStruct.name << "\"] = Json::Value();" << std::endl;
		ss << "\t" << "} else {" << std::endl;
		ss << "\t" << "\t" << "obj[\"" << c.columnStruct.name << "\"] = m_"<< c.columnStruct.name <<".Data();" << std::endl;
		ss << "\t" << "}" << std::endl;
	}

	ss << "\t" << "return obj;" << std::endl;
	ss << "}" << std::endl;
}

void TableCppClass::ImplementFromJson(std::stringstream& ss) const
{
	ss << "void " << m_name << "::FromJson(const Json::Value& json)" << std::endl;
	ss << "{" << std::endl;
	for (const auto& c : m_columns) {
		if (c.columnStruct.isPrimaryKey) {
			ss << "\t" << "if (json.isMember(\"" << c.columnStruct.name << "\")) {" << std::endl;
			ss << "\t" << "\t" << "const Json::Value& id = json[\"" << c.columnStruct.name << "\"];" << std::endl;
			ss << "\t" << "\t" << "if (id.isString()){" << std::endl;
			ss << "\t" << "\t" << "\t" << "m_id = id.asString();" << std::endl;
			ss << "\t" << "\t" << "\t" << "m_patch->ResetId(m_id.Data());" << std::endl;
			ss << "\t" << "\t" << "}" << std::endl;
			ss << "\t" << "}" << std::endl;
			continue;
		}

		ss << "\t" << "if (json.isMember(\"" << c.columnStruct.name << "\")) {" << std::endl;
		if (
			c.columnStruct.type == TableColumnStruct::Type::text || 
			c.columnStruct.type == TableColumnStruct::Type::timestamp ||
			c.columnStruct.type == TableColumnStruct::Type::blob
		) {
			if (c.columnStruct.isNotNull) {
				ss << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asString());" << std::endl;
			}
			else {
				ss << "\t" << "\t" << "if (json[\"" << c.columnStruct.name << "\"].isNull()) {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(nullptr);" << std::endl;
				ss << "\t" << "\t" << "} else {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asString());" << std::endl;
				ss << "\t" << "\t" << "}" << std::endl;
			}
		}

		if (c.columnStruct.type == TableColumnStruct::Type::integer) {
			if (c.columnStruct.isNotNull) {
				ss << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asInt());" << std::endl;
			}
			else {
				ss << "\t" << "\t" << "if (json[\"" << c.columnStruct.name << "\"].isNull()) {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(nullptr);" << std::endl;
				ss << "\t" << "\t" << "} else {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asInt());" << std::endl;
				ss << "\t" << "\t" << "}" << std::endl;
			}
		}

		if (c.columnStruct.type == TableColumnStruct::Type::bigint) {
			if (c.columnStruct.isNotNull) {
				ss << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asInt64());" << std::endl;
			}
			else {
				ss << "\t" << "\t" << "if (json[\"" << c.columnStruct.name << "\"].isNull()) {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(nullptr);" << std::endl;
				ss << "\t" << "\t" << "} else {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asInt64());" << std::endl;
				ss << "\t" << "\t" << "}" << std::endl;
			}
		}

		if (c.columnStruct.type == TableColumnStruct::Type::real) {
			if (c.columnStruct.isNotNull) {
				ss << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asDouble());" << std::endl;
			}
			else {
				ss << "\t" << "\t" << "if (json[\"" << c.columnStruct.name << "\"].isNull()) {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(nullptr);" << std::endl;
				ss << "\t" << "\t" << "} else {" << std::endl;
				ss << "\t" << "\t" << "\t" << "this->Set_" << c.columnStruct.name << "(json[\"" << c.columnStruct.name << "\"].asDouble());" << std::endl;
				ss << "\t" << "\t" << "}" << std::endl;
			}
		}

		ss << "\t" << "}" << std::endl;
	}
	ss << "}" << std::endl;
}

void TableCppClass::ImplementCopy(std::stringstream & ss) const
{
	if (IsTablePrivate()) {
		return;
	}

	ss << m_name << " " << m_name << "::Copy() const ";
	ss << "{" << std::endl;
	ss << "\t" << m_name << " copy;" << std::endl;

	for (const auto& col : m_columns) {
		if (col.columnStruct.isPrimaryKey) {
			continue;
		}

		ss << "\tcopy.Set_" << col.columnStruct.name << "(Get_" << col.columnStruct.name << "());" << std::endl;
	}

	ss << "\treturn copy;" << std::endl;
	ss << "}" << std::endl;
}

void TableCppClass::ImplementDeepCopy(std::stringstream& ss) const
{
	if (IsTablePrivate()) {
		return;
	}
	
	std::stack <std::string> parentTableStack;
	parentTableStack.push(m_name);

	ss << m_name << " " << m_name << "::DeepCopy(repo::ISession& session) const\n";
	ss << "{\n";
	ss << "\tuuids::uuid incrementUuid = uuids::create();\n";
	ss << "\tstd::map<std::string, std::vector<std::string>> parentIdMap;\n";
	ss << "\tstd::shared_ptr<repo::TransactionPatch> transaction(new repo::TransactionPatch);\n";
	ss << "\tparentIdMap[\"" << m_name << "\"] = {Get_id()};\n";
	ss << "\t" << m_name << " copy = Copy();\n";
	ss << "\tcopy.m_transaction = transaction;\n";
	ss << "\tconst uuids::uuid copyId = uuids::uuid::from_string(Get_id()) + incrementUuid;\n";
	ss << "\tcopy.m_id = copyId.to_string();\n";
	ss << "\tcopy.m_patch->ResetId(copy.m_id.Data());\n";

	while (!parentTableStack.empty()) {
		const std::string parentTable = parentTableStack.top();
		parentTableStack.pop();
		std::stringstream selectChildren;
		selectChildren << "select * from private_ChildToParentNames";
		selectChildren << " where parent = " << Tools::GetTextValue(parentTable);

		m_session.ExecSql(selectChildren.str().c_str(), [&](const repo::IRow& row) {
			ss << "\t{\n";
			ss << "\tstd::vector<std::string> newParentIdList;\n";
			ss << "\tfor(const auto& parentId : parentIdMap[\"" << parentTable << "\"]) {\n";
			const std::string tableName = row.Data("child").Data();
			std::stringstream selectByParent;
			selectByParent << "\"select * from " << tableName << " where parentId = \" << Tools::GetTextValue(parentId)";
			ss << "\t\tstd::stringstream select" << tableName << ";\n";
			ss << "\t\tselect" << tableName << " << " << selectByParent.str() << ";\n";
			ss << "\t\tsession.ExecSql(select" << tableName << ".str().c_str(), [&](const repo::IRow& row) {\n";
			ss << "\t\t\t" << tableName << " obj(row);\n";
			ss << "\t\t\tJson::Value asJson = obj.ToJson();\n";
			ss << "\t\t\tTools::IncrementUuids(asJson, incrementUuid);\n";
			ss << "\t\t\t" << tableName << " copy;\n";
			ss << "\t\t\tcopy .FromJson(asJson);\n";
			ss << "\t\t\tnewParentIdList.push_back(obj.Get_id());\n";
			ss << "\t\t\tcopy.Save(*transaction);\n";
			ss << "\t\t});\n";
			ss << "\t}\n";
			ss << "\tparentIdMap[\"" << tableName << "\"] = newParentIdList;\n";
			ss << "\t}\n";
			parentTableStack.push(tableName);
			});
	}

	ss << "\treturn copy;\n";
	ss << "}\n";
}

bool TableCppClass::IsTablePrivate() const
{
	return m_name.substr(0, 8) == "private_";
}

}
}
