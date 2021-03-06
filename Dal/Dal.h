/*
This file is auto-generated
*/

#pragma once

#include <IRepoObject.h>
#include <IRow.h>
#include <NewObjectPatch.h>
#include <DeleteObjectPatch.h>
#include <TransactionPatch.h>

namespace repo
{

namespace ProductSchema
{

class private_Journal
{
public:
static const char* TableName;

struct Field
{
	static const char* id;
	static const char* time;
	static const char* schemaVersion;
	static const char* contextId;
	static const char* patchJson;
};
private_Journal(const repo::IRow& row);
int32_t Get_id() const;
std::string Get_time() const;
repo::SqlBigInt Get_schemaVersion() const;
repo::SqlString Get_contextId() const;
repo::SqlString Get_patchJson() const;
Json::Value ToJson() const;
private:
repo::SqlInteger m_id;
repo::SqlString m_time;
repo::SqlBigInt m_schemaVersion;
repo::SqlString m_contextId;
repo::SqlString m_patchJson;
};

class private_ChildToParentNames
{
public:
static const char* TableName;

struct Field
{
	static const char* child;
	static const char* parent;
};
private_ChildToParentNames(const repo::IRow& row);
std::string Get_child() const;
repo::SqlString Get_parent() const;
Json::Value ToJson() const;
private:
repo::SqlString m_child;
repo::SqlString m_parent;
};

class Product : public repo::IRepoObject
{
public:
static const char* TableName;

struct Field
{
	static const char* id;
	static const char* name;
	static const char* description;
};
Product(const repo::IRow& row);
Product();
std::string Get_id() const;
std::string Get_name() const;
void Set_name(const std::string& value);
repo::SqlString Get_description() const;
void Set_description(const repo::SqlString& value);
virtual void Save(repo::TransactionPatch& transaction) const override;
virtual void Delete(repo::TransactionPatch& transaction) const override;
virtual Json::Value ToJson() const override;
std::shared_ptr<repo::EditObjectPatch> GetPatch() const;
virtual void FromJson(const Json::Value& json) override;
Product Copy() const;
Product DeepCopy(repo::ISession& s) const;
private:
mutable std::shared_ptr<repo::EditObjectPatch> m_patch;
mutable std::shared_ptr<repo::TransactionPatch> m_transaction;
repo::SqlString m_id;
repo::SqlString m_name;
repo::SqlString m_description;
};

class Inspection : public repo::IRepoObject
{
public:
static const char* TableName;
static const char* ParentTableName;

struct Field
{
	static const char* id;
	static const char* parentId;
	static const char* name;
};
Inspection(const repo::IRow& row);
Inspection();
std::string Get_id() const;
std::string Get_parentId() const;
void Set_parentId(const std::string& value);
std::string Get_name() const;
void Set_name(const std::string& value);
virtual void Save(repo::TransactionPatch& transaction) const override;
virtual void Delete(repo::TransactionPatch& transaction) const override;
virtual Json::Value ToJson() const override;
std::shared_ptr<repo::EditObjectPatch> GetPatch() const;
virtual void FromJson(const Json::Value& json) override;
Inspection Copy() const;
Inspection DeepCopy(repo::ISession& s) const;
private:
mutable std::shared_ptr<repo::EditObjectPatch> m_patch;
mutable std::shared_ptr<repo::TransactionPatch> m_transaction;
repo::SqlString m_id;
repo::SqlString m_parentId;
repo::SqlString m_name;
};

class Region : public repo::IRepoObject
{
public:
static const char* TableName;
static const char* ParentTableName;

struct Field
{
	static const char* id;
	static const char* parentId;
	static const char* name;
};
Region(const repo::IRow& row);
Region();
std::string Get_id() const;
std::string Get_parentId() const;
void Set_parentId(const std::string& value);
std::string Get_name() const;
void Set_name(const std::string& value);
virtual void Save(repo::TransactionPatch& transaction) const override;
virtual void Delete(repo::TransactionPatch& transaction) const override;
virtual Json::Value ToJson() const override;
std::shared_ptr<repo::EditObjectPatch> GetPatch() const;
virtual void FromJson(const Json::Value& json) override;
Region Copy() const;
Region DeepCopy(repo::ISession& s) const;
private:
mutable std::shared_ptr<repo::EditObjectPatch> m_patch;
mutable std::shared_ptr<repo::TransactionPatch> m_transaction;
repo::SqlString m_id;
repo::SqlString m_parentId;
repo::SqlString m_name;
};

} //namespace ProductSchema
} //namespace repo
