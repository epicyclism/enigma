#pragma once

#include "modalpha.h"

// bigram definitions

struct bigram_table
{
	const std::array<alpha_max * alpha_max, unsigned> tab_;

	[[nodiscard]] const unsigned wt(modalpha a, modalpha b) const noexcept
	{
		unsigned off = b;
		off *= alpha_max;
		off += a;
		return tab_[off];
	}
	bigram_table() = delete;
};

constexpr bigram_table make_bigram_table();
