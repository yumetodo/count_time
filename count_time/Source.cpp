#include <iostream>
#include <array>
#include <chrono>
#include <ratio>
#include <DxLib.h>
#include <string>
#include <thread>
#include "check_digit.hpp"
#include "crc32.hpp"
#include "io.hpp"
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 200;
using day = std::chrono::duration<std::int32_t, std::ratio<24 * 60 * 60> >;
template<class _Rep, class _Period> std::string to_string(const std::chrono::duration<_Rep, _Period>& time, const std::array<const char*, 4>& strs = {"日", "時間", "分", "秒"} ) {
	using namespace std::chrono;
	auto t = time;
	std::string str;
	const auto d = duration_cast<day>(t).count();
	str += std::to_string(d) + strs[0];
	if (1 <= d) t -= day(d);
	const auto h = duration_cast<hours>(t).count();
	str += std::to_string(h) + strs[1];
	if (1 <= h) t -= hours(h);
	const auto m = duration_cast<minutes>(t).count();
	str += std::to_string(m) + strs[2];
	if (1 <= m) t -= minutes(m);
	const auto s = duration_cast<seconds>(t).count();
	str += std::to_string(s) + strs[3];
	if (1 <= s) t -= seconds(s);
	str += std::to_string(duration_cast<milliseconds>(t).count());
	return str;
}

std::chrono::microseconds decide_begin_time_d(const date_io_help_c& data_io_helper, const std::chrono::time_point<std::chrono::steady_clock>& current) {
	using std::chrono::duration_cast;
	std::chrono::microseconds begin;
	try {
		begin = data_io_helper.read_file("state.txt");
		if (day(1) <= duration_cast<day>(begin)) begin = std::chrono::milliseconds();//24時間経過していたらリセット
	}
	catch (const std::exception&) {
		begin = std::chrono::milliseconds();
	}
	return begin;
}
int init() {
	SetMainWindowText("std::chronoをつかった簡単なタイマー");
	SetOutApplicationLogValidFlag(FALSE);
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
	ChangeWindowMode(TRUE);
	SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1) return -1;// エラーが起きたら直ちに終了
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	if (-1 == init()) return -1;
	const int font_h = CreateFontToHandle(nullptr, 30, 1, DX_FONTTYPE_ANTIALIASING);//「Zキーを押してね」の奴
	date_io_help_c data_io_helper;
	const auto begin_t = std::chrono::steady_clock::now();
	const auto begin_d = decide_begin_time_d(data_io_helper, begin_t);
	auto normal_con_f = []() -> bool {
		return -1 != ProcessMessage() && 0 == ScreenFlip() && 0 == ClearDrawScreen();
	};
	DxLib::CheckHitKey(KEY_INPUT_ESCAPE);
	while (!DxLib::CheckHitKey(KEY_INPUT_ESCAPE) && normal_con_f()) {
		const auto l_begin_t = std::chrono::steady_clock::now();
		const auto d = l_begin_t - begin_t + begin_d;
		const auto d_str = to_string(d);
		DrawStringToHandle(0, 0, d_str.c_str(), GetColor(0, 0, 0), font_h);
		//std::this_thread::sleep_until(l_begin_t + std::chrono::seconds(1));
	}
	data_io_helper.write_file("state.txt", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin_t + begin_d));
	DxLib::DxLib_End();
	return 0;
}
