#pragma once

#include <json/json.h>

namespace repo
{

class ISerializable
{
public:
	virtual ~ISerializable() {}

	virtual Json::Value ToJson() const = 0;
	virtual void FromJson(const Json::Value& json) = 0;
};

typedef std::shared_ptr<ISerializable> ISerializablePtr;

}

