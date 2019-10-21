#pragma once

#include "ISqlValue.h"

namespace repo
{

class IRow
{
public:
	virtual ~IRow() {}

	virtual SqlString Data(const std::string& columnName) const = 0;
	virtual SqlDouble GetReal(const std::string& columnName) const = 0;
	virtual SqlInteger GetInteger(const std::string& columnName) const = 0;
	virtual SqlBigInt GetBigInt(const std::string& columnName) const = 0;
};

}

