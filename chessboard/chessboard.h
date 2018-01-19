﻿/*
   Copyright 2017-2018 Chijun Sima

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

class chessboard
{
	friend class evaluation;
public:
	DllExport chessboard() noexcept;
	DllExport void reset() noexcept;
	DllExport int8_t get(const int8_t row, const int8_t col) const noexcept;
	DllExport bool put(const int8_t row, const int8_t col, const int8_t x) noexcept;
	DllExport bool Fullboard() const noexcept;
	DllExport int8_t undo(const int8_t row, const int8_t col) noexcept;
	DllExport int8_t checkpoint(const int8_t row, const int8_t col) const noexcept;
	DllExport std::vector<std::tuple<int, int8_t, int8_t>> genmove() const noexcept;
	DllExport uint8_t getNumber()const noexcept
	{
		return number;
	}
protected:
	std::vector<std::array<int8_t, 15>> board = std::vector<std::array<int8_t, 15>>(15);
	std::vector<std::array<std::array<uint8_t, 15>, 29>> layer_2 = std::vector<std::array<std::array<uint8_t, 15>, 29>>(4);
	std::vector<std::array<std::array<uint8_t, 15>, 15>> layer_3 = std::vector<std::array<std::array<uint8_t, 15>, 15>>(4); //[direction][row][col]
	std::vector<std::pair<uint8_t, uint8_t>>layer_5 = std::vector<std::pair<uint8_t, uint8_t>>(225);

private:
	int check() const noexcept;
	void update_remotecell(const int8_t row, const int8_t col, const bool add) noexcept;
	void update_layer_2(const int8_t row, const int8_t col, const bool add) noexcept;
	std::vector<std::array<uint8_t, 15>> remote_cell = std::vector<std::array<uint8_t, 15>>(15);
	uint8_t number;
	std::vector<std::array<uint8_t, 15>> pos = std::vector<std::array<uint8_t, 15>>(15);
	std::vector<std::pair<int8_t, int8_t>>score_cache;
};
