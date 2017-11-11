#pragma once
#include"pch.h"
#include"wrapper.h"
using namespace Platform;

namespace unmanaged
{
	public ref class cliwrapper sealed
	{
	public:
		cliwrapper() { }
		Array<int>^ Putcomputer(bool hard, int turn)
		{
			std::tuple<int, int8_t, int8_t> x = ptr->Putcomputer(hard, turn);
			auto res = ref new Array<int>(3);
			res[0] = std::get<0>(x);
			res[1] = std::get<1>(x);
			res[2] = std::get<2>(x);
			return res;
		}
		bool Fullboard()
		{
			return ptr->Fullboard();
		}
		bool BoardUndo()
		{
			return ptr->Undoboard();
		}
		int Checkwin(bool change)
		{
			return ptr->Checkwin(change);
		}
		bool Puthuman(int row, int col, int turn)
		{
			return ptr->Put(row, col, turn);
		}
		Array<int>^ Getchessboard()
		{
			auto res = ref new Array<int>(15 * 15);
			for (int i = 0; i < 15; i++)
				for (int ii = 0; ii < 15; ii++)
					res[i * 15 + ii] = ptr->Getchessboard(static_cast<int8_t>(i), static_cast<int8_t>(ii));
			return res;
		}
		Array<int>^ Gethistory()
		{
			auto x = ptr->Gethistory();
			auto res = ref new Array<int>(x.size() * 3);
			for (size_t i = 0; i < x.size(); i++)
			{
				res[3 * i] = std::get<0>(x[i]);
				res[3 * i + 1] = std::get<1>(x[i]);
				res[3 * i + 2] = std::get<2>(x[i]);
			}
			return res;
		}
	private:
		std::unique_ptr<Cwrapper>ptr = std::make_unique<Cwrapper>();
	};
}
