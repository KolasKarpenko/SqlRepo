#pragma once

#include <Patch.h>

namespace repo
{
namespace sqlite
{

class ZeroSchemaPatch : public repo::Patch
{
public:
	ZeroSchemaPatch();
	virtual std::string GetType() const override;
	virtual void Apply(ISession& session) const override;
protected:
	virtual void ToJsonImpl(Json::Value& json) const override;
	virtual void FromJsonImpl(const Json::Value& json) override;
};

}
}

