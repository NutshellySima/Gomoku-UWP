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

#include "pch.h"
#include "searcher.h"
#include "chessboard.h"
#include <shared_mutex>
#include <future>
using namespace std;

// fixed 1
shared_mutex alphaval_mutex;
// fixed 2
shared_mutex trueval_mutex;
// fixed 3
shared_mutex betaval_mutex;

std::vector<std::tuple<int, int8_t, int8_t>> searcher::smart_genmove(const int8_t turn, chessboard& board, const int8_t current, evaluation& evaluator)
{
	auto moves = board.genmove();
	if (current <= 1)
		return moves;
	std::vector<std::tuple<int, int8_t, int8_t>> ress;
	ress.reserve(1 + moves.size());
	bool checkneg2 = false;
	bool checkneg1 = false;
	bool check1 = false; // Searched normal results.
	bool check2 = false; // Searched very good result.
	bool check3 = false; // Searched ultra good result.
	bool check4 = false; // Win signal.
	if (current == 2)
	{
		for (auto&x : moves)
		{
			board.put(std::get<1>(x), std::get<2>(x), turn);
			int nturn;
			if (turn == 1)
				nturn = 2;
			else
				nturn = 1;
			std::tuple<int, int8_t, int8_t> temp;
			evaluator.evaluate(ref(board), nturn, std::get<1>(x), std::get<2>(x), true);
			int res = 0 - evaluator.evaluate(ref(board), nturn, std::get<1>(x), std::get<2>(x), false);
			if (res == -100000)
				temp = std::make_tuple(0 - 10000000, std::get<1>(x), std::get<2>(x));
			else if (res == 100000)
				temp = std::make_tuple(10000000, std::get<1>(x), std::get<2>(x));
			else
				temp = std::make_tuple(res, std::get<1>(x), std::get<2>(x));
			auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
			ress.emplace_back(com);
			board.undo(std::get<1>(x), std::get<2>(x));
			evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		}
		sort(ress.rbegin(), ress.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
		return ress;
	}
	decltype(ress) realretval;
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		int nturn;
		if (turn == 1)
			nturn = 2;
		else
			nturn = 1;
		std::tuple<int, int8_t, int8_t> temp;
		evaluator.evaluate(ref(board), nturn, std::get<1>(x), std::get<2>(x), true);
		int res = 0 - evaluator.evaluate(ref(board), nturn, std::get<1>(x), std::get<2>(x), false);
		if (res == -100000)
			temp = std::make_tuple(0 - 10000000, std::get<1>(x), std::get<2>(x));
		else if (res == 100000)
			temp = std::make_tuple(10000000, std::get<1>(x), std::get<2>(x));
		else
			temp = std::make_tuple(res, std::get<1>(x), std::get<2>(x));
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
		board.undo(std::get<1>(x), std::get<2>(x));
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

std::tuple<int, int8_t, int8_t> searcher::alpha_beta_search(int8_t turn, chessboard board, int8_t depth, int timeout)
{
	if (board.getNumber() == 0)
	{
		return std::make_tuple(0, 'H' - 'A', 'H' - 'A');
	}
	if (depth >= 7)
		maxnum = 16;
	else
		maxnum = 32;
	timeoutnum = timeout;
	start = chrono::steady_clock::now();
	this->search_depth = depth;
	this->alphaval = -0x7fffffff;
	trueval = std::make_tuple(-0x7fffffff, -1, -1);
	evaluation evaluator;
	evaluator.evaluate(ref(board), turn, -1, -1, true);
	return max_value_first(turn, ref(board), 0x7fffffff, depth, -1, -1, 0, ref(evaluator));
}

std::tuple<int, int8_t, int8_t> searcher::max_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator)
{
	bool changed = false;
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (depth <= 0 || board.Fullboard())
	{
		return std::make_tuple(res, i, ii);
	}
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		std::tuple<int, int8_t, int8_t> temp;
		if (depth == this->search_depth)
		{
			if (changed == false)
			{
				v = std::make_tuple(-0x7fffffff, std::get<1>(x), std::get<2>(x));
				changed = true;
			}
		}
		temp = min_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, ref(evaluator));
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (std::get<0>(v) < std::get<0>(com))
			v = com;
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) >= beta)
			return v;
		alpha = max(alpha, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() >= timeoutnum)
			break;
	}
	return v;
}

std::tuple<int, int8_t, int8_t> searcher::min_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator)
{
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (depth <= 0 || board.Fullboard())
	{
		return std::make_tuple(res, i, ii);
	}
	auto moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	std::tuple<int, int, int> v = std::make_tuple(0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		auto temp = max_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, ref(evaluator));
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (get<0>(v) > get<0>(com))
			v = com;
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) <= alpha)
			return v;
		beta = min(beta, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() >= timeoutnum)
			break;
	}
	return v;
}

std::tuple<int, int8_t, int8_t> searcher::max_value_first(int8_t turn, chessboard& board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator)
{
	bool changed = false;
	bool first = false;
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (depth <= 0 || board.Fullboard())
	{
		return std::make_tuple(res, i, ii);
	}
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	vector<future<void>>futures;
	moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	trueval = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		if (changed == false)
		{
			trueval = std::make_tuple(-0x7fffffff, std::get<1>(x), std::get<2>(x));
			changed = true;
			std::async(std::launch::async, &searcher::min_value_second, this, nturn, board, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, evaluator).wait();
		}
		if (!first)
			futures.emplace_back(std::async(std::launch::async, &searcher::min_value_first, this, nturn, board, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, evaluator));
		else
			first = true;
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
	}
	for (auto&x : futures)
		x.wait();
	return trueval;
}

