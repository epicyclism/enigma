#pragma once

#include <algorithm>
#include <numeric>

#if !defined (_MSC_VER)
#if defined ( __GNUC__ ) && ( __GNUC__ < 9)
#define SEQ_PROC
#endif
#endif

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

// generate a sorted frequency array, pairs of letter/frequency
//
template<typename I> std::array<std::pair<modalpha, unsigned>, alpha_max> gen_freq_tab_seq(I b, I e)
{
	static_assert(std::is_same<modalpha, typename I::value_type>::value);

	std::array<std::pair<modalpha, unsigned>, alpha_max> tab;
	modalpha l{ alpha::A };
	std::for_each(tab.begin(), tab.end(), [&l](auto& p) { p.first = l, p.second = 0; ++l; });
	// count
	std::for_each(b, e, [&tab](auto c)
		{
			++tab[c.Val()].second;
		});
	std::sort(tab.begin(), tab.end(), [](auto& l, auto& r) { return l.second > r.second; });

	return tab;
}

// return an array of modalpha in freq order, high low
template<typename I> std::array<modalpha, alpha_max> gen_freq_seq(I b, I e)
{
	static_assert(std::is_same<modalpha, typename I::value_type>::value);

	std::array<std::pair<unsigned, modalpha>, alpha_max> tab;
	modalpha l{ alpha::A };
	std::for_each(tab.begin(), tab.end(), [&l](auto& p) { p.first = 0, p.second = l; ++l; });
	// count
	std::for_each(b, e, [&tab](auto c)
	{
		++tab[c.Val()].first;
	});
	std::sort(tab.begin(), tab.end(), [](auto& l, auto& r) { return l.first > r.first; });

	std::array<modalpha, alpha_max> ra;
	std::transform(tab.begin(), tab.end(), ra.begin(), [](auto& p) { return p.second; });

	return ra;;
}

// as above but don't count the 'popular' letters, ENXRSI
template<typename I> std::array<modalpha, alpha_max> gen_freq_seq_special(I b, I e)
{
	static_assert(std::is_same<modalpha, typename I::value_type>::value);

	std::array<std::pair<int, modalpha>, alpha_max> tab;
	modalpha l{ alpha::A };
	std::for_each(tab.begin(), tab.end(), [&l](auto& p) { p.first = 0, p.second = l; ++l; });
	tab[  4 /*alpha::E*/].first = -256;
	tab[  8 /*alpha::I*/].first = -256;
	tab[ 13 /*alpha::N*/].first = -256;
	tab[ 17 /*alpha::R*/].first = -256;
	tab[ 18 /*alpha::S*/].first = -256;
	tab[ 23 /*alpha::X*/].first = -256;
	// count
	std::for_each(b, e, [&tab](auto c)
	{
		++tab[c.Val()].first;
	});
	std::sort(tab.begin(), tab.end(), [](auto& l, auto& r) { return l.first > r.first; });

	std::array<modalpha, alpha_max> ra;
	std::transform(tab.begin(), tab.end(), ra.begin(), [](auto& p) { return p.second; });

	return ra;;
}

// Score a piece of text (presumably a trial decrypt) using the 'index of coincidence'
// as documented widely, for example, https://en.wikipedia.org/wiki/Index_of_coincidence
// Doesn't normalise by multiplying through by alpha_max, as seems to be the convention.

double ioc_wkr(std::array<unsigned, alpha_max> const& tab, size_t N)
{
	double nn = double(N) * double(N - 1);

#if defined(SEQ_PROC)
	return 	std::inner_product(std::begin(tab), std::end(tab), std::begin(tab), 0.0, std::plus{}, [nn](auto n, auto)
	{
		return double(n * (n - 1)) / nn;
	});
#else
	return 	std::transform_reduce(std::begin(tab), std::end(tab), 0.0, std::plus{}, [nn](auto n)
	{
		return double(n * (n - 1)) / nn;
	});
#endif
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

#if defined(SEQ_PROC)
	return 	std::inner_product(std::begin(tab), std::end(tab), std::begin(tab), 0.0, std::plus{}, [nn](auto n, auto)
	{
		return double(n * (n - 1)) / nn;
	});
#else
	return 	std::transform_reduce(std::begin(tab), std::end(tab), 0.0, std::plus{}, [nn](auto n)
	{
		return double(n * (n - 1)) / nn;
	});
#endif
}

struct index_of_coincidence_op
{
	template<typename I> double operator()(I b, I e)
	{
		return index_of_coincidence(b, e);
	}
};

