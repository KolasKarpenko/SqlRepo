#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <iterator>
#include <random>
#include <memory>
#include <functional>
#include <type_traits>
#include <assert.h>

#ifdef _WIN32
#include <objbase.h>
#elif defined(__linux__) || defined(__unix__)
#include <uuid/uuid.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CFUUID.h>
#endif

namespace uuids
{
namespace detail
{
template <typename TChar>
inline unsigned char hex2char(TChar const ch)
{
	if (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9'))
		return ch - static_cast<TChar>('0');
	if (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f'))
		return 10 + ch - static_cast<TChar>('a');
	if (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'))
		return 10 + ch - static_cast<TChar>('A');
	return 0;
}

template <typename TChar>
inline bool is_hex(TChar const ch)
{
	return
		(ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9')) ||
		(ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f')) ||
		(ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'));
}

template <typename TChar>
inline unsigned char hexpair2char(TChar const a, TChar const b)
{
	return (hex2char(a) << 4) | hex2char(b);
}

}

struct uuid
{
public:
template<typename ForwardIterator>
uuid(ForwardIterator first, ForwardIterator last) : data({})
{
	if (std::distance(first, last) == 16)
		std::copy(first, last, std::begin(data));
}

uuid() : data({}) { }

bool is_nil() const
{
	for (size_t i = 0; i < data.size(); ++i) 
		if (data[i] != 0) 
			return false;

	return true;
}

void swap(uuid & other) 
{
	data.swap(other.data);
}

template <typename TChar>
static uuid from_string(TChar const * const str, size_t const size)
{
	if (str == nullptr || size < 36 || size > 38) {
		return uuid();
	}

	TChar digit = 0;
	bool firstDigit = true;
	int hasBraces = 0;
	size_t index = 0;
	std::array<uint8_t, 16> data{ { 0 } };

	if (str[0] == static_cast<TChar>('{'))
		hasBraces = 1;

	if (hasBraces && str[size - 1] != static_cast<TChar>('}'))
		return uuid();

	for (size_t i = hasBraces; i < size - hasBraces; ++i) {
		if (str[i] == static_cast<TChar>('-')) continue;

		if (index >= 16 || !detail::is_hex(str[i])) {
			return uuid();
		}

		if (firstDigit) {
			digit = str[i];
			firstDigit = false;
		}
		else {
			data[index++] = detail::hexpair2char(digit, str[i]);
			firstDigit = true;
		}
	}

	if (index < 16) {
		return uuid();
	}

	return uuid{ std::cbegin(data), std::cend(data) };
}

static uuid from_string(const std::string& str) {
	return from_string(str.data(), str.size());
}

std::string to_string() const {
	std::stringstream s;

	s << std::hex << std::setfill('0');
	s  << std::setw(2) << (int)data[0];
	s  << std::setw(2) << (int)data[1];
	s  << std::setw(2) << (int)data[2];
	s  << std::setw(2) << (int)data[3];
	s  << '-';
	s  << std::setw(2) << (int)data[4];
	s  << std::setw(2) << (int)data[5];
	s  << '-';
	s  << std::setw(2) << (int)data[6];
	s  << std::setw(2) << (int)data[7];
	s  << '-';
	s  << std::setw(2) << (int)data[8];
	s  << std::setw(2) << (int)data[9];
	s  << '-';
	s  << std::setw(2) << (int)data[10];
	s  << std::setw(2) << (int)data[11];
	s  << std::setw(2) << (int)data[12];
	s  << std::setw(2) << (int)data[13];
	s  << std::setw(2) << (int)data[14];
	s  << std::setw(2) << (int)data[15];

	return s.str();
}

friend uuid operator+(uuid lhs, const uuid& rhs) {
	for (int i = 0; i < 16; ++i) {
		lhs.data[i] += rhs.data[i];
	}
	return lhs;
}

bool operator==(const uuid & rhs) {
	return data == rhs.data;
}

bool operator!= (const uuid& rhs) {
	return !(*this == rhs);
}

bool operator<(const uuid & rhs) {
	return data < rhs.data;
}

friend bool operator<(const uuid& lhs, const uuid& rhs) {
	return lhs.data < rhs.data;
}

private:
	std::array<uint8_t, 16> data;
};

class uuid_system_generator
{
public:
	using result_type = uuid;
	
	uuid operator()()
	{
#ifdef _WIN32
		GUID newId;
		HRESULT res = ::CoCreateGuid(&newId);

		if (res != S_OK) {
			return uuid{};
		}
		
		std::array<uint8_t, 16> bytes =
		{
			{
				(unsigned char)((newId.Data1 >> 24) & 0xFF),
				(unsigned char)((newId.Data1 >> 16) & 0xFF),
				(unsigned char)((newId.Data1 >> 8) & 0xFF),
				(unsigned char)((newId.Data1) & 0xFF),

				(unsigned char)((newId.Data2 >> 8) & 0xFF),
				(unsigned char)((newId.Data2) & 0xFF),

				(unsigned char)((newId.Data3 >> 8) & 0xFF),
				(unsigned char)((newId.Data3) & 0xFF),

				newId.Data4[0],
				newId.Data4[1],
				newId.Data4[2],
				newId.Data4[3],
				newId.Data4[4],
				newId.Data4[5],
				newId.Data4[6],
				newId.Data4[7]
			}
		};

		return uuid(std::begin(bytes), std::end(bytes));

#elif defined(__linux__) || defined(__unix__)
		uuid_t id;
		uuid_generate(id);

		std::array<uint8_t, 16> bytes =
		{
			{
				id[0],
				id[1],
				id[2],
				id[3],
				id[4],
				id[5],
				id[6],
				id[7],
				id[8],
				id[9],
				id[10],
				id[11],
				id[12],
				id[13],
				id[14],
				id[15]
			}
		};

		return uuid{ std::begin(bytes), std::end(bytes) };

#elif defined(__APPLE__)
		auto newId = CFUUIDCreate(nullptr);
		auto bytes = CFUUIDGetUUIDBytes(newId);
		CFRelease(newId);
		
		std::array<uint8_t, 16> arrbytes =
		{
			{
				bytes.byte0,
				bytes.byte1,
				bytes.byte2,
				bytes.byte3,
				bytes.byte4,
				bytes.byte5,
				bytes.byte6,
				bytes.byte7,
				bytes.byte8,
				bytes.byte9,
				bytes.byte10,
				bytes.byte11,
				bytes.byte12,
				bytes.byte13,
				bytes.byte14,
				bytes.byte15
			}
		};
		
		return uuid{ std::begin(arrbytes), std::end(arrbytes) };
#elif
		return uuid{};
#endif
}

};

static uuid create()
{
	uuid_system_generator gen;
	return gen();
}
}

namespace std
{

template <>
struct hash<uuids::uuid>
{
	using argument_type = uuids::uuid;
	using result_type   = std::size_t;
	
	result_type operator()(argument_type const &uuid) const
	{
		std::hash<std::string> hasher;
		return static_cast<result_type>(hasher(uuid.to_string()));
	}
};

}
