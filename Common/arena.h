#pragma once

#include <array>
#include <numeric>
#include "wheelset.h"

// maintain alpha_max rows of width W, each row is the output of encoding continuous letters in the machine.
// position recorded to ensure can go back without having to separate out the stepping process.
//
template<size_t W> struct arena_base
{
	// a result for a line
	using results_t = std::array<unsigned, W>;
	// remember the position for each column
	using position_t = std::array<position, W>;

	static const size_t Width = W;
	// position of each column
	position_t pos_;
	// array of lines.
	using line_t = std::array<modalpha, W>;
	std::array<line_t, alpha_max> arena_;

};

//                                                  A  B  C  D  E  F  G  H  I  J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
constexpr std::array<modalpha, alpha_max> alphabet{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
//                                                     A  B  C  D  E  F  G  H  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z

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

template<typename A, typename O> void arena_print_r(A const& a, modalpha l, int cnt, O& ostr)
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

// some stuff for one dimensional arenas.
//
// search primitive
// slide the ciphertext along the row and count the matches at each position.
//
// the result is added to anything already in the results buffer, allowing
// multiple searches to be accumulated.
//
template<typename I, size_t W> void linear_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
	while (itb != ite)
	{
		*ito += std::transform_reduce(cb, ce, itb, 0, std::plus{}, [](auto l, auto r) { return l == r ? 1 : 0; });
		++ito;
		++itb;
	}
}

// a line of enigma output
// assumption is that a machine will be configured and given a constant input.
// this structure records the output and each position that generates it.
//
template<size_t W> struct line_base
{
	static const size_t Width = W;
	// position of each column
	std::array<position, W> pos_;
	// line.
	std::array<modalpha, W> ln_;

	// define a matching results type.
	using results_t = std::array<unsigned, W>;
};

// applies ch to the machine and fills the line!
//
template<typename M, typename L> void fill_line(M& m3, L& l, modalpha ch)
{
	auto itp = std::begin(l.pos_);
	std::generate(std::begin(l.ln_), std::end(l.ln_), [&]()
	{
		*itp = m3.Position();
		++itp;
		return m3.Transform(ch);
	});
}

// assumes that *I is a modalpha, IOW 0-25 for A-Z
// update to genericity later.
//
template<typename I> double index_of_coincidence(I b, I e)
{
	auto N = std::distance(b, e);
	std::array<unsigned, alpha_max> tab;
	tab.fill(0);

	// count
	std::for_each(b, e, [&tab](auto c)
	{
		++tab[c.Val()];
	});
	// calculate
	double nn = double(N) * double(N - 1);

	return 	std::transform_reduce(std::begin(tab), std::end(tab), 0.0, std::plus{}, [nn](auto n)
	{
		return double(n * (n - 1)) / nn;
	});
}