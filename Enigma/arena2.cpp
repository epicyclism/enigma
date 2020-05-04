#include <iostream>
#include <iomanip>
#include <execution>
#include <algorithm>
#include <numeric>
#include "wheelset.h"
#include "machine.h"
#include "position.h"
#include "ioc.h"
#include "bigram.h"
#include "match.h"
#include "arena.h"
#include "jobs.h"

constexpr  char version[] = "v0.05";

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
	std::cerr << "arena2 " << version << " : Enigma settings hunt, pass 1.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena2 BC 12345 [b] [e]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from\n";
	std::cerr << "reflectors BC and wheels 12345 and attempts to find interesting information\n";
	std::cerr << "about the possibilities.\n";
	std::cerr << "The optional trailing 'b' and 'e' values determine where in the order to\n";
	std::cerr << "start, and, if present where to end so that sessions can be interrupted,\n";
	std::cerr << "restarted or shared across systems.\n";
	std::cerr << "Reports progress to cerr, statistics to cout. Try gnuplot.\n\n";
}

// assume worst case 256 char message at ZZZ.
// adjust this if you have a really long one...
//
using arena_t = arena_base<26 * 26 * 26 + 256>;
// this can be singular since parallelism is within an arena rather than over multiple arenas.
//
arena_t arena;

template<typename A, typename R, typename CI> struct job_arena2
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;
	typename A::line_t     const& line_;
	typename A::position_t const& pos_;
	typename A::results_t& r_;
	modalpha       const  bs_;
	R				      vr_;

	job_arena2(machine_settings_t const& mst, CI ctb, CI cte, typename A::line_t const& l, typename A::position_t const& pos, typename A::results_t& r, modalpha bs)
		: mst_(mst), ctb_(ctb), cte_(cte), line_(l), pos_(pos), r_(r), bs_(bs)
	{
	}
};

// a stats results type
//
struct stats_results_t
{
	std::array<unsigned, 1000> percent_;
	template<typename R> stats_results_t(R const& r)
	{
		percent_.fill(0);
		for (auto p : r)
			++percent_[p];
	}
};

// to do results type
//
struct a2_result_t_todo
{
	machine_settings_t mst_;
	unsigned mtch_;

	explicit a2_result_t_todo(machine_settings_t const& mst, plug_set_msg const& psm) : mst_(mst)
	{
		mtch_ = std::accumulate(psm.begin(), psm.begin() + 10, 0, [](auto& l, auto& r) { return l + r.cnt_; }) ;
	}
};

template<typename A> void print_collection(A& a)
{
	// find end
	auto er = std::find_if(std::rbegin(a), std::rend(a), [](auto v) { return v != 0; });
	if (er == std::rend(a))
		return; // nothing to print
	auto e = std::begin(a) + std::size(a) - std::distance(std::rbegin(a), er);
	unsigned cnt = 0;
	unsigned sum = 0;
	std::for_each(std::begin(a), e, [&cnt, &sum](auto val)
		{
			std::cout << std::setw(2) << cnt << std::setw(10) << val << std::setw(10) << sum << '\n';
			++cnt;
			sum += val;
		}
	);
	std::cout << "\n\n";
}

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

	try
	{
		std::cerr << "\nReady\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		// B251 bcn UED "AO BV DS EX FT HZ IQ JW KU PR"
//		auto ct = make_alpha_array("UPONTXBBWFYAQNFLZTBHLBWXSOZUDCDYIZNRRHPPBNSV");
		std::cerr << "Ciphertext is - ";
		for (auto c : ct)
			std::cerr << c;
		std::cerr << "\nInitialising search\n";
		using job_wheels_t = job_wheels<decltype(ct.cbegin())>;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], jobbegin, jobend, std::begin(ct), std::end(ct));
		//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "123", std::begin(ct), std::end(ct));
		//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "251", std::begin(ct), std::end(ct));

		std::cerr << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";
#if 0
		{
			unsigned cnt = 0;
			for (auto const& j : vjbw)
			{
				std::cerr << std::setw(3) << cnt << " " << j.mst_ << '\n';
					++cnt;
			}
		}
#endif
		std::array<unsigned, 1000> collect_;
		collect_.fill(0);
		std::array<unsigned, 1000> wheel_;

		// work through the wheel orders linearly
		for (auto& j : vjbw)
		{
			wheel_.fill(0);
			// search each wheel order in parallel
			do
			{
				std::cerr << j.mst_ << "\n";
				machine3 m3 = MakeMachine3(j.mst_);
				// fill the arena
				fill_arena_width(m3.Wheels(), arena, m3.CycleLength() + ct.size());
				// job list
				using job_arena_t = job_arena2<arena_t, std::array<unsigned, 1000>, decltype(ct.cbegin())>;
				auto vjb = make_job_list_arena<job_arena_t>(j.mst_, arena, j.ctb_, j.cte_);
				// do the search for quite likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						match_search_exp(aj.ctb_, aj.cte_, aj.line_, arena.active_width_, aj.r_, aj.bs_);
						aj.vr_.fill(0);
						for (auto p : aj.r_)
							++aj.vr_[p];
						// the number of actual results is less than the size of the result vector because
						// of the length of the ciphertext, so we collect a bunch of '0' results which
						// don't need to be counted. Blanking them out seems most efficient.
						aj.vr_[0] = 0;
					});
				// gather the stats
				std::for_each(std::begin(vjb), std::end(vjb), [&](auto& aj)
					{
						std::transform(aj.vr_.begin(), aj.vr_.end(), wheel_.begin(), wheel_.begin(), std::plus());
					});

			} while (AdvanceRing(j.mst_));
			// report wheel
			std::cout << "# " << j.mst_.ref_ << j.mst_.w3_ << j.mst_.w2_ << j.mst_.w1_ << '\n';
			print_collection(wheel_);
			// accumulate totals
			std::transform(collect_.begin(), collect_.end(), wheel_.begin(), collect_.begin(), std::plus());
		}
		std::cerr << "Finished\n";
		// report
		// average
		std::transform(collect_.begin(), collect_.end(), collect_.begin(), [&vjbw](auto v) { return v / static_cast<unsigned>(vjbw.size()); });
		// label
		std::cout << "# " << av[1] << av[2] << '\n';
		print_collection(collect_);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