void searcher::min_value_first(int8_t turn, chessboard board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator)
{
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == -100000)
	{
		write_val(std::make_tuple(0 - 10000000 + ply, i, ii));
		return;
	}
	else if (res == 100000)
	{
		write_val(std::make_tuple(10000000 - ply, i, ii));
		return;
	}
	else if (depth <= 0 || board.Fullboard())
	{
		write_val(std::make_tuple(res, i, ii));
		return;
	}
	auto moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	std::tuple<int, int8_t, int8_t> v = std::make_tuple(0x7fffffff, i, ii);
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		int alpha = getAlphaVal();
		auto temp = max_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, ref(evaluator));
		auto com = make_tuple(std::get<0>(temp), i, ii);
		if (get<0>(v) > get<0>(com))
			v = com;
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) <= getAlphaVal())
		{
			return;
		}
		beta = min(beta, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() >= timeoutnum)
			break;
	}
	write_val(v);
	return;
}

void searcher::min_value_second(int8_t turn, chessboard board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator)
{
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == -100000)
	{
		write_val(std::make_tuple(0 - 10000000 + ply, i, ii));
		return;
	}
	else if (res == 100000)
	{
		write_val(std::make_tuple(10000000 - ply, i, ii));
		return;
	}
	else if (depth <= 0 || board.Fullboard())
	{
		write_val(std::make_tuple(res, i, ii));
		return;
	}
	auto moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	std::tuple<int, int8_t, int8_t> v = std::make_tuple(0x7fffffff, i, ii);
	vector<future<std::tuple<int, int8_t, int8_t>>>futures;
	vector<std::tuple<int, int8_t, int8_t>>result;
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		int alpha = getAlphaVal();
		futures.emplace_back(std::async(std::launch::async, &searcher::max_value_second, this, nturn, board, alpha, ref(beta), depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, evaluator));
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() >= timeoutnum)
			break;
	}
	for (auto&x : futures)
		x.wait();
	for (auto&x : futures)
		result.emplace_back(make_tuple(std::get<0>(x.get()), i, ii));
	v = *min_element(result.begin(), result.end(), [](const auto& x1, const auto& x2) {return get<0>(x1) < get<0>(x2); });
	write_val(v);
	return;
}

std::tuple<int, int8_t, int8_t> searcher::max_value_second(int8_t turn, chessboard board, int alpha, int& beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator)
{
	auto betageter = [&beta]()
	{
		std::shared_lock<std::shared_mutex> lock(betaval_mutex);
		return beta;
	};
	auto betachanger = [&beta, &betageter](const std::tuple<int, int8_t, int8_t>&val) {
		// Change betaval to min
		if (std::get<0>(val) < betageter())
		{
			betaval_mutex.lock();
			if (std::get<0>(val) < beta)
				beta = std::get<0>(val);
			betaval_mutex.unlock();
		}
	};
	bool changed = false;
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = evaluator.evaluate(ref(board), turn, i, ii, false);
	if (res == 100000)
	{
		betachanger(std::make_tuple(10000000 - ply, i, ii));
		return std::make_tuple(10000000 - ply, i, ii);
	}
	else if (res == -100000)
	{
		betachanger(std::make_tuple(0 - 10000000 + ply, i, ii));
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	}
	else if (depth <= 0 || board.Fullboard())
	{
		betachanger(std::make_tuple(res, i, ii));
		return std::make_tuple(res, i, ii);
	}
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves = smart_genmove(turn, ref(board), depth, ref(evaluator));
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(std::get<1>(x), std::get<2>(x), turn);
		std::tuple<int, int8_t, int8_t> temp;
		if (depth == this->search_depth)
		{
			if (changed == false)
			{
				v = std::make_tuple(-0x7fffffff, std::get<1>(x), std::get<2>(x));
				changed = true;
			}
		}
		temp = min_value(nturn, ref(board), alpha, betageter(), depth - 1, std::get<1>(x), std::get<2>(x), ply + 1, ref(evaluator));
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (std::get<0>(v) < std::get<0>(com))
			v = com;
		board.undo(std::get<1>(x), std::get<2>(x));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) >= betageter())
		{
			return v;
		}
		alpha = max(alpha, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() >= timeoutnum)
			break;
	}
	betachanger(v);
	return v;
}

void searcher::write_val(const std::tuple<int, int8_t, int8_t>& val)
{
	std::tuple<int, int8_t, int8_t> temp;
	// Change trueval
	if (getTrueVal(ref(temp)); std::get<0>(temp) < std::get<0>(val))
	{
		trueval_mutex.lock();
		if (std::get<0>(trueval) < std::get<0>(val))
			trueval = val;
		trueval_mutex.unlock();
	}
	// Change alphaval to max
	if (getTrueVal(ref(temp)); std::get<0>(temp) > getAlphaVal())
	{
		alphaval_mutex.lock();
		if (getTrueVal(ref(temp)); std::get<0>(temp) > alphaval)
		{
			alphaval = std::get<0>(temp);
		}
		alphaval_mutex.unlock();
	}
}

void searcher::getTrueVal(std::tuple<int, int8_t, int8_t>& temp)
{
	std::shared_lock<std::shared_mutex> lock(trueval_mutex);
	temp = trueval;
}
int searcher::getAlphaVal()
{
	std::shared_lock<std::shared_mutex> lock(alphaval_mutex);
	return alphaval;
}