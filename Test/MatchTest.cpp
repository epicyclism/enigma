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

arena_t a;

void reportwkr(arena_t::results_t const& r, std::array<int, 61>& hi)
{
	auto itb = std::begin(r);
	while (itb != std::end(r))
	{
		int score = ((*itb & 0x0000ffff) - (*itb >> 16));
		if (score >= 60)
			++hi[60];
		else
		if (score < 0)
			++hi[0];
		else
			++hi[score];
		++itb;
	}
}

void report(arena_t::results_t const& r)
{
	std::array<int, 61> hi;
	hi.fill(0);

	reportwkr(r, hi);

	int i = 0;
	for (auto n : hi)
	{
		std::cout << i << " " << n << "\n";
		++i;
	}
	std::cout << "\n";
}

void reportall(arena_t const& a)
{
	std::array<int, 61> hi;
	hi.fill(0);

	for (auto& r : a.results_)
		reportwkr(r, hi);

	int i = 0;
	for (auto n : hi)
	{
		std::cout << i << " " << n << "\n";
		++i;
	}
	std::cout << "\n";
}

int main()
{
	// B213 zcp YTL "BM DV KT LN RS UP XZ EA QW OI"
	//auto ct = make_alpha_array("SIAZKQGEMLIVDBIYWAKCAMPYKCFLOPQDCWPVMITCWAYWKBRUJAVGRYYCISIJZSGRMTZEKGEQLWUXIXYPMQLUHODQFPNRKBZDISWXPHYDBNEQHJUZJRZFWWMVTGIXFSFCQIBVMHGENWKNKYXMQRYSMAWCMBWFHYPNWJEBVYBZEZRCUFZYLIFFJCQFKGOGBYGXMDJLUJMMKZDLNNNJIYEAOYUVDFRFCCUVPWYPJHWFSGGRLXQDFFOKLSKGXZ");
	// B213 zcp MUM "BM DV KT LN RS UP XZ EA QW OI"
	auto ct = make_alpha_array("YNDXIHNTJYETDDJVBPCAPORBPPASUKHYHTHETMFGJNPUFWAMEBFIKQBZGGFZZXJMUYNJDWXJXZDMEEVPYRDGPYMAXWTWHUGDQZTMJWKYQRDQXKVGTZYIIMPBVDJPQVJLOIOSXQENZZHCNTWCQYQYMHCOXPNTDXMTZWABTWRVYIGMJEICMHXHHEITFPKXEFWMICOVTIVIBIEACPFVXZILJXWTBRVBEFENEWQZTCCDMWVWGLDZTXGUDJWSTR");
	// B213 zcp NWF "BM DV KT LN RS UP XZ EA QW OI"
	//auto ct = make_alpha_array("BKWVQICHPWRRYJDAXQEIQJKQQYMLTPVAKYCJZZTDAODOLSTOKLSSXJRTQCKIKGRRDRJZYZWWJPTABZJEOWGRUKLASPPBMKZBJRHIOKPAKYFZPCOUAAXDMZQMTLDFNNKEZDGRNUZQA");

	machine3 m3 = MakeMachine3("B213");
	Ring(m3, "zcp");
	m3.Setting(alpha::M, alpha::U, alpha::M);
	std::cout << "# ";
	m3.ReportSettings(std::cout);
	std::cout << "\n# Ready\n";
	fill_arena(m3.Wheels(), a, 0);
#if 0	
	for (int i = 0; i < 26; ++i)
	{
		a.results_[i].fill(0);
		match_search(std::begin(ct), std::end(ct) - 1, a.arena_[i], a.results_[i], modalpha(i));
	}
	reportall(a);
#else
	a.results_[0].fill(0);
	match_test(std::begin(ct), std::end(ct) - 1, a.arena_[0], a.results_[0], modalpha(0));
	auto mst_n = use_ees_test(std::begin(ct), std::end(ct) - 1, std::begin(a.arena_[0]), a.pos_[0], modalpha(0), m3.machine_settings());
//	hillclimb_test(std::begin(ct), std::end(ct) - 1, a.pos_[0], modalpha(0), mst_n);
//	hillclimb_test(std::begin(ct), std::end(ct) - 1, a.pos_[0], modalpha(0), m3.machine_settings());
//	a.results_[1].fill(0);
//	match_test(std::begin(ct), std::end(ct) - 1, a.arena_[1], a.results_[1], modalpha(1));
//	mst_n = use_ees_test(std::begin(ct), std::end(ct) - 1, std::begin(a.arena_[1]), a.pos_[0], modalpha(1), m3.machine_settings());
//	hillclimb_test(std::begin(ct), std::end(ct) - 1, a.pos_[0], modalpha(1), mst_n);
//	hillclimb_test(std::begin(ct), std::end(ct) - 1, a.pos_[0], modalpha(1), m3.machine_settings());
#endif
}