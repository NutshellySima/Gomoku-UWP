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
#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport

class DllExport evaluation
{
public:
	evaluation() noexcept {};
	int evaluate(chessboard &, const int turn, const int row, const int col, bool pure) noexcept;

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
	const uint8_t STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7, DFOUR = 8,
		FOURT = 9, DTHREE = 10, NOTYPE = 11, ANALYSED = 255, TODO = 0;
	const uint8_t BLACK = 1, WHITE = 2;
	const std::array<uint8_t, 3> nturn = { 0, 2, 1 };
	std::array<uint8_t, 15> result;
	uint8_t flag;
	std::array<uint8_t, 30>copy_place;
};
