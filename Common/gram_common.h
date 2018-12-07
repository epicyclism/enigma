#pragma once

constexpr int stride_ = 32;

constexpr inline int from_printable_flex(char const ch)
{
	if (ch > 'Z')
		return ch - 'a';
	return ch - 'A';
}
