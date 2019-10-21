#pragma once

#include "ISession.h"
#include <json/json.h>

namespace repo
{

class Patch
{
public:
	template<class T>
	static std::string GetType() {
		T t;
		return t.GetType();
	}

	virtual std::string GetType() const = 0;

	Json::Value ToJson() const;
	void FromJson(const Json::Value& json);

	virtual bool HasChanges() const;
	const repo::SqlBigInt& GetSchemaVersion() const;

	virtual void ApplyJournal(ISession& session) const;
	virtual void Apply(ISession& session) const = 0;
protected:
	Patch();
	Patch(int64_t schemaVersion);

	virtual void ToJsonImpl(Json::Value&) const = 0;
	virtual void FromJsonImpl(const Json::Value& json) = 0;

private:
	repo::SqlBigInt m_schemaVersion;
};

}