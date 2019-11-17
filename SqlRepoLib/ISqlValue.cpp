#include "ISqlValue.h"

#include "Tools.h"

namespace repo
{

std::function<std::string(const char*)> SqlString::ms_convertFromData = [](const char* data) { return std::string(data); };
std::function<double(const char*)> SqlDouble::ms_convertFromData = [](const char* data) { return std::stod(data); };
std::function<int(const char*)> SqlInteger::ms_convertFromData = [](const char* data) { return std::stoi(data); };
std::function<int64_t(const char*)> SqlBigInt::ms_convertFromData = [](const char* data) { return std::stoll(data); };

std::function<std::string(const std::string&)> SqlString::ms_convertToSqlValue = [](const std::string& data) { return Tools::GetTextValue(data); };
std::function<std::string(const double&)> SqlDouble::ms_convertToSqlValue = [](const double& data) { return std::to_string(data); };
std::function<std::string(const int32_t&)> SqlInteger::ms_convertToSqlValue = [](const int32_t& data) { return std::to_string(data); };
std::function<std::string(const int64_t&)> SqlBigInt::ms_convertToSqlValue = [](const int64_t& data) { return std::to_string(data); };

}