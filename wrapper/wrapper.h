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
#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport
#include"pch.h"
#include"chessboard.h"
#include"searcher.h"

using std::make_tuple;


class Cwrapper {
public:
	DllExport Cwrapper(void);
	DllExport int8_t Getchessboard(int8_t row, int8_t col)
	{
		return chessboarddata[row][col];
	}
	DllExport std::vector<std::tuple<int8_t, int8_t, int8_t>> Gethistory()
	{
		return history;
	}
	DllExport void Resetboard()
	{
		board.reset();
		for (int i = 0; i < 15; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
				chessboarddata[i][ii] = 0;
		}
	}
	DllExport bool Fullboard()
	{
		return board.Fullboard();
	}
	DllExport bool Undoboard()
	{
		if (history.size() >= 1)
		{
			board.undo(std::get<1>(history.back()), std::get<2>(history.back()));
			chessboarddata[std::get<1>(history.back())][std::get<2>(history.back())] = 0;
			history.pop_back();
			return true;
		}
		return false;
	}
	DllExport std::tuple<int, int8_t, int8_t> Putcomputer(bool hard, int turn)
	{
		std::tuple<int, int8_t, int8_t> y;
		if (hard)
			y = searchman.alpha_beta_search(static_cast<int8_t>(turn), board, static_cast<int8_t>(7), 5000);
		else
			y = searchman.alpha_beta_search(static_cast<int8_t>(turn), board, static_cast<int8_t>(6), 5000);
		Put(std::get<1>(y), std::get<2>(y), static_cast<int8_t>(turn));
		return y;
	}
	DllExport bool Put(int row, int col, int8_t turn)
	{
		const bool valid = board.put(row, col, turn);
		if (valid)
		{
			chessboarddata[row][col] = turn;
			history.push_back(std::make_tuple(turn, row, col));
		}
		return valid;
	}
	DllExport bool IsFree(int8_t i, int8_t ii)
	{
		return board.get(i, ii) == 0;
	}
	DllExport int Checkwin(bool checker)noexcept
	{
		std::vector<std::tuple<int8_t, int8_t, int8_t>>changer;
		int flag = 0;
		if (!checker)
		{
			for (int i = 0; i < 15; ++i)
			{
				for (int ii = 0; ii < 15; ++ii)
				{
					if (chessboarddata[i][ii] == 3)
						chessboarddata[i][ii] = 1;
					if (chessboarddata[i][ii] == 4)
						chessboarddata[i][ii] = 2;
				}
			}
		}
		int counto = 0, countx = 0;
		for (int i = 0; i < 15; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				if (chessboarddata[i][ii] == 0)
				{
					countx = counto = 0;
				}
				if (chessboarddata[i][ii] == 1)
				{
					++counto;
					countx = 0;
				}
				if (chessboarddata[i][ii] == 2)
				{
					++countx;
					counto = 0;
				}
				if (counto == 5)
				{
					if (checker)
					{
						for (int c = ii; c > ii - 5; --c)
							changer.push_back(make_tuple(3, i, c));
						for (int c = ii; c < 15; ++c)
						{
							if (chessboarddata[i][c] == 1)
								changer.push_back(make_tuple(3, i, c));
							else
								break;
						}
					}
					flag = 1;
				}
				if (countx == 5)
				{
					if (checker)
					{
						for (int c = ii; c > ii - 5; --c)
							changer.push_back(make_tuple(4, i, c));
						for (int c = ii; c < 15; ++c)
						{
							if (chessboarddata[i][c] == 2)
								changer.push_back(make_tuple(4, i, c));
							else
								break;
						}
					}
					flag = 2;
				}
			}
			counto = countx = 0;
		}
		for (int i = 0; i < 15; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				if (chessboarddata[ii][i] == 0)
				{
					countx = counto = 0;
				}
				if (chessboarddata[ii][i] == 1)
				{
					++counto;
					countx = 0;
				}
				if (chessboarddata[ii][i] == 2)
				{
					++countx;
					counto = 0;
				}
				if (counto == 5)
				{
					if (checker)
					{
						for (int c = ii; c > ii - 5; --c)
							changer.push_back(make_tuple(3, c, i));
						for (int c = ii; c < 15; ++c)
						{
							if (chessboarddata[c][i] == 1)
								changer.push_back(make_tuple(3, c, i));
							else
								break;
						}
					}
					flag = 1;
				}
				if (countx == 5)
				{
					if (checker)
					{
						for (int c = ii; c > ii - 5; --c)
							changer.push_back(make_tuple(4, c, i));
						for (int c = ii; c < 15; ++c)
						{
							if (chessboarddata[c][i] == 2)
								changer.push_back(make_tuple(4, c, i));
							else
								break;
						}
					}
					flag = 2;
				}
			}
			counto = countx = 0;
		}
		for (int i = -10; i <= 10; ++i)
		{
			int x = i, y = 0;
			while (x < 0)
				++x, ++y;
			for (; x < 15 && y < 15; ++x, ++y)
			{
				if (chessboarddata[x][y] == 0)
				{
					countx = counto = 0;
				}
				if (chessboarddata[x][y] == 1)
				{
					++counto;
					countx = 0;
				}
				if (chessboarddata[x][y] == 2)
				{
					++countx;
					counto = 0;
				}
				if (counto == 5)
				{
					if (checker)
					{
						for (int xx = x, yy = y; xx > x - 5; --xx, --yy)
							changer.push_back(make_tuple(3, xx, yy));
						for (int xx = x, yy = y; xx < 15 && yy < 15; ++xx, ++yy)
						{
							if (chessboarddata[xx][yy] == 1)
								changer.push_back(make_tuple(3, xx, yy));
							else
								break;
						}
					}
					flag = 1;
				}
				if (countx == 5)
				{
					if (checker)
					{
						for (int xx = x, yy = y; xx > x - 5; --xx, --yy)
							changer.push_back(make_tuple(4, xx, yy));
						for (int xx = x, yy = y; xx < 15 && yy < 15; ++xx, ++yy)
						{
							if (chessboarddata[xx][yy] == 2)
								changer.push_back(make_tuple(4, xx, yy));
							else
								break;
						}
					}
					flag = 2;
				}
			}
			counto = countx = 0;
		}
		for (int i = 4; i <= 24; ++i)
		{
			int x = 0;
			int y = i;
			while (y >= 15)
			{
				++x;
				--y;
			}
			for (; x < 15 && y >= 0; ++x, --y)
			{
				if (chessboarddata[x][y] == 0)
				{
					countx = counto = 0;
				}
				if (chessboarddata[x][y] == 1)
				{
					++counto;
					countx = 0;
				}
				if (chessboarddata[x][y] == 2)
				{
					++countx;
					counto = 0;
				}
				if (counto == 5)
				{
					if (checker)
					{
						for (int xx = x, yy = y; xx > x - 5; --xx, ++yy)
							changer.push_back(make_tuple(3, xx, yy));
						for (int xx = x, yy = y; xx < 15 && y >= 0; ++xx, --yy)
						{
							if (chessboarddata[xx][yy] == 1)
								changer.push_back(make_tuple(3, xx, yy));
							else
								break;
						}
					}
					flag = 1;
				}
				if (countx == 5)
				{
					if (checker)
					{
						for (int xx = x, yy = y; xx > x - 5; --xx, ++yy)
							changer.push_back(make_tuple(4, xx, yy));
						for (int xx = x, yy = y; xx < 15 && y >= 0; ++xx, --yy)
						{
							if (chessboarddata[xx][yy] == 2)
								changer.push_back(make_tuple(4, xx, yy));
							else
								break;
						}
					}
					flag = 2;
				}
			}
			counto = countx = 0;
		}
		for (const auto& x : changer)
			chessboarddata[std::get<1>(x)][std::get<2>(x)] = std::get<0>(x);
		return flag;
	}
private:
	chessboard board;
	searcher searchman;
	std::array<std::array<int8_t, 15>, 15>chessboarddata;
	std::vector<std::tuple<int8_t, int8_t, int8_t>>history;
	int _Checkwin(const int row, const int col)
	{
		return board.checkpoint(row, col);
	}
};