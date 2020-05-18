// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <execution>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "wheelset.h"
#include "machine.h"
#include "position.h"
#include "ioc.h"
#include "bigram.h"
#include "match.h"
#include "hillclimb.h"
#include "arena.h"
#include "jobs.h"

constexpr  char version[] = "v0.13";

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
	std::cerr << "./arena BC 12345 [t [r [b [e]]]]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 't' is a value to use as the first pass threshold. Default is 50.\n";
	std::cerr << "The further optional trailing 'r' is the range of threshold to process.\n";
	std::cerr << "And the final optional trailing 'b' and 'e' values determine where in the order to\n";
	std::cerr << "start, and, if present where to end so that sessions can be interrupted,\n";
	std::cerr << "restarted or shared across systems.\n\n";
}

// assume worst case 256 char message at ZZZ.
// adjust this if you have a really long one...
//
using arena_t = arena_base<26 * 26 * 26 + 256>;
// this can be singular since parallelism is within an arena rather than over multiple arenas.
//
arena_t arena;

// thresholds
constexpr unsigned ees_threshold_default = 50; // greater than this
constexpr unsigned ees_threshold_range   = 1000000; // range to end of threshold window
constexpr double   ioc_threshold         = 0.05;
constexpr unsigned bg_threshold          = 48000; // bigram - not used at present, we always do both
constexpr unsigned tg_threshold          = 18000; // trigram

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
	double   ioc_;
	unsigned btg_;

	explicit result_t(machine_settings_t const& mst, unsigned scr) : mst_(mst), btg_(scr)
	{
		ioc_ = 0.0;
	}
	explicit result_t(machine_settings_t const& mst, double ioc) : mst_(mst), ioc_(ioc)
	{
		btg_ = 0;
	}
};

// these are sort of shared and could be common...
//
template<typename J> void collect_results(J& j, unsigned ees_thld, unsigned ees_thld_end)
{
	auto sz = std::distance(j.ctb_, j.cte_);
	auto itp = std::begin(j.pos_);

	auto rb = std::begin(j.r_);
	while (rb != std::end(j.r_))
	{
		auto score = *rb;
		if (score >= ees_thld && score < ees_thld_end) // decode!
		{
			auto off = std::distance(std::begin(j.r_), rb);
			use_ees(j.ctb_, j.cte_, std::begin(j.line_) + off, *(itp + off), j.bs_, j.mst_, j.vr_);
			// ick
			j.vr_.back().btg_ = score;
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
			if (r.ioc_ > ioc_threshold)
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
		if (r.btg_ > bg_threshold)
			out.emplace_back(r);
	}
}

unsigned collate_results_tg(std::vector<result_t> const& in, std::vector<result_t>& out)
{
	unsigned tg_max = 0;
	for (auto& r : in)
	{
		if (r.btg_ > tg_max)
			tg_max = r.btg_;
		if (r.btg_ > tg_threshold)
			out.emplace_back(r);
	}
	return tg_max;
}

int main(int ac, char** av)
{
#if 1
	if (ac < 3)
	{
		Help();
		return 0;
	}
	unsigned ees_threshold = ees_threshold_default;
	unsigned ees_threshold_end = ees_threshold_default + ees_threshold_range;
	int jobbegin = 0;
	int jobend = -1;
	if (ac > 3)
		ees_threshold = ::atoi(av[3]);
	if (ac > 4)
		ees_threshold_end = ::atoi(av[4]);
	if (ac > 5)
		jobbegin = ::atoi(av[5]);
	if (ac > 6)
		jobend = ::atoi(av[6]);

	if (jobend != -1 && jobend <= jobbegin)
	{
		std::cout << "Nonsensical values for job begin and/or end\n";
		return -1;
	}
#endif
	// this is where we collect the overall results!
	std::vector<result_t> vr_oall;
	try
	{
		std::cout << "arena version " << version << '\n';
		std::cout << "Accept scores between " << ees_threshold << " and " << ees_threshold_end << ", job range from " << jobbegin << " to " << jobend << ".\n\n";
		std::cout << "\nReady to read ciphertext\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		// B251 bcn UED "AO BV DS EX FT HZ IQ JW KU PR"
		std::cout << "Ciphertext is - ";
		for (auto c : ct)
			std::cout << c;
		std::cout << "\nInitialising search\n";
		using job_wheels_t = job_wheels<decltype(ct.cbegin())> ;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], jobbegin, jobend, std::begin(ct), std::end(ct));
		if (vjbw.empty())
		{
			std::cout << "No matching wheel and reflector arrangements found!\n";
			return -1;
		}
		std::cout << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";
		auto start = std::chrono::steady_clock::now();

		// work through the wheel orders linearly
		for (auto & j : vjbw)
		{
			auto start_wo = std::chrono::steady_clock::now();
			// search each wheel order in parallel
			do
			{
				std::cout << j.mst_ << " " << vr_oall.size() ;
				machine3 m3 = MakeMachine3(j.mst_);
				// fill the arena
				fill_arena_width(m3.Wheels(), arena, m3.CycleLength() + ct.size());
				// job list
				using job_arena_t = job_arena<arena_t, result_t, decltype(ct.cbegin())>;
				auto vjb = make_job_list_arena<job_arena_t>(j.mst_, arena, j.ctb_, j.cte_);
				// do the search for quite likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						match_search_exp(aj.ctb_, aj.cte_, aj.line_, arena.active_width_, aj.r_, aj.bs_);
					});
				// do the search for more likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [ees_threshold, ees_threshold_end](auto& aj)
					{
						collect_results(aj, ees_threshold, ees_threshold_end);
					});
				// do the search for most likely
				auto vr = collate_results_ioc(vjb);
				std::cout << " - considering " << vr.size() << " possibles.";
				std::for_each(std::execution::par, std::begin(vr), std::end(vr), [&ct](auto& r)
					{
#if 0
						hillclimb_base(std::begin(ct), std::end(ct), bigram_score_op(), r.mst_);
						r.btg_ = hillclimb_base(std::begin(ct), std::end(ct), trigram_score_op(), r.mst_);
#else
						r.btg_ = hillclimb_bgtg_fast(std::begin(ct), std::end(ct), r.mst_);
#endif
					});
				auto n = vr_oall.size();
				auto mx = collate_results_tg(vr, vr_oall);
				std::cout << " Max tg score = " << mx << ".\n";
				std::for_each(vr_oall.begin() + n, vr_oall.end(), [&ct](auto& r)
					{
						machine3 m3 = MakeMachine3(r.mst_);
						std::vector<modalpha> vo;
						vo.reserve(ct.size());
						decode(std::begin(ct), std::end(ct), m3, vo);
						// report
						std::cout << r.mst_ << " { " << r.ioc_ << ", " << r.btg_ << " } : ";
						for (auto c : vo)
							std::cout << c;
						std::cout << "\n";
					});
			} while (AdvanceRing(j.mst_));
			auto now = std::chrono::steady_clock::now();
			std::cout << "Wheel order search time: " << std::chrono::duration<double>(now - start_wo).count() << "s\n";
		}
		auto now = std::chrono::steady_clock::now();
		std::cout << "Finished, search time: " << std::chrono::duration<double>(now - start).count() << "s\n";
		// report
		for (auto r : vr_oall)
		{
			machine3 m3 = MakeMachine3(r.mst_);
			std::vector<modalpha> vo;
			vo.reserve(ct.size());
			decode(std::begin(ct), std::end(ct), m3, vo);
			// report
			std::cout << r.mst_ << " { " << r.ioc_ << ", " << r.btg_ << " } : ";
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
