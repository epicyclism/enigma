#pragma once

#include <algorithm>
#include <numeric>

// three methods for 'scoring'.
// 
// 1. The 'index of coincidence'.
// 2. Bigrams (these are baked in for now).
// 3. Trigrams (etc).

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