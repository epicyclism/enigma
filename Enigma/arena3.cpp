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

constexpr  char version[] = "v0.01";

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
	std::cerr << "arena3 " << version << " : Enigma settings hunt.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena3 BC 12345 [t [tr [b [e]]]]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 't' is a value to use as the first pass threshold. Default is 50.\n";
	std::cerr << "The further optional trailing 'tr' is the range of the first pass threshold. Default is infinite.\n";
	std::cerr << "The additional further optional trailing 'b' and 'e' values determine where in the order to\n";
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
constexpr unsigned ees_threshold_default = 50;   // greater than this
constexpr unsigned ees_threshold_range   = 1000000; // range to end of threshold window
constexpr double   ioc_threshold = 0.045; // irrelevant?
constexpr unsigned tg_threshold = 14000; // trigram

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

// a results type for the score process
//
struct result_t
{
	machine_settings_t mst_;
	modalpha bs_;

	explicit result_t(machine_settings_t const& mst, position const& pos, modalpha bs) : mst_(mst), bs_(bs)
	{
		mst_.pos_ = pos;
	}
};

// condense the lists of above into thie, with the set of 'bs' to work on for each relevant position.
//
struct result_bs_t
{
	machine_settings_t	mst_;
	unsigned			scr_ = 0;
	int ees_cnt_ = 0;
	std::array<modalpha, alpha_max> ees_;
	void push_ee(modalpha bs)
	{
		ees_[ees_cnt_] = bs;
		++ees_cnt_;
	}
	void concat_ees(result_bs_t const& r)
	{
		for (int ee = 0; ee < r.ees_cnt_; ++ee)
			push_ee(r.ees_[ee]);
	}
	result_bs_t() = default;
	explicit result_bs_t(machine_settings_t const& mst, modalpha bs) : mst_(mst)
	{
		scr_ = 0;
		ees_cnt_ = 0;
		push_ee(bs);
	}
};

// a results type for the hillclimb process
//
struct result_out_t
{
	machine_settings_t mst_;
	unsigned scr_;

	explicit result_out_t(machine_settings_t const& mst, unsigned scr) : mst_(mst), scr_(scr)
	{
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
		if ((score > ees_thld) && (score < ees_thld_end)) // look at
		{
			auto off = std::distance(std::begin(j.r_), rb);
			j.vr_.emplace_back(j.mst_, *(itp + off), j.bs_);
		}
		++rb;
	}
}

template<typename J> std::vector<result_bs_t> collate_results(std::vector<J> const& vj)
{
	std::vector<result_bs_t> vr;
	for (auto const& j : vj)
	{
		for (auto const& r : j.vr_)
		{
			vr.emplace_back(r.mst_, r.bs_);
		}
	}
	// sort 
	std::sort(std::execution::par, vr.begin(), vr.end(), [](auto const& l, auto const& r) { return l.mst_.pos_ < r.mst_.pos_; });
	// and condense
	auto end = std::unique(std::execution::par, vr.begin(), vr.end(), [](auto& l, auto const& r) { if (l.mst_.pos_ == r.mst_.pos_)
																											{
																												l.concat_ees(r);
																												return true;
																											}
																											return false;});
	vr.erase(end, vr.end());
	return vr;
}

unsigned collate_results_tg(std::vector<result_bs_t> const& in, std::vector<result_out_t>& out)
{
	unsigned tg_max = 0;
	for (auto& r : in)
	{
		if (r.scr_ > tg_max)
			tg_max = r.scr_;
		if (r.scr_ > tg_threshold)
			out.emplace_back(r.mst_, r.scr_);
	}
	return tg_max;
}

#if 0
template<typename I> void report_result(result_out_t const& r, I cb, I ce)
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
	std::cout << r.scr_ << " - ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";
}
#endif

int main(int ac, char** av)
{
	if (ac < 3)
	{
		Help();
		return 0;
	}
	unsigned ees_threshold     = ees_threshold_default;
	unsigned ees_threshold_end = ees_threshold_default + ees_threshold_range;
	int jobbegin = 0;
	int jobend = -1;
	if (ac > 3)
		ees_threshold = ::atoi(av[3]);
	if (ac > 4)
		ees_threshold_end = ees_threshold + ::atoi(av[4]);
	if (ac > 5)
		jobbegin = ::atoi(av[5]);
	if (ac > 6)
		jobend = ::atoi(av[6]);

	if (jobend != -1 && jobend <= jobbegin)
	{
		std::cout << "Nonsensical values for job begin and/or end\n";
		return -1;
	}
	// this is where we collect the overall results!
	std::vector<result_out_t> vr_oall;
	try
	{
		std::cout << "arena version " << version << '\n';
		std::cout << "Accept scores between " << ees_threshold << " and " << ees_threshold_end << ", job range from " << jobbegin << " to " << jobend << ".\n\n";
		std::cout << "\nReady to read ciphertext\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is - ";
		for (auto c : ct)
			std::cout << c;
		std::cout << "\nInitialising search\n";
		using job_wheels_t = job_wheels<decltype(ct.cbegin())>;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], jobbegin, jobend, std::begin(ct), std::end(ct));
		if (vjbw.empty())
		{
			std::cout << "No matching wheel and reflector arrangements found!\n";
			return -1;
		}
		std::cout << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";
		auto start = std::chrono::steady_clock::now();

		// work through the wheel orders linearly
		for (auto& j : vjbw)
		{
			auto start_wo = std::chrono::steady_clock::now();
			// search each wheel order in parallel
			do
			{
				std::cout << j.mst_ << " " << vr_oall.size();
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
				auto vr = collate_results(vjb);
				std::cout << " - considering " << vr.size() << " possibles.";
				std::for_each(std::execution::par, std::begin(vr), std::end(vr), [&ct](auto& r)
					{
						r.scr_ = hillclimb_partial_exhaust_fast(std::begin(ct), std::end(ct), r.ees_.begin(), r.ees_.begin() + r.ees_cnt_, trigram_score_op(), r.mst_);
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
						std::cout << r.mst_ << " { " << r.scr_ << " } : ";
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
			std::cout << r.mst_ << " { " << r.scr_ << " } : ";
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
