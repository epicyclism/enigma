// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#if !defined (__GNUC__)
#include <execution>
#endif
#include <algorithm>
#include <numeric>
#if defined (__GNUC__)
#include <pstl/execution>
#include <pstl/algorithm>
#include <pstl/numeric>
#endif
#include "wheelset.h"
#include "machine.h"
#include "position.h"
#include "ioc.h"
#include "bigram.h"
#include "match.h"
#include "arena.h"
#include "jobs.h"

constexpr  char version[] = "v0.04";

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
	std::cerr << "arena " << version << " : Enigma settings hunt.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena BC 12345 [n]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 'n' determines where in the order to start, so that sessions can\n";
	std::cerr << "(effectively) be interrupted and restarted.\n\n";
}

using arena_t = arena_base<26*26*26 + 512>;

arena_t arena;

template<typename CI> struct job_wheels
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;

	job_wheels(machine_settings_t const& mst, CI ctb, CI cte)
		: mst_(mst), ctb_(ctb), cte_(cte)
	{}
};

template<typename J, typename... ARGS> auto make_job_list_t(std::string_view reflector, std::string_view wheels, ARGS... args) -> std::vector<J>
{
	machine_settings_t mst;
	ZeroRing(mst);
//	Ring(mst, "zcp");
	std::vector <J> vjb;
	mst.ref_ = reflector[0];
	mst.w3_ = wheels[0];
	mst.w2_ = wheels[1];
	mst.w1_ = wheels[2];
	vjb.emplace_back(mst, args...);

	return vjb;
}

// a results type that can hold the sequence of scores we use and the evolving mst
//
struct result_t
{
	machine_settings_t mst_;
	unsigned mtch_;
	double   ioc_;
	unsigned bg_;

	explicit result_t(machine_settings_t const& mst, double ioc) : mst_(mst), ioc_(ioc)
	{}
};

template<typename CI> struct job_arena
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;
	arena_t::line_t         const& line_;
	arena_t::position_t     const& pos_;
	arena_t::results_t           & r_;
	modalpha                const  bs_;
	std::vector<result_t>      vr_;

	job_arena(machine_settings_t const& mst, CI ctb, CI cte, arena_t::line_t const& l, arena_t::position_t const& pos, arena_t::results_t& r, modalpha bs)
		: mst_(mst), ctb_(ctb), cte_(cte), line_(l), pos_(pos), r_(r), bs_(bs)
	{
		vr_.reserve(256);
	}
};

template<typename J, typename CI> auto make_job_list_arena(machine_settings_t mst, arena_t& a, CI ctb, CI cte) -> std::vector<J>
{
	std::vector<J> vjb;
	for (int i = 0; i < 26; ++i)
	{
		a.results_[i].fill(0);
		vjb.emplace_back(mst, ctb, cte, a.arena_[i], a.pos_, a.results_[i], modalpha(i));
	}

	return vjb;
}

// these are sort of shared and could be common...
//
template<typename J> void collect_results(J& j)
{
	unsigned cnt_ = 0;
	auto sz = std::distance(j.ctb_, j.cte_);
	auto itp = std::begin(j.pos_);
	auto threshold = sz / 12;

	auto rb = std::begin(j.r_);
	while (rb != std::end(j.r_))
	{
		auto score = *rb;

		if (score > threshold) // decode!
		{
			++cnt_;
			auto off = std::distance(std::begin(j.r_), rb);
			use_ees(j.ctb_, j.cte_, std::begin(j.line_) + off, *(itp + off), j.bs_, j.mst_, j.vr_);
			// ick
			j.vr_.back().mtch_ = score;
		}
		++rb;
	}
}

