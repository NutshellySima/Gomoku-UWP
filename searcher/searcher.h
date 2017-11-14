//Copyright(C) 2017  Chijun Sima
//
//This program is free software : you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see <http://www.gnu.org/licenses/>.

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
