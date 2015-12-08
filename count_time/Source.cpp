#include <iostream>
#include <array>
#include <chrono>
#include <ratio>
#include <DxLib.h>
#include "check_digit.hpp"
#include "crc32.hpp"
#include "io.hpp"
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 200;

std::chrono::microseconds decide_begin_time_d(const date_io_help_c& data_io_helper, const std::chrono::time_point<std::chrono::steady_clock>& current) {
	using std::chrono::duration_cast;
	auto begin = data_io_helper.read_file("state.txt");
	const auto c_begin = current.time_since_epoch();
	if (day(1) <= duration_cast<day>(c_begin - begin)) begin = duration_cast<std::chrono::milliseconds>(c_begin);//24時間経過していたらリセット
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
	using std::chrono::steady_clock;
	if (-1 == init()) return -1;
	const int font_h = CreateFontToHandle(nullptr, 30, 1, DX_FONTTYPE_ANTIALIASING);//「Zキーを押してね」の奴
	date_io_help_c data_io_helper;
	const auto begin_t = steady_clock::now();
	const auto begin_d = decide_begin_time_d(data_io_helper, begin_t);
	auto r = steady_clock::now() - begin_t + begin_d;
	DrawStringToHandle(100 + 5, WINDOW_HEIGHT / 3 + 5, "D...DON'T PUSH!!", GetColor(255, 255, 255), font_h);		//タイトル（影）
	auto normal_con_f = []() -> bool {
		return -1 != ProcessMessage() && 0 == ScreenFlip() && 0 == ClearDrawScreen();
	};
	DxLib::CheckHitKey(KEY_INPUT_ESCAPE);
	while (!DxLib::CheckHitKey(KEY_INPUT_ESCAPE) && normal_con_f()) {
		auto t = steady_clock::now() - begin_t + begin_d;

	}
	return 0;
}
//int main() {
//	crc32_generator gen;
//	//uint8_t arr[3];
//	std::array<uint8_t, 3> arr;
//	auto re = gen(arr);
//}
