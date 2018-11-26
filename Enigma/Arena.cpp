// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <execution>
#include <algorithm>
#include <numeric>
#include "wheelset.h"
#include "machine.h"
#include "position.h"
#include "match.h"
#include "arena.h"

constexpr  char version[] = "v0.03";

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

void Help()
{
	std::cerr << "arena " << version << " : Enigma Stecker hunt.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena B125 fvn\n";
	std::cerr << "Configures a machine for wheels B125 and ring fvn then reads ciphertext from stdin\n";
	std::cerr << "and attempts to find plug settings that produce a decrypt\n\n";
}

using arena_t = arena_base<26*26*26 + 256>;

arena_t a;

template<typename CI> struct job
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;
	arena_t::line_t         const& line_;
	arena_t::position_t     const& pos_;
	arena_t::results_t           & r_;
	modalpha                const  bs_;

	job(machine_settings_t const& mst, CI ctb, CI cte, arena_t::line_t const& l, arena_t::position_t const& pos, arena_t::results_t& r, modalpha bs)
		: mst_(mst), ctb_(ctb), cte_(cte), line_(l), pos_(pos), r_(r), bs_(bs)
	{}
};

// these are sort of shared and could be common...
//
template<typename J, typename R> void collect_results(J const& j, R& r)
{
	unsigned cnt_ = 0;
	auto sz = std::distance(j.ctb_, j.cte_);
	auto itp = std::begin(j.pos_);
	int threshold = 24;

	auto rb = std::begin(j.r_);
	while (rb != std::end(j.r_))
	{
		int score = *rb;

		if (score > threshold) // decode!
		{
			++cnt_;
			auto off = std::distance(std::begin(j.r_), rb);
			use_ees(j.ctb_, j.cte_, std::begin(j.line_) + off, *(itp + off), j.bs_, j.mst_, r);
		}
		++rb;
	}
	std::cout << "Evaluated " << cnt_ << " results.\n";
}

template<typename I> void report_result(result_t const& r, I cb, I ce)
{
	machine3 m3 = MakeMachine3(r.mst_);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(cb, ce));
	auto cbc = cb;
	while (cbc != ce)
	{
		vo.push_back(m3.Transform(*cbc));
		++cbc;
	}
	m3.ReportSettings(std::cout);
	std::cout << " - ";
	std::cout << r.ioc_ << " - ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";
}

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
#else
		machine3 m3 = MakeMachine3("B213");
		Ring(m3, "zcp");
#endif
		m3.Setting(alpha::A, alpha::A, alpha::A);
		std::cout << "arena " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady\n";
		// capture the ciphertext
//		auto ct = read_ciphertext();
		auto ct = make_alpha_array("YNDXIHNTJYETDDJVBPCAPORBPPASUKHYHTHETMFGJNPUFWAMEBFIKQBZGGFZZXJMUYNJDWXJXZDMEEVPYRDGPYMAXWTWHUGDQZTMJWKYQRDQXKVGTZYIIMPBVDJPQVJLOIOSXQENZZHCNTWCQYQYMHCOXPNTDXMTZWABTWRVYIGMJEICMHXHHEITFPKXEFWMICOVTIVIBIEACPFVXZILJXWTBRVBEFENEWQZTCCDMWVWGLDZTXGUDJWSTR");

		std::cout << "\nInitialising search\n";
		fill_arena(m3.Wheels(), a, 0);
		// create the jobs
//		std::vector < job< std::vector<modalpha>::const_iterator>> vjb;
		std::vector < job< std::array<modalpha, 251>::const_iterator>> vjb;
		vjb.reserve(alpha_max);
		for ( int i = 0; i < alpha_max; ++i)
		{
			vjb.emplace_back(m3.machine_settings(), std::begin(ct), std::end(ct), a.arena_[i], a.pos_, a.results_[i], modalpha(i));

		}
		// run the jobs
		std::cout << "Searching\n";
		std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& j)
		{
			match_search(j.ctb_, j.cte_, j.line_, j.r_, j.bs_);
		});

		// evaluate and report!
		std::cout << "Evaluating\n";
		std::vector<result_t> results;
		for (auto& j : vjb)
		{
			collect_results(j, results);
		}
		for (auto& res : results)
		{
			report_result(res, std::begin(ct), std::end(ct));
		}

		std::cout << "Finished\n";
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
