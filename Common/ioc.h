#pragma once

#include <algorithm>
#include <numeric>
#include <execution>

// Score a piece of text (presumably a trial decrypt) using the 'index of coincidence'
// as documented widely, for example, https://en.wikipedia.org/wiki/Index_of_coincidence
// Doesn't normalise by multiplying through by alpha_max, as seems to be the convention.

// generate a frequency array (assuming alpha_max etc and modalpha)
template<typename I> std::array<unsigned, alpha_max> gen_freq_tab(I b, I e)
{
	static_assert(std::is_same<modalpha, typename I::value_type>::value);

	std::array<unsigned, alpha_max> tab;
	tab.fill(0);
	// count
	std::for_each(b, e, [&tab](auto c)
	{
		++tab[c.Val()];
	});

	return tab;
}

double ioc_wkr(std::array<unsigned, alpha_max> const& tab, size_t N)
{
	double nn = double(N) * double(N - 1);

	return 	std::transform_reduce(std::execution::seq, std::begin(tab), std::end(tab), 0.0, std::plus{}, [nn](auto n)
	{
		return double(n * (n - 1)) / nn;
	});
}

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

	return 	std::transform_reduce(std::execution::seq, std::begin(tab), std::end(tab), 0.0, std::plus{}, [nn](auto n)
	{
		return double(n * (n - 1)) / nn;
	});
}

struct index_of_coincidence_op
{
	template<typename I> double operator()(I b, I e)
	{
		return index_of_coincidence(b, e);
	}
};

