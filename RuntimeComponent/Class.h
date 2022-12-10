#pragma once

#include "Class.g.h"
#include "wrapper.h"
#include <vector>

namespace winrt::RuntimeComponent::implementation
{
    struct Class : ClassT<Class>
    {
        Class() = default;

		winrt::com_array<int> Putcomputer(bool hard, int turn)
		{
			std::tuple<int, int8_t, int8_t> x = ptr->Putcomputer(hard, turn);
			auto res = std::vector<int>(3);
			res[0] = std::get<0>(x);
			res[1] = std::get<1>(x);
			res[2] = std::get<2>(x);
			return winrt::com_array<int>(res);
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
		winrt::com_array<int> Getchessboard()
		{
			auto res = std::vector<int>(15 * 15);
			for (int i = 0; i < 15; i++)
				for (int ii = 0; ii < 15; ii++)
					res[i * 15 + ii] = ptr->Getchessboard(static_cast<int8_t>(i), static_cast<int8_t>(ii));
			return winrt::com_array<int>(res);
		}
		winrt::com_array<int> Gethistory()
		{
			auto x = ptr->Gethistory();
			auto res = std::vector<int>(static_cast<unsigned int>(x.size()) * 3);
			for (unsigned int i = 0; i < x.size(); i++)
			{
				res[3 * i] = std::get<0>(x[i]);
				res[3 * i + 1] = std::get<1>(x[i]);
				res[3 * i + 2] = std::get<2>(x[i]);
			}
			return winrt::com_array<int>(res);
		}
	private:
		std::unique_ptr<Cwrapper>ptr = std::make_unique<Cwrapper>();
        
    };
}

namespace winrt::RuntimeComponent::factory_implementation
{
    struct Class : ClassT<Class, implementation::Class>
    {
    };
}
