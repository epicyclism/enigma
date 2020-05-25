#pragma once

// a simple arena for fast decode
//
template<size_t W> struct arena_simple
{
	static const size_t Width = W;
#if defined (__NVCC__)
	modalpha arena_[alpha_max * W];
#else
	// a line
	using pos_t = std::array<modalpha, alpha_max>;

	// the arena
	std::array<pos_t, W> arena_;
#endif
};

// an arena for bulk fast decode, shouldn't be slower than calulating per position, should be quicker because each
// position only calculated once!
//
using arena_decode_t = arena_simple<26 * 26 * 26 + 250>;
