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

constexpr inline char to_printable(modalpha const& c)
{
	if (c == alpha::SZ)
		return ' ';
	return char(c.Val() + 'A');
}

constexpr inline char to_printable_lower(modalpha const& c)
{
	if (c == alpha::SZ)
		return ' ';
	return char(c.Val() + 'a');
}

constexpr inline modalpha from_printable(char const ch)
{
	if (ch == ' ' || ch == 0 || ch == '=')
		return modalpha(alpha::SZ); // invalid character can be ignored later. Lets us use five letter groups for example.
//	if (ch == '=')
//		return modalpha(alpha::X); // missing character, replace with X for now
	return modalpha(ch - 'A');
}

constexpr inline bool valid_from_char(char const ch)
{
	return ch >= 'A' && ch <= 'Z';
}

using als_t  = std::array<modalpha, alpha_max>;
using alsp_t = std::pair<als_t, als_t>;

inline std::ostream& operator<< (std::ostream& ostr, modalpha const& m)
{
	ostr << to_printable(m);
	return ostr;
}
