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
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 'b' and 'e' values determine where in the order to start, and, if\b";
	std::cerr << "present where to end so that sessions can be interrupted, restarted and shared across systems.\n\n";
}

using arena_t = arena_base<26 * 26 * 26 + 512>;
// this can be singular since parallelism is within an arena rather than over multiple arenas.
// OTOH this means no more than 26 threads get used in this pass by this executable.
//
arena_t arena;

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
				using job_arena_t = job_arena<arena_t, result_t, decltype(ct.cbegin())>;
				auto vjb = make_job_list_arena<job_arena_t>(j.mst_, arena, j.ctb_, j.cte_);
				// do the search for quite likely
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& aj)
					{
						match_search(aj.ctb_, aj.cte_, aj.line_, aj.r_, aj.bs_);
					});
			} while (AdvanceRingAll(j.mst_));
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
