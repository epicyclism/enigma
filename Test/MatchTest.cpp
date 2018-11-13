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

int main()
{
	auto ct = make_alpha_array("SIAZKQGEMLIVDBIYWAKCAMPYKCFLOPQDCWPVMITCWAYWKBRUJAVGRYYCISIJZSGRMTZEKGEQLWUXIXYPMQLUHODQFPNRKBZDISWXPHYDBNEQHJUZJRZFWWMVTGIXFSFCQIBVMHGENWKNKYXMQRYSMAWCMBWFHYPNWJEBVYBZEZRCUFZYLIFFJCQFKGOGBYGXMDJLUJMMKZDLNNNJIYEAOYUVDFRFCCUVPWYPJHWFSGGRLXQDFFOKLSKGXZ");

	machine3 m3 = MakeMachine3("B213");
	Ring(m3, "zcp");
	m3.ReportSettings(std::cout);
	std::cout << "\nReady\n";
	fill_arena(m3.Wheels(), a, 0);

	stkrset ss;
	match_ciphertext(std::begin(ct), std::end(ct), std::begin(a.arena_[0]), ss);
	print(ss);

}