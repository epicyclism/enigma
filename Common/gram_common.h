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
	const std::array<unsigned, stride_* alpha_max> tab_;

	[[nodiscard]] unsigned wt(modalpha a, modalpha b) const noexcept
	{
		unsigned off = a.Val() * stride_ + b.Val();
		return tab_[off];
	}
	bigram_table() = delete;
};

struct trigram_table
{
	const std::array<unsigned, stride_* stride_* alpha_max> tab_;

	[[nodiscard]] unsigned wt(modalpha a, modalpha b, modalpha c) const noexcept
	{
		unsigned off = a.Val() * stride_ * stride_ + b.Val() * stride_ + c.Val();
		return tab_[off];
	}
	trigram_table() = delete;
};
