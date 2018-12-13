// linear.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <vector>
#if !defined ( __GNUC__ )
#include <execution>
#endif
#include <algorithm>
#include <numeric>
#if defined ( __GNUC__ )
#include <pstl/execution>
#include <pstl/algorithm>
#include <pstl/numeric>
#endif
#include <string>
#include "machine.h"
#include "arena.h"
#include "ioc.h"
#include "jobs.h"

constexpr  char version[] = "v0.06";

template<typename I, typename L, typename R> void report_results(machine_settings_t const& mst, I cb, I ce, L const& a, R& r)
{
	auto sz = std::distance(cb, ce);
	auto itp = std::begin(a.pos_);
	auto threshold = sz / 11;
	auto max_ioc {0.0};
	auto rb = std::begin(r);

	while ( rb != std::end(r))
	{
		if (*rb > threshold) // decode!
		{
			machine3 m3 = MakeMachine3(mst);
			m3.Position(*(itp + std::distance(std::begin(r), rb)));
			std::vector<modalpha> vo;
			vo.reserve(sz);
			auto cbc = cb;
			while (cbc != ce)
			{
				vo.push_back(m3.Transform(*cbc));
				++cbc;
			}
			auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
			if (ioc > max_ioc)
				max_ioc = ioc;
			if (ioc > 0.049)
			{
				std::cout << *rb << " - ";
				m3.ReportSettings(std::cout);
				std::cout << " - ";
				std::cout << ioc << " - ";
				for (auto c : vo)
					std::cout << c;
				std::cout << "\n";
			}
		}
		++rb;
	}
}

template<typename I> void report_result(result_ioc_t const& r, I cb, I ce)
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

void Help()
{
	std::cerr << "linearM " << version << " : searches for the ciphertext supplied on stdin in the output of the set of enigma machines defined by\n";
	std::cerr << "a collection of rotors and reflectors.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./linearM BC 12345 fvn \"AH BO CG DP FL JQ KS MU TZ WY\"\n\n";
	std::cerr << "Configures a 'machine' with all permutations of reflectors B and C, rotors 1, 2, 3, 4, 5, ring setting fvn\n";
	std::cerr << "and plug board as indicated. The plug settings can optionally be condensed and the quotes omitted.\n";
	std::cerr << "Then when given\n";
	std::cerr << "BYHSQTPUWDCMXYGQWMTZZMPNTUFSVGASNAXGLHFHAOVT\n";
	std::cerr << "the original message,\n";
	std::cerr << "DERFUEHRERISTTODXDERKAMPFGEHTWEITERXDOENITZX\n";
	std::cerr << "will be produced somewhere in a list of best fits by brute force and magic.\n";
	std::cerr << "(It seems that a 'manual' entry of the ciphertext on Windows can be terminated with \'Enter Ctrl-Z Enter\')\n";
	std::cerr << "\nCopyright (c) 2018, paul@epicyclism.com. Free to a good home.\n\n";
}

// an entire enigma cycle plus a reasonable max message length.
//
using line_t = line_base<26 * 26 * 26 + 256>;

// wrap a machine description, a work space and a results space
// so it can be handed off to a function for processing.
//
template<typename CI> struct job_base
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;

	line_t l_;
	line_t::results_t r_;
	job_base(machine_settings_t const& mst, CI ctb, CI cte) : mst_(mst), ctb_(ctb), cte_(cte)
	{
		r_.fill(0);
	}
};

using job = job_base< std::vector<modalpha>::const_iterator>;

template<typename J, typename R> void collect_results(J const& j, R& r)
{
	auto sz = std::distance(j.ctb_, j.cte_);
	auto itp = std::begin(j.l_.pos_);
	auto threshold = sz / 10;

	auto rb = std::begin(j.r_);
	while (rb != std::end(j.r_))
	{
		if (*rb > threshold) // decode!
		{
			machine3 m3 = MakeMachine3(j.mst_);
			m3.Position(*(itp + std::distance(std::begin(j.r_), rb)));
			std::vector<modalpha> vo;
			vo.reserve(sz);
			auto cbc = j.ctb_;
			while (cbc != j.cte_)
			{
				vo.push_back(m3.Transform(*cbc));
				++cbc;
			}
			auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
			if (ioc > 0.052)
			{
				// put the machine back...
				m3.Position(*(itp + std::distance(std::begin(j.r_), rb)));
				// record
				r.emplace_back( m3.machine_settings(), ioc );
			}
		}
		++rb;
	}
}

template <typename JOB> void linear_search(JOB& jb, modalpha ch)
{
	machine3 m3 = MakeMachine3(jb.mst_);
	fill_line(m3, jb.l_, ch);
	linear_search(jb.ctb_, jb.cte_, jb.l_.ln_, jb.r_);
}

int main(int ac, char**av)
{
	if (ac < 4)
	{
		Help();
		return 0;
	}
	try
	{
		// validate the arguments (these throw on problems)
		check_reflectors(av[1]);
		check_wheels(av[2]);
		int stecker_arg = 3;
		bool simple = false;
		if (ac > 4)
		{
			check_ring(av[3]);
			stecker_arg = 4;
			simple = true;
		}
		// capture the ciphertext
		std::vector<modalpha> ct;
		while (1)
		{
			char c;
			std::cin >> c;
			if (!std::cin)
				break;
			if (valid_from_char(c))
			{
				ct.push_back(from_printable(c));
			}
		}
		std::cout << "Message length is " << ct.size() << " characters.\n";
		// make the job list
		auto vjb = make_job_list<job>(av[1], av[2], 0, std::begin(ct), std::end(ct));
		if (simple)
		{
			std::for_each(std::begin(vjb), std::end(vjb), [&av, stecker_arg](auto& j)
			{
				Stecker(j.mst_, av[stecker_arg]);
				Ring(j.mst_, av[3]);
			});
		}
		std::cout << "\nSearching\n";
		// just used as a counter
		machine_settings_t mst = vjb[0].mst_;
		if (simple)
			std::cout << mst << "\n";
		// collect results here
		std::vector<result_ioc_t> results;
		while (1)
		{
			std::cout << to_printable_lower(mst.r3_) << to_printable_lower(mst.r2_) << to_printable_lower(mst.r1_) ;
			std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& j) { linear_search(j, alpha::E); });
			for (auto& r : vjb)
			{
				collect_results(r, results);
			}
			if (simple)
				break;
			if (!AdvanceRing(mst))
				break;
			std::for_each(std::begin(vjb), std::end(vjb), [](auto& j) { AdvanceRing(j.mst_); });
			std::cout << " " << results.size() << "\n";
		}
		std::cout << "\nFinished\n";
		// report results
		for (auto& res : results)
		{
			report_result(res, std::begin(ct), std::end(ct));
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
