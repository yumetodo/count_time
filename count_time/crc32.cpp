#include "crc32.hpp"
class counter_iterator {
	std::size_t i;
public:
	constexpr counter_iterator() : i(0) { }
	constexpr counter_iterator(std::size_t n) : i(n) { }
	constexpr bool operator == (const counter_iterator & rhs) const { return i == rhs.i; }
	constexpr bool operator != (const counter_iterator & rhs) const { return i != rhs.i; }
	constexpr const std::size_t & operator * () const { return i; }
	counter_iterator & operator ++ () { ++i; return *this; }
};
class nth_loop {
private:
	std::size_t i, countSize;
public:
	constexpr nth_loop(const std::size_t n) : i(0), countSize(n){ }
	constexpr nth_loop(const std::size_t pos, const std::size_t length) : i(pos), countSize(((length)<0) ? (pos) : (pos + length)) {}
	constexpr counter_iterator begin() const { return counter_iterator(i); }
	constexpr counter_iterator end() const { return counter_iterator(countSize); }
};
constexpr nth_loop operator "" _(unsigned long long n) { return nth_loop(static_cast<size_t>(n)); }
constexpr nth_loop rep(std::size_t n){ return nth_loop(n); }
crc32_generator::crc32_generator(){
	constexpr uint32_t poly = 0xEDB88320;
	for (auto i : rep(table_size)) {
		uint32_t u = static_cast<uint32_t>(i);
		for (auto j : 8_) u = (u & 1U) ? u >> 1 : (u >> 1) ^ poly;
		this->table_[i] = u;
	}
}

uint32_t crc32_generator::operator()(const uint8_t * buf, std::size_t buf_size, uint32_t crc_hint) const noexcept
{
	auto re = crc_hint;
	for (auto i : rep(buf_size)) re = (re >> 8) ^ table_[buf[i] ^ (re & 0xFF)];
	return ~re;
}
