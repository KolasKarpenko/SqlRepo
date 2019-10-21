#pragma once

#include <map>

#include "Patch.h"

namespace repo
{

class EditObjectPatch : public repo::Patch
{
public:
	EditObjectPatch(const std::string& tableName, const std::string& id);
	EditObjectPatch(int64_t schemaVersion, const std::string& tableName, const std::string& id);
	EditObjectPatch();

	void SetValue(const std::string& field, const ISqlValue& value);
	void ResetValue(const std::string& field);
	virtual void Clear();
	std::string GetObjectId() const;

	virtual std::string GetType() const override;
	virtual bool HasChanges() const override;
	virtual void Apply(ISession& session) const override;

protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;

	std::string m_tableName;
	std::string m_id;
	std::map<std::string, std::string> m_fieldValues;
};

}