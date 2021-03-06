/*
This file is auto-generated
*/

#include "Dal.h"
#include <Tools.h>
#include <uuid.h>
namespace repo

{
namespace ProductSchema
{

const char* private_Journal::TableName = "private_Journal";
const char* private_Journal::Field::id = "id";
const char* private_Journal::Field::time = "time";
const char* private_Journal::Field::schemaVersion = "schemaVersion";
const char* private_Journal::Field::contextId = "contextId";
const char* private_Journal::Field::patchJson = "patchJson";
private_Journal::private_Journal(const repo::IRow& row) :
	m_id(row.GetInteger(Field::id).Data())
	, m_time(row.Data(Field::time).Data())
	, m_schemaVersion(row.GetBigInt(Field::schemaVersion))
	, m_contextId(row.Data(Field::contextId))
	, m_patchJson(row.Data(Field::patchJson))
{
}
int32_t private_Journal::Get_id() const { return m_id.Data(); }
std::string private_Journal::Get_time() const { return m_time.Data(); }
repo::SqlBigInt private_Journal::Get_schemaVersion() const { return m_schemaVersion; }
repo::SqlString private_Journal::Get_contextId() const { return m_contextId; }
repo::SqlString private_Journal::Get_patchJson() const { return m_patchJson; }
Json::Value private_Journal::ToJson() const
{
	Json::Value obj;
	if (m_id.IsNull()) {
		obj["id"] = Json::Value();
	} else {
		obj["id"] = m_id.Data();
	}
	if (m_time.IsNull()) {
		obj["time"] = Json::Value();
	} else {
		obj["time"] = m_time.Data();
	}
	if (m_schemaVersion.IsNull()) {
		obj["schemaVersion"] = Json::Value();
	} else {
		obj["schemaVersion"] = m_schemaVersion.Data();
	}
	if (m_contextId.IsNull()) {
		obj["contextId"] = Json::Value();
	} else {
		obj["contextId"] = m_contextId.Data();
	}
	if (m_patchJson.IsNull()) {
		obj["patchJson"] = Json::Value();
	} else {
		obj["patchJson"] = m_patchJson.Data();
	}
	return obj;
}

const char* private_ChildToParentNames::TableName = "private_ChildToParentNames";
const char* private_ChildToParentNames::Field::child = "child";
const char* private_ChildToParentNames::Field::parent = "parent";
private_ChildToParentNames::private_ChildToParentNames(const repo::IRow& row) :
	m_child(row.Data(Field::child).Data())
	, m_parent(row.Data(Field::parent))
{
}
std::string private_ChildToParentNames::Get_child() const { return m_child.Data(); }
repo::SqlString private_ChildToParentNames::Get_parent() const { return m_parent; }
Json::Value private_ChildToParentNames::ToJson() const
{
	Json::Value obj;
	if (m_child.IsNull()) {
		obj["child"] = Json::Value();
	} else {
		obj["child"] = m_child.Data();
	}
	if (m_parent.IsNull()) {
		obj["parent"] = Json::Value();
	} else {
		obj["parent"] = m_parent.Data();
	}
	return obj;
}

const char* Product::TableName = "Product";
const char* Product::Field::id = "id";
const char* Product::Field::name = "name";
const char* Product::Field::description = "description";
Product::Product(const repo::IRow& row) :
	m_id(row.Data(Field::id).Data())
	, m_name(row.Data(Field::name).Data())
	, m_description(row.Data(Field::description))
{
	m_patch.reset(new repo::EditObjectPatch(TableName, m_id.Data()));
}

Product::Product()
{
	m_patch.reset(new repo::NewObjectPatch(TableName));
	m_id = m_patch->GetObjectId();
}
std::string Product::Get_id() const { return m_id.Data(); }
std::string Product::Get_name() const { return m_name.Data(); }
void Product::Set_name(const std::string& value){
	m_name = value;
	m_patch->SetValue("name", m_name);
}
repo::SqlString Product::Get_description() const { return m_description; }
void Product::Set_description(const repo::SqlString& value){
	m_description = value;
	m_patch->SetValue("description", m_description);
}
void Product::Save(repo::TransactionPatch& transaction) const
{
	transaction.AddPatch(*m_patch);
	m_patch->Clear();
	if (m_transaction) {
		transaction.AddPatch(*m_transaction);
		m_transaction = nullptr;
	}
}
void Product::Delete(repo::TransactionPatch& transaction) const
{
	repo::DeleteObjectPatch del(TableName, m_id.Data());
	transaction.AddPatch(del);
}
Json::Value Product::ToJson() const
{
	Json::Value obj;
	if (m_id.IsNull()) {
		obj["id"] = Json::Value();
	} else {
		obj["id"] = m_id.Data();
	}
	if (m_name.IsNull()) {
		obj["name"] = Json::Value();
	} else {
		obj["name"] = m_name.Data();
	}
	if (m_description.IsNull()) {
		obj["description"] = Json::Value();
	} else {
		obj["description"] = m_description.Data();
	}
	return obj;
}
std::shared_ptr<repo::EditObjectPatch> Product::GetPatch() const
{
	return m_patch;
}
void Product::FromJson(const Json::Value& json)
{
	if (json.isMember("id")) {
		const Json::Value& id = json["id"];
		if (id.isString()){
			m_id = id.asString();
			m_patch->ResetId(m_id.Data());
		}
	}
	if (json.isMember("name")) {
		this->Set_name(json["name"].asString());
	}
	if (json.isMember("description")) {
		if (json["description"].isNull()) {
			this->Set_description(nullptr);
		} else {
			this->Set_description(json["description"].asString());
		}
	}
}
Product Product::Copy() const {
	Product copy;
	copy.Set_name(Get_name());
	copy.Set_description(Get_description());
	return copy;
}
Product Product::DeepCopy(repo::ISession& session) const
{
	std::map<std::string, repo::IRepoObjectPtr> copyObjects;
	std::map<std::string, Json::Value> copyObjectsJson;
	std::map<std::string, std::vector<std::string>> parentIdMap;
	std::shared_ptr<repo::TransactionPatch> transaction(new repo::TransactionPatch);
	std::shared_ptr<Product> copyPtr(new Product());
	copyObjectsJson.insert(std::make_pair(Get_id(), ToJson()));
	copyPtr->m_transaction = transaction;
	parentIdMap["Product"] = {Get_id()};
	copyObjects.insert(std::make_pair(Get_id(), copyPtr));
	{
	std::vector<std::string> newParentIdList;
	for(const auto& parentId : parentIdMap["Product"]) {
		std::stringstream selectInspection;
		selectInspection << "select * from Inspection where parentId = " << Tools::GetTextValue(parentId);
		session.ExecSql(selectInspection.str().c_str(), [&](const repo::IRow& row) {
			Inspection obj(row);
			copyObjectsJson.insert(std::make_pair(obj.Get_id(), obj.ToJson()));
			copyObjects.insert(std::make_pair(obj.Get_id(), new Inspection()));
			newParentIdList.push_back(obj.Get_id());
		});
	}
	parentIdMap["Inspection"] = newParentIdList;
	}
	{
	std::vector<std::string> newParentIdList;
	for(const auto& parentId : parentIdMap["Inspection"]) {
		std::stringstream selectRegion;
		selectRegion << "select * from Region where parentId = " << Tools::GetTextValue(parentId);
		session.ExecSql(selectRegion.str().c_str(), [&](const repo::IRow& row) {
			Region obj(row);
			copyObjectsJson.insert(std::make_pair(obj.Get_id(), obj.ToJson()));
			copyObjects.insert(std::make_pair(obj.Get_id(), new Region()));
			newParentIdList.push_back(obj.Get_id());
		});
	}
	parentIdMap["Region"] = newParentIdList;
	}
	uuids::uuid incrementUuid = uuids::create();
	std::set<uuids::uuid> allIds;
	for (const auto& copyObj : copyObjects) {
		allIds.insert(uuids::uuid::from_string(copyObj.first));
	}
	for (auto& copyObj : copyObjects) {
		Json::Value& asJson = copyObjectsJson[copyObj.first];
		Tools::IncrementUuids(asJson, incrementUuid, allIds);
		copyObj.second->FromJson(asJson);
		if (copyObj.first != Get_id())
			copyObj.second->Save(*transaction);
	}
	return *copyPtr;
}

const char* Inspection::TableName = "Inspection";
const char* Inspection::ParentTableName = "Product";
const char* Inspection::Field::id = "id";
const char* Inspection::Field::parentId = "parentId";
const char* Inspection::Field::name = "name";
Inspection::Inspection(const repo::IRow& row) :
	m_id(row.Data(Field::id).Data())
	, m_parentId(row.Data(Field::parentId).Data())
	, m_name(row.Data(Field::name).Data())
{
	m_patch.reset(new repo::EditObjectPatch(TableName, m_id.Data()));
}

Inspection::Inspection()
{
	m_patch.reset(new repo::NewObjectPatch(TableName));
	m_id = m_patch->GetObjectId();
}
std::string Inspection::Get_id() const { return m_id.Data(); }
std::string Inspection::Get_parentId() const { return m_parentId.Data(); }
void Inspection::Set_parentId(const std::string& value){
	m_parentId = value;
	m_patch->SetValue("parentId", m_parentId);
}
std::string Inspection::Get_name() const { return m_name.Data(); }
void Inspection::Set_name(const std::string& value){
	m_name = value;
	m_patch->SetValue("name", m_name);
}
void Inspection::Save(repo::TransactionPatch& transaction) const
{
	transaction.AddPatch(*m_patch);
	m_patch->Clear();
	if (m_transaction) {
		transaction.AddPatch(*m_transaction);
		m_transaction = nullptr;
	}
}
void Inspection::Delete(repo::TransactionPatch& transaction) const
{
	repo::DeleteObjectPatch del(TableName, m_id.Data());
	transaction.AddPatch(del);
}
Json::Value Inspection::ToJson() const
{
	Json::Value obj;
	if (m_id.IsNull()) {
		obj["id"] = Json::Value();
	} else {
		obj["id"] = m_id.Data();
	}
	if (m_parentId.IsNull()) {
		obj["parentId"] = Json::Value();
	} else {
		obj["parentId"] = m_parentId.Data();
	}
	if (m_name.IsNull()) {
		obj["name"] = Json::Value();
	} else {
		obj["name"] = m_name.Data();
	}
	return obj;
}
std::shared_ptr<repo::EditObjectPatch> Inspection::GetPatch() const
{
	return m_patch;
}
void Inspection::FromJson(const Json::Value& json)
{
	if (json.isMember("id")) {
		const Json::Value& id = json["id"];
		if (id.isString()){
			m_id = id.asString();
			m_patch->ResetId(m_id.Data());
		}
	}
	if (json.isMember("parentId")) {
		this->Set_parentId(json["parentId"].asString());
	}
	if (json.isMember("name")) {
		this->Set_name(json["name"].asString());
	}
}
Inspection Inspection::Copy() const {
	Inspection copy;
	copy.Set_parentId(Get_parentId());
	copy.Set_name(Get_name());
	return copy;
}
Inspection Inspection::DeepCopy(repo::ISession& session) const
{
	std::map<std::string, repo::IRepoObjectPtr> copyObjects;
	std::map<std::string, Json::Value> copyObjectsJson;
	std::map<std::string, std::vector<std::string>> parentIdMap;
	std::shared_ptr<repo::TransactionPatch> transaction(new repo::TransactionPatch);
	std::shared_ptr<Inspection> copyPtr(new Inspection());
	copyObjectsJson.insert(std::make_pair(Get_id(), ToJson()));
	copyPtr->m_transaction = transaction;
	parentIdMap["Inspection"] = {Get_id()};
	copyObjects.insert(std::make_pair(Get_id(), copyPtr));
	{
	std::vector<std::string> newParentIdList;
	for(const auto& parentId : parentIdMap["Inspection"]) {
		std::stringstream selectRegion;
		selectRegion << "select * from Region where parentId = " << Tools::GetTextValue(parentId);
		session.ExecSql(selectRegion.str().c_str(), [&](const repo::IRow& row) {
			Region obj(row);
			copyObjectsJson.insert(std::make_pair(obj.Get_id(), obj.ToJson()));
			copyObjects.insert(std::make_pair(obj.Get_id(), new Region()));
			newParentIdList.push_back(obj.Get_id());
		});
	}
	parentIdMap["Region"] = newParentIdList;
	}
	uuids::uuid incrementUuid = uuids::create();
	std::set<uuids::uuid> allIds;
	for (const auto& copyObj : copyObjects) {
		allIds.insert(uuids::uuid::from_string(copyObj.first));
	}
	for (auto& copyObj : copyObjects) {
		Json::Value& asJson = copyObjectsJson[copyObj.first];
		Tools::IncrementUuids(asJson, incrementUuid, allIds);
		copyObj.second->FromJson(asJson);
		if (copyObj.first != Get_id())
			copyObj.second->Save(*transaction);
	}
	return *copyPtr;
}

const char* Region::TableName = "Region";
const char* Region::ParentTableName = "Inspection";
const char* Region::Field::id = "id";
const char* Region::Field::parentId = "parentId";
const char* Region::Field::name = "name";
Region::Region(const repo::IRow& row) :
	m_id(row.Data(Field::id).Data())
	, m_parentId(row.Data(Field::parentId).Data())
	, m_name(row.Data(Field::name).Data())
{
	m_patch.reset(new repo::EditObjectPatch(TableName, m_id.Data()));
}

Region::Region()
{
	m_patch.reset(new repo::NewObjectPatch(TableName));
	m_id = m_patch->GetObjectId();
}
std::string Region::Get_id() const { return m_id.Data(); }
std::string Region::Get_parentId() const { return m_parentId.Data(); }
void Region::Set_parentId(const std::string& value){
	m_parentId = value;
	m_patch->SetValue("parentId", m_parentId);
}
std::string Region::Get_name() const { return m_name.Data(); }
void Region::Set_name(const std::string& value){
	m_name = value;
	m_patch->SetValue("name", m_name);
}
void Region::Save(repo::TransactionPatch& transaction) const
{
	transaction.AddPatch(*m_patch);
	m_patch->Clear();
	if (m_transaction) {
		transaction.AddPatch(*m_transaction);
		m_transaction = nullptr;
	}
}
void Region::Delete(repo::TransactionPatch& transaction) const
{
	repo::DeleteObjectPatch del(TableName, m_id.Data());
	transaction.AddPatch(del);
}
Json::Value Region::ToJson() const
{
	Json::Value obj;
	if (m_id.IsNull()) {
		obj["id"] = Json::Value();
	} else {
		obj["id"] = m_id.Data();
	}
	if (m_parentId.IsNull()) {
		obj["parentId"] = Json::Value();
	} else {
		obj["parentId"] = m_parentId.Data();
	}
	if (m_name.IsNull()) {
		obj["name"] = Json::Value();
	} else {
		obj["name"] = m_name.Data();
	}
	return obj;
}
std::shared_ptr<repo::EditObjectPatch> Region::GetPatch() const
{
	return m_patch;
}
void Region::FromJson(const Json::Value& json)
{
	if (json.isMember("id")) {
		const Json::Value& id = json["id"];
		if (id.isString()){
			m_id = id.asString();
			m_patch->ResetId(m_id.Data());
		}
	}
	if (json.isMember("parentId")) {
		this->Set_parentId(json["parentId"].asString());
	}
	if (json.isMember("name")) {
		this->Set_name(json["name"].asString());
	}
}
Region Region::Copy() const {
	Region copy;
	copy.Set_parentId(Get_parentId());
	copy.Set_name(Get_name());
	return copy;
}
Region Region::DeepCopy(repo::ISession& session) const
{
	std::map<std::string, repo::IRepoObjectPtr> copyObjects;
	std::map<std::string, Json::Value> copyObjectsJson;
	std::map<std::string, std::vector<std::string>> parentIdMap;
	std::shared_ptr<repo::TransactionPatch> transaction(new repo::TransactionPatch);
	std::shared_ptr<Region> copyPtr(new Region());
	copyObjectsJson.insert(std::make_pair(Get_id(), ToJson()));
	copyPtr->m_transaction = transaction;
	parentIdMap["Region"] = {Get_id()};
	copyObjects.insert(std::make_pair(Get_id(), copyPtr));
	uuids::uuid incrementUuid = uuids::create();
	std::set<uuids::uuid> allIds;
	for (const auto& copyObj : copyObjects) {
		allIds.insert(uuids::uuid::from_string(copyObj.first));
	}
	for (auto& copyObj : copyObjects) {
		Json::Value& asJson = copyObjectsJson[copyObj.first];
		Tools::IncrementUuids(asJson, incrementUuid, allIds);
		copyObj.second->FromJson(asJson);
		if (copyObj.first != Get_id())
			copyObj.second->Save(*transaction);
	}
	return *copyPtr;
}

} //namespace ProductSchema
} //namespace repo
