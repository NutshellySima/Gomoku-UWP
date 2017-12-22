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
	std::array<std::pair<int8_t, int8_t>, 15 * 15>score_cache;
};
