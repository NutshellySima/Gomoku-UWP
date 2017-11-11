#pragma once
#include"pch.h"
#include"evaluation.h"
#include"chessboard.h"

#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport

class searcher
{
public:
	DllExport std::tuple<int, int8_t, int8_t> alpha_beta_search(int8_t turn, chessboard board, int8_t depth, int timeout)noexcept;
private:
	evaluation evaluator;
	std::vector<std::tuple<int, int8_t, int8_t>> smart_genmove(int8_t turn, chessboard& board, int8_t depth, int8_t current)noexcept;
	std::tuple<int, int8_t, int8_t> max_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply)noexcept;
	std::tuple<int, int8_t, int8_t> min_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply)noexcept;
	int timeoutnum = 30000;
	std::chrono::steady_clock clock;
	std::chrono::time_point<std::chrono::steady_clock> start;
};
