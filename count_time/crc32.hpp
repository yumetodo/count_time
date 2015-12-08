#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <string>
class crc32_generator
{
public:
	template<bool condition, typename T = void> using enable_if_t = typename std::enable_if<condition, T>::type;
	crc32_generator();
	~crc32_generator() = default;
	template<typename Container, enable_if_t< std::is_same<typename Container::value_type, uint8_t>::value , std::nullptr_t> = nullptr>
	uint32_t operator()(const Container& c, uint32_t crc_hint = 0xFFFFFFFF) const noexcept {
		auto re = crc_hint;
		for (uint8_t i : c) re = (re >> 8) ^ table_[i ^ (re & 0xFF)];
		return ~re;
	}
	uint32_t operator()(const std::string& str, uint32_t crc_hint = 0xFFFFFFFF) const noexcept {
		auto re = crc_hint;
		for (auto i : str) re = (re >> 8) ^ table_[static_cast<uint8_t>(i) ^ (re & 0xFF)];
		return ~re;
	}
	uint32_t operator()(const uint8_t* buf, std::size_t buf_size, uint32_t crc_hint = 0xFFFFFFFF) const noexcept;
	static constexpr size_t table_size = 256;
private:
	uint32_t table_[table_size];
};
