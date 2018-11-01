#pragma once

// wiring definitions for Enigma rotors.
// the wiring relates the nominal input to output for each wheel,
// this is specified right to left (as fitted to the machine) in terms
// of letters, we need this converted to values we can do arithmetic with
// and return through the wheel from left to right. So this is computed in a
// a constexpr function at compile time.
//
#pragma warning ( disable : 4514)

#include <array>
#include <algorithm>
#include "const_helpers.h"
#include "modalpha.h"

struct wiring
{
	als_t rl_ ;
	als_t lr_ ;

	constexpr wiring() noexcept : rl_{}, lr_{}
	{}
	constexpr wiring (wiring const& w) : rl_{}, lr_{}
	{
		// seriously????
		epicyclism::ctransform(std::begin(w.rl_), std::end(w.rl_), std::begin(rl_), [](auto m) { return m; });
		epicyclism::ctransform(std::begin(w.lr_), std::end(w.lr_), std::begin(lr_), [](auto m) { return m; });
	}
	constexpr modalpha evalRL(modalpha val) const
	{
		return rl_[val.Val()];
	}
	constexpr modalpha evalLR(modalpha val) const
	{
		return lr_[val.Val()];
	}
	constexpr modalpha evalRef(modalpha val) const
	{
		return lr_[val.Val()] + val;
	}
	template<typename O> modalpha evalRef(modalpha val, O& ostr) const
	{
		auto r = lr_[val.Val()] + val;
		ostr << "<>" << r ;
		return r;
	}
};

constexpr wiring FromDescWheel(char const desc[26])
{
	struct pr
	{
		modalpha r_;
		modalpha l_;
		constexpr pr() noexcept {}
	};
	std::array<pr, 26> wking{};
	modalpha l{ alpha::A };
	int n = 0;

	for (auto& w : wking)
	{
		w.r_ = l;
		w.l_ = from_printable(desc[n]);
		++l;
		++n;
	}

	// render forwards
	wiring rv{};
	epicyclism::ctransform(std::begin(wking), std::end(wking), std::begin(rv.rl_), [](auto lr)
	{
		return lr.l_ - lr.r_;
	});
	// sort...
	epicyclism::qsort(std::begin(wking), std::end(wking), [](auto l, auto r) { return l.l_ < r.l_; });
	// render backwards
	epicyclism::ctransform(std::begin(wking), std::end(wking), std::begin(rv.lr_), [](auto lr)
	{
		return lr.r_ - lr.l_;
	});

	return rv;
}

constexpr wiring FromDescReflector(char const desc[26])
{
	// load forwards, which is A->desc[0], B->desc[1]...Z->desc[25]
	struct pr
	{
		modalpha i_;
		modalpha o_;
		constexpr pr() noexcept {}
	};
	std::array<pr, 26> wking{};
	modalpha l{ alpha::A };
	int n = 0;

	for (auto& w : wking)
	{
		w.i_ = l;
		w.o_ = from_printable(desc[n]);
		++l;
		++n;
	}

	// render 
	wiring rv{};
	epicyclism::ctransform(std::begin(wking), std::end(wking), std::begin(rv.rl_), [](auto lr)
	{
		return lr.o_ - lr.i_;
	});
	// duplicate
	epicyclism::ctransform(std::begin(wking), std::end(wking), std::begin(rv.lr_), [](auto lr)
	{
		return lr.o_ - lr.i_;
	});

	return rv;
}

// reflector wiring
//
constexpr wiring     B = FromDescReflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
constexpr wiring     C = FromDescReflector("FVPJIAOYEDRZXWGCTKUQSBNMHL");
constexpr wiring  B_M4 = FromDescReflector("ENKQAUYWJICOPBLMDXZVFTHRGS");
constexpr wiring  C_M4 = FromDescReflector("RDOBJNTKVEHMLFCWZAXGYIPSUQ");
