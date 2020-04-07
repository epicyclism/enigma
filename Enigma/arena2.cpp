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
	std::cerr << "arena2 " << version << " : Enigma settings hunt, pass 1.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena2 BC 12345 [b] [e]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from\n";
	std::cerr << "reflectors BC and wheels 12345 and attempts to find wheels, ring and plug\n";
	std::cerr << "settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 'b' and 'e' values determine where in the order to\n";
	std::cerr << "start, and, if present where to end so that sessions can be interrupted,\n";
	std::cerr << "restarted or shared across systems.\n\n";
}

using arena_t = arena_base<26 * 26 * 26 + 512>;
// this can be singular since parallelism is within an arena rather than over multiple arenas.
// OTOH this means no more than 26 threads get used in this pass by this executable.
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
	std::array<unsigned, 100> percent_;
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

template<typename AJ> void collect_results(AJ const& aj, std::array<unsigned, 100>& out)
{

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
		std::cout << "\nReady\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		// B251 bcn UED "AO BV DS EX FT HZ IQ JW KU PR"
//		auto ct = make_alpha_array("UPONTXBBWFYAQNFLZTBHLBWXSOZUDCDYIZNRRHPPBNSV");
		std::cout << "Ciphertext is - ";
		for (auto c : ct)
			std::cout << c;
		std::cout << "\nInitialising search\n";
		using job_wheels_t = job_wheels<decltype(ct.cbegin())>;
		std::vector<job_wheels_t> vjbw = make_job_list<job_wheels_t>(av[1], av[2], jobbegin, jobend, std::begin(ct), std::end(ct));
		//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "123", std::begin(ct), std::end(ct));
		//		std::vector<job_wheels_t> vjbw = make_job_list_t<job_wheels_t>("B", "251", std::begin(ct), std::end(ct));

		std::cout << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";

		std::array<unsigned, 100> collect_;
		collect_.fill(0);

		// work through the wheel orders linearly
		for (auto& j : vjbw)
		{
			// search each wheel order in parallel
			do
			{
				std::cout << j.mst_ << "\n";
				machine3 m3 = MakeMachine3(j.mst_);
				// fill the arena
				fill_arena(m3.Wheels(), arena, 0);
				// job list
				using job_arena_t = job_arena2<arena_t, std::array<unsigned, 100>, decltype(ct.cbegin())>;
				auto vjb = make_job_list_arena<job_arena_t>(j.mst_, arena, j.ctb_, j.cte_);
				// do the search for quite likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						match_search(aj.ctb_, aj.cte_, aj.line_, aj.r_, aj.bs_);
						aj.vr_.fill(0);
						for (auto p : aj.r_)
							++aj.vr_[p];

					});
				// gather the stats
				std::for_each(std::begin(vjb), std::end(vjb), [&](auto& aj)
					{
						std::transform(aj.vr_.begin(), aj.vr_.end(), collect_.begin(), collect_.begin(), std::plus());
					});

			} while (AdvanceRingAll(j.mst_));
		}
		std::cout << "Finished\n";
		// report
		unsigned off = 0;
		for (auto p : collect_)
		{
			std::cout << off << ' ' << p << '\n';
			++off;
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
