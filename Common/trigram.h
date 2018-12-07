#pragma once

#include "modalpha.h"
#include "const_helpers.h"
#include "gram_common.h"

// definitions for the static table, that shouldn't end up in the binary...
///
struct tg_def
{
	char a_;
	char b_;
	char c_;
	unsigned score_;
};

// the table we end up with.
//
struct trigram_table
{
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
	for (auto& v : wking)
		v = 0;
//	epicyclism::cfor_each(std::begin(wking), std::end(wking), [](auto& v) { v = 0; });
	epicyclism::cfor_each(b, e, [&wking](auto const& tg)
		{
			wking[from_printable_flex(tg.a_) * stride_ + from_printable_flex(tg.b_) * stride_ + from_printable_flex(tg.c_)] = tg.score_;
		});
	return trigram_table{ wking };
}

// the end result used for scoring.
//
#include "trigram_data.h"
constexpr trigram_table gen_tg = make_trigram_table(std::begin(gen_tg_def), std::end(gen_tg_def));

// score with the given trigram table.
//
template<typename I> unsigned trigram_score(I b, I e, trigram_table const& tt)
{
	if (std::distance(b, e) < 3)
		return 0;
	auto len = std::distance(b, e);
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
	return score / (static_cast<unsigned>(len) - 2);
}

// convenience function.
//
template<typename I> unsigned trigram_score(I b, I e)
{
	return trigram_score(b, e, gen_tg);
}