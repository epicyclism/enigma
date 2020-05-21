#pragma once

#include <array>
#include "wheelset.h"

#include <execution>

#include "arena_simple.h"

// maintain alpha_max rows of width W, each row is the output of encoding continuous letters in the machine.
// position recorded to ensure can go back without having to separate out the stepping process.
//
template<size_t W> struct arena_base
{
	// a line
	using line_t = std::array<modalpha, W>;
	// a result for a line
	using results_t = std::array<unsigned, W>;
	// remember the position for each column
	using position_t = std::array<position, W>;
	static const size_t Width = W;

	size_t active_width_;
	// position of each column
	position_t pos_;
	// array of lines.
	std::array<line_t, alpha_max> arena_;
	// array of results
	std::array<results_t, alpha_max> results_;
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
		if constexpr (!std::is_same_v<A, arena_simple<A::Width>>)
			a.pos_[i] = w.Position();
		w.Step();
		auto col = w.Evaluate(alphabet);
		auto itc = std::begin(col);
		for (auto ita = std::begin(a.arena_); ita != std::end(a.arena_); ++ita, ++itc)
		{
			(*ita)[i] = *itc;
		}
	}
	if constexpr (!std::is_same_v<A, arena_simple<A::Width>>)
		a.active_width_ = A::Width - off;
}

template<typename A> void fill_arena_simple(wheels3& w, A& a)
{
	std::for_each(a.arena_.begin(), a.arena_.end(), [&w](auto& pos)
		{
			w.Step();
			auto col = w.Evaluate(alphabet);
			std::copy(col.begin(), col.end(), pos.begin());
		});
}

// fills the arena for width steps, so that the cycle length of the particular machine plus
// the length of the ciphertext can be adjusted.
// With double step wheels cycle 6, 7, 8, length notably shorter, min 26*13*13 = 4394, max 26*26*26 = 17576. Roughly.
//
template<typename A> void fill_arena_width(wheels3& w, A& a, size_t width)
{
	a.active_width_ = std::min(A::Width, width);
	// for each wheel state we have to go down all 26 arrays
	for (auto i = 0; i < a.active_width_; ++i)
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
		*ito += std::transform_reduce(std::execution::seq, cb, ce, itb, 0, std::plus{}, [](auto l, auto r) { return l == r ? 1 : 0; });
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

// work on an arena
//
// A arena
// CI ciphertext iterator
// R is a result to collect good hits into.
//   and is therefore somewhat optional
//
template<typename A, typename R, typename CI> struct job_arena
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;
	typename A::line_t     const& line_;
	typename A::position_t const& pos_;
	typename A::results_t& r_;
	modalpha       const  bs_;
	std::vector<R>        vr_;

	job_arena(machine_settings_t const& mst, CI ctb, CI cte, typename A::line_t const& l, typename A::position_t const& pos, typename A::results_t& r, modalpha bs)
		: mst_(mst), ctb_(ctb), cte_(cte), line_(l), pos_(pos), r_(r), bs_(bs)
	{
	}
};

template<typename J, typename A, typename CI> auto make_job_list_arena(machine_settings_t mst, A& a, CI ctb, CI cte) -> std::vector<J>
{
	std::vector<J> vjb;
	for (int i = 0; i < 26; ++i)
	{
		a.results_[i].fill(0);
		vjb.emplace_back(mst, ctb, cte, a.arena_[i], a.pos_, a.results_[i], modalpha(i));
	}

	return vjb;
}
