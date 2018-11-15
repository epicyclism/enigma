// Matchtest.cpp
//
// test the matching function between Enigma output at a steady input and the ciphertext looking for stecker sets
// that maximise the match
//

#include <iostream>
#include "const_helpers.h"
#include "machine.h"
#include "arena.h"
#include "match.h"

using arena_t = arena_base<26 * 26 * 26 + 256>;

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
	
	arena_t::results_t r;
	r.fill(0);
	match_search(std::begin(ct), std::end(ct) - 1,  a.arena_[0], r, alpha::A);
//	report(r);
}