#include "check_digit.hpp"
#include <utility>
#include <numeric>
#include <stdexcept>
#include <cctype>
uint8_t calc_check_digit(const std::string& n) noexcept(false) {
	if (11 != n.size()) throw std::runtime_error("n.digit must be 11");
	const uint8_t r = std::accumulate(n.rbegin(), n.rend(), std::pair<int, int>{}, [](const auto& s, const char& e) -> std::pair<int, int> {
		if (!std::isdigit(e)) throw std::runtime_error("n.digit must be 11");
		return{ s.first + (e - '0') * ((5 < s.second) ? s.second - 4 : s.second + 2), s.second + 1 };
	}).first % 11;
	return (0 == r || 1 == r) ? 0 : 11 - r;
}