template<typename J> std::vector<result_t> collate_results_ioc(std::vector<J> const& vj) 
{
	std::vector<result_t> vr;
	for (auto const& j : vj)
	{
		for (auto const& r : j.vr_)
		{
			if (r.ioc_ > 0.055)
				vr.emplace_back(r);
		}
	}
	return vr;
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

void collate_results_bg(std::vector<result_t> const& in, std::vector<result_t>& out)
{
	for (auto& r : in)
	{
		if (r.bg_ > 45000)
			out.emplace_back(r);
	}
}

int main(int ac, char**av)
{
#if 1
	if (ac < 3)
	{
		Help();
		return 0;
	}
	int joboffset = 0;
	if (ac > 3)
		joboffset = ::atoi(av[3]);
#endif
	// this is where we collect the overall results!
	std::vector<result_t> vr_oall;
	try
	{
		std::cout << "\nReady\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
//		auto ct = make_alpha_array("YNDXIHNTJYETDDJVBPCAPORBPPASUKHYHTHETMFGJNPUFWAMEBFIKQBZGGFZZXJMUYNJDWXJXZDMEEVPYRDGPYMAXWTWHUGDQZTMJWKYQRDQXKVGTZYIIMPBVDJPQVJLOIOSXQENZZHCNTWCQYQYMHCOXPNTDXMTZWABTWRVYIGMJEICMHXHHEITFPKXEFWMICOVTIVIBIEACPFVXZILJXWTBRVBEFENEWQZTCCDMWVWGLDZTXGUDJWSTR");
		std::cout << "Ciphertext is - ";
		for (auto c : ct)
			std::cout << c;
		std::cout << "\nInitialising search\n";
		using job_wheels_t = job_wheels<decltype(ct.cbegin())> ;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], joboffset, std::begin(ct), std::end(ct));
//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "123", std::begin(ct), std::end(ct));
//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "213", std::begin(ct), std::end(ct));

		std::cout << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";

		// work through the wheel orders linearly
		for (auto & j : vjbw)
		{
			// search each wheel order in parallel
			do
			{
				std::cout << j.mst_ << " " << vr_oall.size() << "\n";
				machine3 m3 = MakeMachine3(j.mst_);
				// fill the arena
				fill_arena(m3.Wheels(), arena, 0);
				// job list
				using job_arena_t = job_arena<decltype(ct.cbegin())>;
				auto vjb = make_job_list_arena<job_arena_t>(j.mst_, arena, j.ctb_, j.cte_);
				// do the search for quite likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						match_search(aj.ctb_, aj.cte_, aj.line_, aj.r_, aj.bs_);
					});

				// do the search for more likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						collect_results(aj);
					});
				// do the search for most likely
				auto vr = collate_results_ioc(vjb);
				std::for_each(std::execution::par, std::begin(vr), std::end(vr), [&ct](auto& r)
					{
						hillclimb(std::begin(ct), std::end(ct), r.mst_, r.bg_);
					});
				auto n = vr_oall.size();
				collate_results_bg(vr, vr_oall);
				std::for_each(vr_oall.begin() + n, vr_oall.end(), [&ct](auto& r)
					{
						machine3 m3 = MakeMachine3(r.mst_);
						std::vector<modalpha> vo;
						vo.reserve(ct.size());
						decode(std::begin(ct), std::end(ct), m3, vo);
						// report
						std::cout << r.mst_ << " { " << r.mtch_ << ", " << r.ioc_ << ", " << r.bg_ << " } : ";
						for (auto c : vo)
							std::cout << c;
						std::cout << "\n";
					});
			} while (AdvanceRing(j.mst_));
		}
		std::cout << "Finished\n";
		// report
		for (auto r : vr_oall)
		{
			machine3 m3 = MakeMachine3(r.mst_);
			std::vector<modalpha> vo;
			vo.reserve(ct.size());
			decode(std::begin(ct), std::end(ct), m3, vo);
			// report
			std::cout << r.mst_ << " { " << r.mtch_ << ", " << r.ioc_ << ", " << r.bg_ << " } : ";
			for (auto c : vo)
				std::cout << c;
			std::cout << "\n";
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
