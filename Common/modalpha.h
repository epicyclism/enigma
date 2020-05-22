#pragma once

#include <limits>
#include <type_traits>
#include <ostream>
#include <array>

#include "modular.h"

enum class alpha : char {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, SZ };
namespace std
{
	// Think is actually legal C++.
	//
	template<> class numeric_limits<alpha> : public numeric_limits<char>
	{
	public:
		[[nodiscard]]  static constexpr char (max)() noexcept
		{	// return maximum value
			return (char(alpha::SZ));
		}
	};
};

constexpr int alpha_max = std::numeric_limits<alpha>::max();
using modalpha = mod_t<alpha_max, alpha>;

CONSTEXPR inline char to_printable(modalpha const& c)
{
	if (c.Val() == alpha_max)
		return ' ';
	return char(c.Val() + 'A');
}

CONSTEXPR inline char to_printable_lower(modalpha const& c)
{
	if (c.Val() == alpha_max)
		return ' ';
	return char(c.Val() + 'a');
}

CONSTEXPR inline modalpha from_printable(char const ch)
{
	if (ch == ' ' || ch == 0 )
		return modalpha(alpha::SZ); // invalid character can be ignored later. Lets us use five letter groups for example.
	if (ch == '=')
		return modalpha(alpha::X); // missing character, replace with X for now
	return modalpha(ch - 'A');
}

CONSTEXPR inline bool valid_from_char(char const ch)
{
	return (ch >= 'A' && ch <= 'Z') || ch == '=';
}

using als_t  = std::array<modalpha, alpha_max>;
using alsp_t = std::pair<als_t, als_t>;

inline std::ostream& operator<< (std::ostream& ostr, modalpha const& m)
{
	ostr << to_printable(m);
	return ostr;
}

//                                              A           B           C           D           E           F           G           H
CONSTEXPR unsigned modalpha_bits[] = { 0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
//                                              I           J           K           L           M           N           O           P 
									   0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
//                                              Q           R           S           T           U           V           W           X  
                                       0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
//                                              Y           Z
									   0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };
CONSTEXPR inline unsigned modalpha_bit(modalpha m)
{
	return modalpha_bits[m.Val()];
}

CONSTEXPR inline bool modalpha_is_bit(modalpha m, unsigned bits)
{
	return bits & modalpha_bits[m.Val()];
}

CONSTEXPR inline unsigned modalpha_set_bit(modalpha m, unsigned bits)
{
	return bits | modalpha_bits[m.Val()];
}

