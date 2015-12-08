#include "io.hpp"
#include "crc32.hpp"
#include "check_digit.hpp"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <fstream>

struct date_io_help_c::Impl {
	Impl() : crc32_gen() {}
	crc32_generator crc32_gen;
};
date_io_help_c::~date_io_help_c() = default;//for pimpl. YOU MUST NOT DELETE THIS!!
date_io_help_c::date_io_help_c() : pimpl(new date_io_help_c::Impl()){}

static inline uint8_t calc_check_digit_wrap(const std::string& str) noexcept(false) {
	return calc_check_digit((str.size() < 11) ? str + std::string(11 - str.size(), '0') : str.substr(0, 11));
}
/*
format
xxxx....xxxmncccccccccc
xxxx....xxx	:	time(digit:variable. 1-std::numeloc_limits<std::chrono::milliseconds::rep>::digit)
m			:	mynumber check digit(digit:1)
cccccccccc	:	crc32(digit:10)
*/
std::string date_io_help_c::encode(const std::chrono::milliseconds & msec) const noexcept(false)
{
	auto str = std::to_string(msec.count());
	str += calc_check_digit_wrap(str) + '0';//マイナンバーのチェックデジットを追加
	auto crc32 = std::to_string(this->pimpl->crc32_gen(str));//crc32のhashを追記
	str += std::string(10 - crc32.size(), '0') + crc32;
	return str;//NRVOがはたらくことを「祈ります」
}

std::chrono::milliseconds date_io_help_c::decode(const std::string & str) const noexcept(false)
{
	if (str.size() < 22) throw std::range_error("In function `date_io_help_c::decode`: Str's length is too shrot. require 22 or over.");
	const uint32_t crc32 = [](const std::string& str) {
		std::stringstream ss;
		ss << str;
		uint32_t re;
		ss >> re;
		return re;
	}(str.substr(str.size() - 10));
	auto str_wihtout_crc32 = str.substr(0, str.size() - 10);
	if (crc32 != this->pimpl->crc32_gen(str_wihtout_crc32)) throw std::runtime_error("input string is illegal.");//crc32

	const uint8_t check_digit = str_wihtout_crc32.back() - '0';
	str_wihtout_crc32.pop_back();
	if (crc32 != calc_check_digit_wrap(str_wihtout_crc32)) throw std::runtime_error("input string is illegal.");//check_digit

	const auto time = [](const std::string& str) {
		std::stringstream ss;
		ss << str;
		std::chrono::milliseconds::rep re;
		ss >> re;
		return re;
	}(str_wihtout_crc32);
	return std::chrono::milliseconds(time);
}

std::chrono::milliseconds date_io_help_c::read_file(const char * file) const noexcept(false)
{
	std::ifstream f(file);
	std::string buf;
	f >> buf;
	return this->decode(buf);
}

void date_io_help_c::write_file(const char * file, const std::chrono::milliseconds& msec) const noexcept(false)
{
	std::ofstream f(file);
	f << this->encode(msec) << std::endl;
}
