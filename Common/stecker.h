#pragma once

#include "mod26.h"

class stecker
{
private:
	als_t board_;

public:
	constexpr stecker()
	{
		Clear();
	}
	constexpr void Clear()
	{
		for (auto& p : board_)
			p = alpha::A;
	}
	constexpr void Set(modalpha from, modalpha to)
	{
		board_[from.Val()] = to - from;
		board_[to.Val()] = from - to;
	}
	constexpr modalpha Eval(modalpha from) const
	{
		return from + board_[from.Val()];
	}
	template<typename O> constexpr modalpha Eval(modalpha from, O& ostr) const
	{
		auto r = Eval(from);
		ostr << "->" << r << " ";
		return r;
	}
	template<typename O> constexpr void Report(O& ostr) const
	{
		int n = 0;
		for (auto const& s : board_)
		{
			if (s > 0)
			{
				ostr << modalpha(n) << modalpha(n + s) << " ";
			}
			++n;
		}
	}
};