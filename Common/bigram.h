#pragma once

#include "modalpha.h"
#include "const_helpers.h"
#include "gram_common.h"

// definitions for the static tables, that shouldn't end up in the binary...
///
struct bg_def
{
	char a_;
	char b_;
	unsigned score_;
};

// the tables we end up with.
//
struct bigram_table
{
	const std::array<unsigned, stride_ * alpha_max> tab_;

	[[nodiscard]] const unsigned wt(modalpha a, modalpha b) const noexcept
	{
		unsigned off = a.Val() * stride_ + b.Val();
		return tab_[off];
	}
	bigram_table() = delete;
};

// general data table
#include "bigram_data.h"

// bigram
//
constexpr bigram_table make_bigram_table(bg_def const* b, bg_def const* e, int scale = 1)
{
	std::array<unsigned, stride_ * alpha_max> wking{};
	for (auto& v : wking)
		v = 0;
//	epicyclism::cfor_each(std::begin(wking), std::end(wking), [](auto& v) { v = 0; });
	epicyclism::cfor_each(b, e, [&wking, scale](auto const& bg)
		{
			wking[from_printable_flex(bg.a_) * stride_ + from_printable_flex(bg.b_)] = bg.score_ * scale;
		});
	return bigram_table{ wking };
}

// the end result used for scoring.
//
constexpr bigram_table bg_gen  = make_bigram_table(std::begin(gen_bg_def), std::end(gen_bg_def));
constexpr bigram_table bg_1941 = make_bigram_table(std::begin(bg_def_1941), std::end(bg_def_1941), 100);
constexpr bigram_table bg_xxx  = make_bigram_table(std::begin(bg_def_xxx), std::end(bg_def_xxx));

// score with the given bigram table.
//
template<typename I> unsigned bigram_score(I b, I e, bigram_table const& bt)
{
	if (b == e)
		return 0;
	auto len = std::distance(b, e);
	unsigned score = 0;
	auto n = b + 1;
	while (n != e)
	{
		score += bt.wt(*b, *n);
		++b;
		++n;
	}
	// normalise to message length here, right down at the source.
	return score / (static_cast<unsigned>(len) - 1);
}

// convenience functions.
//
template<typename I> unsigned bigram_score_gen(I b, I e)
{
	return bigram_score(b, e, bg_gen) ;
}

struct bigram_score_gen_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return bigram_score(b, e, bg_gen);
	}
};

template<typename I> unsigned bigram_score_1941(I b, I e)
{
	return bigram_score(b, e, bg_1941) ;
}

struct bigram_score_1941_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return bigram_score(b, e, bg_1941);
	}
};

template<typename I> unsigned bigram_score_xxx(I b, I e)
{
	return bigram_score(b, e, bg_xxx) ;
}

struct bigram_score_xxx_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return bigram_score(b, e, bg_xxx);
	}
};

template<typename I> unsigned bigram_score(I b, I e)
{
	return bigram_score(b, e, bg_xxx) ;
}

struct bigram_score_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return bigram_score(b, e, bg_xxx);
	}
};
