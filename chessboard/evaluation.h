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
#include "pch.h"
#include "chessboard.h"
#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport

class evaluation
{
public:
	DllExport evaluation() noexcept {}
	DllExport int evaluate(chessboard &, const int turn, const int row, const int col, bool pure) noexcept;

private:
	void reset_point(chessboard &board, int row, int col, bool pure) noexcept;
	void analyse_line(const std::array<uint8_t, 15> &line, int num, const int pos) noexcept;
	void analysis_horizon(chessboard &, int i, int j) noexcept;
	void analysis_vertical(chessboard &, int i, int j) noexcept;
	void analysis_left(chessboard &, int i, int j) noexcept;
	void analysis_right(chessboard &, int i, int j) noexcept;
	inline int checkturn(const int &side, const int &turn) const noexcept
	{
		if (side == turn)
			return 1;
		return -1;
	}
	inline int sign(int number) const noexcept
	{
		if (number < 0)
			return -1;
		return 1;
	}
	int __evaluate(chessboard &, int turn, const int row, const int col, bool pure) noexcept;
	void evaluate_point(chessboard &board, int row, int col) noexcept;
	const uint8_t STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7,
		  ANALYSED = 255, TODO = 0;
	const uint8_t BLACK = 1, WHITE = 2;
	const std::vector<uint8_t> nturn = { 0, 2, 1 };
	std::vector<uint8_t> result = std::vector<uint8_t>(15);
	uint8_t flag;
	std::vector<uint8_t>copy_place = std::vector<uint8_t>(30);
};
