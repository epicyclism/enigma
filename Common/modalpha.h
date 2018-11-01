#pragma once

#include <limits>
#include <type_traits>
#include <ostream>

#include "modular.h"

enum class alpha : char {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, SZ };
namespace std
{
	// Think is actually legal C++.
	//
	template<> class numeric_limits<alpha> : public numeric_limits<char>
	{
	public:
		_NODISCARD static constexpr char (max)() noexcept
		{	// return maximum value
			return (char(alpha::SZ));
		}
	};
};

constexpr int alpha_max = std::numeric_limits<alpha>::max();
using modalpha = mod_t<alpha_max, alpha>;

constexpr char to_printable(modalpha const& c)
{
	return char(c.Val() + 'A');
}

constexpr modalpha from_printable(char const ch)
{
	return modalpha(ch - 'A');
}

using als_t  = std::array<modalpha, alpha_max>;
using alsp_t = std::pair<als_t, als_t>;

std::ostream& operator<< (std::ostream& ostr, modalpha const& m)
{
	ostr << to_printable(m);
	return ostr;
}
