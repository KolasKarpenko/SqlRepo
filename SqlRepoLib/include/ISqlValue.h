#pragma once

#include <string>
#include <functional>
#include <memory>
#include "SqlTools.h"

namespace repo
{

class ISqlValue
{
public:
	virtual ~ISqlValue() {}
	virtual std::string ToSqlValue() const = 0;
};

template<typename T> 
class Nullable : public ISqlValue
{
public:
	Nullable<T>() : m_isNull(true) {}
	Nullable<T>(const T& val) : m_data(val), m_isNull(false) {}
	Nullable<T>(const nullptr_t& val) : m_isNull(true) {}

	const T& Data() const { return m_data; }
	bool IsNull() const { return m_isNull; }

	virtual std::string ToSqlValue() const override {
		return m_isNull ? "null" : ms_convertToSqlValue(m_data);
	}

	static Nullable<T> FromData(const char* data) {
		return Value(ms_convertFromData(data));
	}

	static Nullable<T> Value(const T& val) {
		Nullable<T> result(val);
		return result;
	}

	static const Nullable<T>& Null() {
		static Nullable<T> null;
		return null;
	}

	bool operator==(Nullable<T> const& rhs) {
		if (m_isNull && rhs.n_isNull) {
			return true;
		}

		if (m_isNull != rhs.n_isNull) {
			return false;
		}

		return m_data == rhs.m_data;
	}

	bool operator!= (Nullable<T> const& rhs) {
		return !(*this == rhs);
	}
private:
	T m_data;
	bool m_isNull;
	static std::function<T(const char*)> ms_convertFromData;
	static std::function<std::string(const T&)> ms_convertToSqlValue;
};

typedef Nullable<std::string> SqlString;
typedef Nullable<double> SqlDouble;
typedef Nullable<int32_t> SqlInteger;
typedef Nullable<int64_t> SqlBigInt;

}