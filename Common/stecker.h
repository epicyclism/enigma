#pragma once

#include "modalpha.h"

#if !defined(CONSTEXPR)
#if defined (__NVCC__)
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif
#endif

class stecker
{
private:
	als_t board_;

public:
	CONSTEXPR stecker() noexcept
	{
		Clear();
	}
	CONSTEXPR void Clear() noexcept
	{
		for (auto& p : board_)
			p = alpha::A;
	}
	CONSTEXPR void Clear(modalpha from) noexcept
	{
		modalpha to = Eval(from);
		board_[from.Val()] = alpha::A;
		board_[to.Val()] = alpha::A;
	}
	CONSTEXPR void Set(modalpha from, modalpha to) noexcept
	{
		board_[from.Val()] = to - from;
		board_[to.Val()] = from - to;
	}
	[[nodiscard]] CONSTEXPR modalpha Eval(modalpha from) const noexcept
	{
		return from + board_[from.Val()];
	}
	[[nodiscard]] CONSTEXPR bool Is(modalpha from, modalpha to) const noexcept
	{
		return !(board_[from.Val()] == 0 && board_[to.Val()] == 0);
	}
	[[nodiscard]] auto Count() const noexcept
	{
		return std::count_if(board_.begin(), board_.end(), [](auto& v) { return v != 0; }) / 2;
	}
	void Apply(modalpha from, modalpha to)
	{
		if (Is(from, to))
		{
			Clear(from);
			Clear(to);
		}
		Set(from, to);
	}
	template<typename O> CONSTEXPR modalpha Eval(modalpha from, O& ostr) const
	{
		auto r = Eval(from);
		ostr << "->" << r << " ";
		return r;
	}
	template<typename O> CONSTEXPR void Report(O& ostr) const
	{
		int n = 0;
		bool sp = false;
		for (auto const& s : board_)
		{
			if (s > 0 && n + s > n)
			{
				if (sp)
					ostr << " " << modalpha(n) << modalpha(n + s);
				else
				{
					ostr << modalpha(n) << modalpha(n + s);
					sp = true;
				}
			}
			++n;
		}
	}
};