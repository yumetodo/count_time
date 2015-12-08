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
