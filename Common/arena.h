#pragma once

#include <array>
#include "wheelset.h"

//using arena_t = std::array<std::array<modalpha, 1024>, alpha_max>;

// maintain alpha_max rows of width W, each row is the output of encoding continuous letters in the machine.
// position recorded to ensure can go back without having to separate out the stepping process.
//
template<size_t W> struct arena_base
{
	static const size_t Width = W;
	// position of each column
	std::array<position, W> pos_;
	// array of columns.
	std::array<std::array<modalpha, W>, alpha_max> arena_;

	using results_t = std::array<std::array<unsigned, W>, alpha_max>;
};

using arena_t = arena_base<17576>;
//                                                  A  B  C  D  E  F  G  H  I  J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
constexpr std::array<modalpha, alpha_max> alphabet{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };

// should these be members?
//
template<typename A> void fill_arena(wheels3& w, A& a, size_t off)
{
	// for each wheel state we have to go down all 26 arrays
	for (auto i = off; i < A::Width; ++i)
	{
		a.pos_[i] = w.Position();
		w.Step();
		auto col = w.Evaluate(alphabet);
		auto itc = std::begin(col);
		for (auto ita = std::begin(a.arena_); ita != std::end(a.arena_); ++ita, ++itc)
		{
			(*ita)[i] = *itc;
		}
	}
}

template<typename A> void shuffle_down_arena(A& a, size_t left)
{
	std::copy(std::begin(a.pos_) + left, std::end(a.pos_), std::begin(a.pos_));
	for (auto& r : a.arena_)
	{
		std::copy(std::begin(r) + left, std::end(r), std::begin(r));
	}
}

template<typename O> void arena_print_r(arena_t const& a, modalpha l, int cnt, O& ostr)
{
	auto c = std::begin(a.pos_);
	auto r = std::begin(a.arena_[l.Val()]);
	for (int n = 0; n < cnt; ++n)
	{
		ostr << *c << " " << *r << "\n";
		++c;
		++r;
	}
}

