// arena4.cpp : final cut
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
#include "utility.h"

constexpr  char version[] = "v0.04";

constexpr unsigned tg_threshold = 16000; // trigram

void Help()
{
	std::cerr << "arena4 " << version << " : Enigma settings hunt.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena4 BC 12345 [b [e]]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 'b' and 'e' values determine where in the set of wheel orders to\n";
	std::cerr << "start, and optionally, to end so that sessions can be interrupted, restarted or shared across systems.\n\n";
}

template<typename CI> struct job_position
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;
	unsigned off_;
	unsigned scr_;

	job_position(machine_settings_t const& mst, unsigned off, CI ctb, CI cte)
		: mst_(mst), ctb_(ctb), cte_(cte), off_(off), scr_(0)
	{
	}
};

template<typename J, typename A, typename CI> auto make_job_list_position(machine_settings_t mst, A& a, CI ctb, CI cte) -> std::vector<J>
{
	std::vector<J> vjb;

	auto ctl = std::distance(ctb, cte);
	bool bend = false;
	auto w3 = MakeWheels3(mst);
	unsigned off = 0;
	auto ap = a.arena_.begin();
	do
	{
		mst.pos_ = w3.Position();
		vjb.emplace_back(mst, off, ctb, cte);
		w3.Step();
		auto col = w3.Evaluate(alphabet);
		std::copy(col.begin(), col.end(), (*ap).begin());
		++ap;
		++off;
		if (w3.Position() == position())
			bend = true;
	} while (!bend || --ctl);
	return vjb;
}

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

template<typename R> unsigned collate_results_tg(R const& in, std::vector<result_out_t>& out)
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

template<typename CI, typename SCR> void report_result(machine_settings_t const& mst, SCR scr, CI ctb, CI cte)
{
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	decode(ctb, cte, m3, vo);
	// report
	std::cout << mst << " { " << scr << " } : ";
	report_ciphertext(vo, std::cout);
}

// an arena for bulk fast decode, shouldn't be slower than calulating per position, should be quicker because each
// position only calculated once!
//
arena_simple<26 * 26 * 26 + 250> arena_;

int main(int ac, char** av)
{
	if (ac < 3)
	{
		Help();
		return 0;
	}
	int jobbegin = 0;
	int jobend = -1;
	if (ac > 3)
		jobbegin = ::atoi(av[3]);
	if (ac > 4)
		jobend = ::atoi(av[4]);
	if (jobend != -1 && jobend <= jobbegin)
	{
		std::cout << "Nonsensical values for job begin and/or end\n";
		return -1;
	}
	// this is where we collect the overall results!
	std::vector<result_out_t> vr_oall;
	try
	{
		std::cout << "arena4 version " << version << '\n';
		std::cout << "\nReady to read ciphertext\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is - ";
		report_ciphertext(ct, std::cout);
		std::cout << "\nInitialising search\n";
		using job_wheels_t = job_wheels_ct<decltype(ct.cbegin())>;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], jobbegin, jobend, std::begin(ct), std::end(ct));
		if (vjbw.empty())
		{
			std::cout << "No matching wheel and reflector arrangements found!\n";
			return -1;
		}
		// process the ciphertext for worthwhile plugs
		auto tab = gen_freq_seq(ct.begin(), ct.end());
		auto plugsets = make_plug_list2(std::begin(tab), std::begin(tab) + 13);

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
				using job_position_t = job_position<decltype(ct.cbegin())>;
				// fills the arena for this wheel order
				auto vjb = make_job_list_position<job_position_t>(j.mst_, arena_, j.ctb_, j.cte_);
				auto pa = arena_.arena_.begin();
				// do the search for quite likely
				std::cout << " - considering " << vjb.size() << " possibles.\n";
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [&plugsets, &pa](auto& aj)
					{
#if 1
#if 1
						aj.scr_ = hillclimb_partial_exhaust3_fast(aj.ctb_, aj.cte_, trigram_score_op(), alpha::E, alpha::N, alpha::S, aj.mst_);
#else
						aj.scr_ = hillclimb_specific_exhaust_fast(aj.ctb_, aj.cte_, std::begin(plugsets), std::end(plugsets), trigram_score_op(), pa + aj.off_, aj.mst_);
#endif
#else
#if 0
						aj.scr_ = hillclimb_partial_exhaust_all(aj.ctb_, aj.cte_, trigram_score_op(), aj.mst_);
#else
						aj.scr_ = hillclimb_partial_exhaust2_fast(aj.ctb_, aj.cte_, trigram_score_op(), alpha::E, alpha::S, aj.mst_);
#endif
#endif
					});
				auto n = vr_oall.size();
				auto mx = collate_results_tg(vjb, vr_oall);
				std::cout << " Max tg score = " << mx << ".\n";
				std::for_each(vr_oall.begin() + n, vr_oall.end(), [&ct](auto& r)
					{
						report_result(r.mst_, r.scr_, std::begin(ct), std::end(ct));
					});
			} while (AdvanceRing(j.mst_));
			auto now = std::chrono::steady_clock::now();
			std::cout << "Wheel order search time: " << std::chrono::duration<double>(now - start_wo).count() << "s\n";
		}
		auto now = std::chrono::steady_clock::now();
		std::cout << "Finished, search time: " << std::chrono::duration<double>(now - start).count() << "s\n";
		// sort in best->worst!
		std::sort(std::begin(vr_oall), std::end(vr_oall), [](auto const& l, auto const& r) { return l.scr_ == r.scr_; });
		// report
		std::for_each(vr_oall.begin(), vr_oall.end(), [&ct](auto& r)
			{
				report_result(r.mst_, r.scr_, std::begin(ct), std::end(ct));
			});
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
