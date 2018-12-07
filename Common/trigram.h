#pragma once

#include "modalpha.h"
#include "const_helpers.h"

// definitions for the static table, that shouldn't end up in the binary...
///
struct tg_def
{
	char a_;
	char b_;
	char c_;
	unsigned score_;
};

constexpr inline int from_printable_flex(char const ch)
{
	if (ch > 'Z')
		return ch - 'a';
	return ch - 'A';
}

// the table we end up with.
//
struct trigram_table
{
	constexpr int stride_ = 32;
	const std::array<unsigned, stride_ * stride_ * alpha_max> tab_;

	[[nodiscard]] const unsigned wt(modalpha a, modalpha b, modalpha c) const noexcept
	{
		unsigned off = a.Val() * stride_ + b.Val() * stride_ + c.Val();
		return tab_[off];
	}
	trigram_table() = delete;
};

constexpr trigram_table make_trigram_table(tg_def const* b, tg_def const* e)
{
	std::array<unsigned, stride_ * stride_ * alpha_max> wking{};
	epicyclism::cfor_each(std::begin(wking), std::end(wking), [](auto& v) { v = 0; });
	epicyclism::cfor_each(b, e, [&wking](auto const& bg)
	{
		wking[from_printable_flex(bg.a_) * stride_ + from_printable_flex(bg.b_) * stride_+ from_printable_flex(bg.c_)] = tg.score_;
	});
	return trigram_table{ wking };
}

#include "trigram_data.h"

// score with the given bigram table.
//
template<typename I> unsigned trigram_score(I b, I e, trigram_table const& tt)
{
	if (std::distance(b, e) < 3)
		return 0;
	unsigned score = 0;
	auto m = b + 1;
	auto n = m + 1;
	while (n != e)
	{
		score += tt.wt(*b, *m, *n);
		++b;
		++m;
		++n;
	}
	return score;
}

// the end result used for scoring.
//
// might want to move the table into a separate compilation unit.
//
constexpr trigram_table gen_tg = make_trigram_table(std::begin(gen_tg_def), std::end(gen_tg_def));

// convenience function.
//
template<typename I> unsigned trigram_score(I b, I e)
{
	return trigram_score(b, e, gen_tg);
}
