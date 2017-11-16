/*
   Copyright 2017 Chijun Sima

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
