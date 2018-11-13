// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <execution>
#include <algorithm>
#include <numeric>
#include "wheelset.h"
#include "machine.h"
#include "arena.h"
#include "position.h"

constexpr  char version[] = "v0.02";

struct stkr
{
	modalpha f_;
	modalpha t_;
	short    cnt_;
};

// assumed max message length
struct stkrset
{
	std::array<stkr, 256> sss_;
	int direct_;
};

void clear(stkrset& ss)
{
	for (auto& s : ss.sss_)
	{
		s.f_ = alpha::SZ;
		s.t_ = alpha::SZ;
		s.cnt_ = 0;
	}
	ss.direct_ = 0;
}

void set(stkrset& ss, modalpha f, modalpha t)
{
	if (f == t)
	{
		++ss.direct_;
		return;
	}
	if (f > t)
	{
		std::swap(f, t);
	}
	auto& sss = ss.sss_;
	std::for_each(std::begin(sss), std::end(sss), [&f, &t](auto& pg)
	{
		if (pg.f_ == alpha::SZ)
		{
			pg.f_ = f;
			pg.t_ = t;
			++pg.cnt_;
			return;
		}
		if ((pg.f_ == f) && (pg.t_ == t))
		{
			++pg.cnt_;
			return;
		}
	});
}

void print(stkrset const& ss)
{
	std::cout << "direct = " << ss.direct_ << "\n";
	for (auto& s : ss.sss_)
	{
		std::cout << s.f_ << "<->" << s.t_ << " - " << s.cnt_ << "\n";
	}
}

std::vector<modalpha> read_ciphertext()
{
	std::vector<modalpha> rv;
	while (1)
	{
		char c;
		std::cin >> c;
		if (!std::cin)
			break;
		if (valid_from_char(c))
		{
			rv.push_back(from_printable(c));
		}
	}
	return rv;
}

template<typename J> void stecker_search(J& j)
{
	auto itb = std::begin(j.line_);
	auto ite = std::end(j.line_) - std::distance(j.ctb_, j.cte_);
	stkrset ss;

	while (itb != ite)
	{
		clear(ss);
		// collect stecker possibles
		auto it = itb;
		std::for_each(j.ctb_, j.cte_, [&it, &ss](auto const c) { set(ss, c, *it); ++it; });
		// evaluate them
		++itb;
	}
}

void Help()
{
	std::cerr << "arena " << version << " : Enigma Stecker hunt experiments.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena B125 fvn\n";
	std::cerr << "Configures a machine for wheels B125 and ring fvn then reads ciphertext from stdin\n";
	std::cerr << "and attempts to find plug settings that produce a decrypt\n\n";
}

using arena_t = arena_base<26*26*26 + 256>;

arena_t a;

// worthwhile hit
// includes possible 'stecker' and position
//
struct result
{
	position pos_;
	als_t    board_;
};

template<typename CI> struct job
{
	CI ctb_;
	CI cte_;
	arena_t::line_t         const& line_;
	std::array<position, arena_t::Width> const& pos_;
	std::vector<result> r_;

	job(CI ctb, CI cte, arena_t::line_t const& l, std::array<position, arena_t::Width> const& pos) : ctb_(ctb), cte_(cte), line_(l), pos_(pos)
	{}
};

int main(int ac, char**av)
{
#if 0
	if (ac < 3)
	{
		Help();
		return 0;
	}
#endif
	try
	{
#if 0
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);
		m3.Setting(alpha::A, alpha::A, alpha::A);
#else
		machine3 m3 = MakeMachine3("B213");
		Ring(m3, "zcp");
#endif
		std::cout << "arena " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady\n";
		// capture the ciphertext
//		auto ct = read_ciphertext();
		auto ct = make_alpha_array("SIAZKQGEMLIVDBIYWAKCAMPYKCFLOPQDCWPVMITCWAYWKBRUJAVGRYYCISIJZSGRMTZEKGEQLWUXIXYPMQLUHODQFPNRKBZDISWXPHYDBNEQHJUZJRZFWWMVTGIXFSFCQIBVMHGENWKNKYXMQRYSMAWCMBWFHYPNWJEBVYBZEZRCUFZYLIFFJCQFKGOGBYGXMDJLUJMMKZDLNNNJIYEAOYUVDFRFCCUVPWYPJHWFSGGRLXQDFFOKLSKGXZ");

		std::cout << "\nInitialising search\n";
		fill_arena(m3.Wheels(), a, 0);
		// create the jobs
//		std::vector < job< std::vector<modalpha>::const_iterator>> vjb;
		std::vector < job< std::array<modalpha, 251>::const_iterator>> vjb;
		vjb.reserve(arena_t::Width - ct.size());
		std::for_each(std::begin(a.arena_), std::end(a.arena_), [&ct, &vjb](auto const& l)
		{
			vjb.emplace_back(std::begin(ct), std::end(ct), l, a.pos_ );
		});
		// run the jobs
		std::cout << "\nSearching\n";
		std::for_each(/*std::execution::par,*/ std::begin(vjb), std::end(vjb), [](auto& j) { stecker_search(j); });

		// evaluate and report!
		std::cout << "\nFinished\n";
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
