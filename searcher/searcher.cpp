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

#include "pch.h"
#include "searcher.h"
#include "searcher.h"
#include"chessboard.h"
using namespace std;

std::vector<std::tuple<int, int8_t, int8_t>> searcher::smart_genmove(int8_t turn, chessboard& board, int8_t depth, int8_t current) noexcept
{
	int maxnum = 16;
	auto moves = board.genmove();
	if (current <= 2)
		return moves;
	const int alpha = -0x7fffffff, beta = 0x7fffffff;
	std::vector<std::tuple<int, int8_t, int8_t>> ress;
	decltype(ress) realretval;
	ress.reserve(1 + moves.size());
	bool checkneg2 = false;
	bool checkneg1 = false;
	bool check1 = false; // Searched normal results.
	bool check2 = false; // Searched very good result.
	bool check3 = false; // Searched ultra good result.
	bool check4 = false; // Win signal.
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		int nturn;
		if (turn == 1)
			nturn = 2;
		else
			nturn = 1;
		auto temp = min_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), 0);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (!checkneg2&&std::get<0>(temp) >= -9000)
			checkneg2 = true;
		if (!checkneg1&&std::get<0>(temp) >= -2000)
			checkneg1 = true;
		if (!check1&&std::get<0>(temp) >= 0)
			check1 = true;
		if (!check2&&std::get<0>(temp) >= 9000)
			check2 = true;
		if (!check3&&std::get<0>(temp) >= 9970)
			check3 = true;
		if (!check4&&std::get<0>(temp) >= 50000)
			check4 = true;
		if (checkneg2&&std::get<0>(temp) <= -9900)
		{
			;
		}
		else if (checkneg1&&std::get<0>(temp) <= -9000)
		{
			;
		}
		else if (check1&&std::get<0>(temp) <= -2000)
		{
			;
		}
		else if (check2&&std::get<0>(temp) < 2000)
		{
			;
		}
		else if (check3&&std::get<0>(temp) <= 9900)
		{
			;
		}
		else if (check4&&std::get<0>(temp) <= 15000)
		{
			;
		}
		else
			ress.emplace_back(com);
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
	}
	if (checkneg2 || checkneg1 || check1 || check2 || check3 || check4)
	{
		if (check4)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) >= 15000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
		else if (check3)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > 9900)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
		else if (check2)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) >= 2000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
		else if (check1)// check1
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > -2000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
		else if (checkneg1)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > -9000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
		else if (checkneg2)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > -9900)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const size_t sizer = realretval.size();
			if (sizer > static_cast<size_t>(maxnum))
				realretval.resize(maxnum);
			return realretval;
		}
	}
	else
	{
		sort(ress.rbegin(), ress.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
		const size_t sizer = ress.size();
		if (sizer > static_cast<size_t>(maxnum))
			ress.resize(maxnum);
	}
	return ress;
}

std::tuple<int, int8_t, int8_t> searcher::alpha_beta_search(int8_t turn, chessboard board, int8_t depth, int timeout)noexcept
{
	timeoutnum = timeout;
	start = clock.now();
	//evaluator.evaluate(ref(board), turn, -1, -1, true);
	return max_value(turn, ref(board), -0x7fffffff, 0x7fffffff, depth, -1, -1, 0);
}

std::tuple<int, int8_t, int8_t> searcher::max_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply)noexcept
{
	bool changed = false;
	/*std::chrono::time_point<std::chrono::steady_clock> timeout;
	if (depth == 8)
	timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutnum);*/
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = evaluator.evaluate(ref(board), turn, i, ii, false);
	//int checker = board.checkpoint(i, ii);
	if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (depth <= 0)
	{
		//res = evaluator.evaluate(ref(board), turn, i, ii, false);
		return std::make_tuple(res, i, ii);
	}
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves = smart_genmove(turn, ref(board), 1, depth);
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		std::tuple<int, int8_t, int8_t> temp;
		if (depth == 7)
		{
			if (changed == false)
			{
				v = std::make_tuple(-0x7fffffff, std::get<1>(x), std::get<2>(x));
				changed = true;
			}
			//auto the_answer = std::async(std::launch::async, &searcher::min_value, this, nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
			//while (true)
			//{
			//	if (the_answer.wait_until(timeout) == std::future_status::timeout)
			//		return v;
			//	else if (the_answer.wait_until(timeout) == std::future_status::ready)
			//	{
			//		temp = the_answer.get();
			//		break;
			//	}
			//}
		}
		temp = min_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (std::get<0>(v) < std::get<0>(com))
			v = com;
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) >= beta)
			return v;
		alpha = max(alpha, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(clock.now() - start).count() >= timeoutnum)
			break;
	}
	return v;
}

std::tuple<int, int8_t, int8_t> searcher::min_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply)noexcept
{
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
	//int checker = board.checkpoint(i, ii);
	if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (depth <= 0)
	{
		//res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
		return std::make_tuple(res, i, ii);
	}
	auto moves = smart_genmove(turn, ref(board), 1, depth);
	std::tuple<int, int, int> v = std::make_tuple(0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		auto temp = max_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (get<0>(v) > get<0>(com))
			v = com;
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) <= alpha)
			return v;
		beta = min(beta, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(clock.now() - start).count() >= timeoutnum)
			break;
	}
	return v;
}