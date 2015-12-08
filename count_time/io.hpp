#pragma once
#include <string>
#include <memory>
#include <chrono>
class date_io_help_c
{
public:
	date_io_help_c();
	~date_io_help_c();//for pimpl. YOU MUST NOT DELETE THIS!!
	std::string encode(const std::chrono::milliseconds& msec) const noexcept(false);
	std::chrono::milliseconds decode(const std::string& str) const noexcept(false);
	std::chrono::milliseconds read_file(const char* file) const noexcept(false);
	void write_file(const char* file, const std::chrono::milliseconds& msec) const noexcept(false);
private:
	struct Impl;
	std::unique_ptr<Impl> pimpl;
};
using day = std::chrono::duration<std::int32_t, std::ratio<24 * 60 * 60> >;
template<class _Rep, class _Period> std::string to_string(const std::chrono::duration<_Rep, _Period>& time) {
	using std::chrono::duration_cast;
	std::string str;
	str += std::to_string(duration_cast<day>(time));
}
