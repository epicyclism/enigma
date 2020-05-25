#pragma once

constexpr int stride_ = 32;

constexpr inline int from_printable_flex(char const ch)
{
	if (ch > 'Z')
		return ch - 'a';
	return ch - 'A';
}

struct bigram_table
{
#if defined (__NVCC__)
	unsigned tab_[stride_ * alpha_max];
#else
	const std::array<unsigned, stride_* alpha_max> tab_;
#endif
#if defined (__NVCC__)
	__device__
#else
	[[nodiscard]]
#endif
	unsigned wt(modalpha a, modalpha b) const noexcept
	{
		unsigned off = a.Val() * stride_ + b.Val();
		return tab_[off];
	}
	bigram_table() = delete;
};

struct trigram_table
{
#if defined (__NVCC__)
	unsigned tab_[stride_ * stride_ * alpha_max];
#else
	const std::array<unsigned, stride_* stride_* alpha_max> tab_;
#endif

#if defined (__NVCC__)
	__device__
#else
	[[nodiscard]]
#endif
	unsigned wt(modalpha a, modalpha b, modalpha c) const noexcept
	{
		unsigned off = a.Val() * stride_ * stride_ + b.Val() * stride_ + c.Val();
		return tab_[off];
	}
	trigram_table() = delete;
};
