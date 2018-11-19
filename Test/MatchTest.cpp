// Matchtest.cpp
//
// test the matching function between Enigma output at a steady input and the ciphertext looking for stecker sets
// that maximise the match
//

#include <iostream>
#include <iomanip>
#include "const_helpers.h"
#include "machine.h"
#include "arena.h"
#include "match.h"

using arena_t = arena_base<26 * 26 * 26 + 256>;
//using arena_t = arena_base<26 + 256>;

arena_t a;

void report(arena_t::results_t const& r)
{
	auto itb = std::begin(r);
	while (itb != std::end(r))
	{
		std::cout << *itb << "\n";
		++itb;
	}
}


void report2(arena_t::results_t const& r)
{
	std::array<int, 61> hi;
	hi.fill(0);

	auto itb = std::begin(r);
	while (itb != std::end(r))
	{
		if (*itb >= 60)
			++hi[60];
		else
			++hi[*itb];
		++itb;
	}
	int i = 0;
	for (auto n : hi)
	{
		std::cout << i << " " << n << "\n";
		++i;
	}
	std::cout << "\n";
	std::cout << "\n";
}

void all_report2(arena_t const& a)
{
	for (auto& r : a.results_)
		report2(r);
}

void mega_report(arena_t const& a, double iocs[arena_t::Width], int N)
{
	// for each column compute the effective index of coincidence.
	for (int c = 0; c < a.Width; ++c)
	{
		// we don't have a 'slice' iterator? Might look at valarray for the arena?
		double sm = 0.0;
		for ( int r = 0; r < 26; ++r)
		{
			int n = a.results_[r][c];
			sm += double(n * (n - 1));
		}
		iocs[c] = sm / (double(N * (N - 1)));
	}
}

void mega_report(arena_t const& a)
{
	for (int r = 0; r < 26; ++r)
	{
		for (int c = 0; c < a.Width - 256; ++c)
		{
			std::cout << std::setw(2) << a.results_[r][c] << " ";
		}
		std::cout << "\n";
	}
}

void report3(arena_t const& a, modalpha k)
{
	auto const& r = a.results_[k.Val()];

	for (int c = 0; c < a.Width - 256; ++c)
	{
		if (r[c] > 30 && r[c] < 40)
			std::cout << a.pos_[c] << " - " << r[c] << "\n";
	}
}

int main()
{
	// B213 zcp YTL "BM DV KT LN RS UP XZ EA QW OI"
	auto ct = make_alpha_array("SIAZKQGEMLIVDBIYWAKCAMPYKCFLOPQDCWPVMITCWAYWKBRUJAVGRYYCISIJZSGRMTZEKGEQLWUXIXYPMQLUHODQFPNRKBZDISWXPHYDBNEQHJUZJRZFWWMVTGIXFSFCQIBVMHGENWKNKYXMQRYSMAWCMBWFHYPNWJEBVYBZEZRCUFZYLIFFJCQFKGOGBYGXMDJLUJMMKZDLNNNJIYEAOYUVDFRFCCUVPWYPJHWFSGGRLXQDFFOKLSKGXZ");

	machine3 m3 = MakeMachine3("B213");
	Ring(m3, "zcp");
	m3.Setting(alpha::Y, alpha::T, alpha::L);
	std::cout << "# ";
	m3.ReportSettings(std::cout);
	std::cout << "\n# Ready\n";
	fill_arena(m3.Wheels(), a, 0);
	
	for (int i = 0; i < 1; ++i)
	{
		a.results_[i].fill(0);
		match_search(std::begin(ct), std::end(ct) - 1, a.arena_[i], a.results_[i], modalpha(i));
	}
	//	all_report2(a);
#if 0
	report3(a, alpha::A);
	double iocs[arena_t::Width];
	mega_report(a, iocs);
	int i = 0;
	for (auto ioc : iocs)
	{
		if (ioc > 0.0385)
			std::cout << a.pos_[i] << " - " << ioc << "\n";
		++i;
	}
	mega_report(a);
	double iocs[arena_t::Width];
	mega_report(a, iocs, std::distance(std::begin(ct), std::end(ct)));
	for ( int i = 0; i < 26; ++i)
	{
		std::cout << a.pos_[i] << " - " << iocs[i] << "\n";
	}
#endif
}