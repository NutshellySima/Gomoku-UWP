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

class DllExport chessboard
{
	friend class evaluation;
public:
	chessboard() noexcept;
	void reset() noexcept;
	int get(const int row, const int col) const noexcept;
	bool put(const int row, const int col, const int x) noexcept;
	bool Fullboard() const noexcept;
	int undo(const int row, const int col) noexcept;
	int checkpoint(const int row, const int col) const noexcept;
	std::vector<std::tuple<int, int8_t, int8_t>> genmove() const noexcept;
protected:
	std::array<std::array<int8_t, 15>, 15> board;
	std::array<std::array<std::array<uint8_t, 15>, 29>, 4> layer_2;
	std::array<std::array<std::array<uint8_t, 15>, 15>, 4> layer_3; //[direction][row][col]
	std::array<std::array<uint8_t, 20>, 3> layer_4;
	std::array<std::pair<uint8_t, uint8_t>, 225>layer_5;

private:
	int check() const noexcept;
	void update_remotecell(const int row, const int col, const bool add) noexcept;
	void update_layer_2(const int row, const int col, const bool add) noexcept;
	std::array<std::array<uint8_t, 15>, 15> remote_cell;
	uint8_t number;
	std::array<std::array<uint8_t, 15>, 15> pos;
};
