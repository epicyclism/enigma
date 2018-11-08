// linear.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <execution>
#include <algorithm>
#include <numeric>
#include <string>
#include "machine.h"
#include "arena.h"

constexpr  char version[] = "v0.01";

// this is a bit crap. There must be a more elegant way to do this
// In essence we want the N best results, in order of bestness
//
template<typename I> std::vector<I> best_of(I rb, I re)
{
	std::vector<I> rv;
	rv.push_back(rb);
	++rb;
	while (rb != re)
	{
		if ( *rb > *rv[0])
		{
			// rv[0] is the littlest in the array. so replace it.
			rv.push_back(rb); 
			// and put the replacement into place.
			std::sort(std::begin(rv), std::end(rv), [](auto l, auto r) { return *l < *r; });
			if (rv.size() > 16)
				rv.erase(rv.begin());
		}
		++rb;
	}
	// now prune to greater than 80% of the best
	auto threshold = (*rv.back() * 6) / 10;
	rv.erase(std::remove_if(rv.begin(), rv.end(), [threshold](auto i) { return *i < threshold; }), rv.end());

	return rv;
}

template<typename I, typename L, typename R> void report_results(I cb, I ce, L const& a, R& r, machine3& m3)
{
	auto sz = std::distance(cb, ce);
	auto itp = std::begin(a.pos_);

	auto rv = best_of(std::begin(r), std::end(r) - sz);
	for (auto hit : rv)
	{
		std::cout << *hit << " - ";
		// decode it!
		m3.Position(*(itp + std::distance(std::begin(r), hit)));
		m3.ReportSettings(std::cout);
		auto cbc = cb;
		while (cbc != ce)
		{
			std::cout << m3.Transform(*cbc);
			++cbc;
		}
		std::cout << "\n";
	}
}

void Help()
{
	std::cerr << "linearM " << version << " : searches for the ciphertext supplied on stdin in the output of the set of enigma machines defined by\n";
	std::cerr << "a collection of rotors and reflectors.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./linear BC 12345 fvn \"AH BO CG DP FL JQ KS MU TZ WY\"\n\n";
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
using line_t = line_base<26 * 26 * 26 + 250>;

// global to avoid allocation.
line_t l;
line_t::results_t r;

// wrap a machine description, a work space and a results space
// so it can be handed off to a function for processing.
//
template<typename CI> struct job
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;

	line_t l_;
	line_t::results_t r_;
	job(machine_settings_t const& mst, CI ctb, CI cte) : mst_(mst), ctb_(ctb), cte_(cte)
	{}
};

int factorial(int n)
{
	int f = 1;
	while (n > 1)
	{
		f *= n;
		--n;
	}
	return f;
}

template <typename JOB> void linear_search(JOB& jb)
{
	machine3 m3 = MakeMachine3(jb.mst_);
	fill_line(m3, jb.l_, alpha::E);
	linear_search(jb.ctb_, jb.cte_, jb.l_.ln_, jb.r_);
}

int main(int ac, char**av)
{
	if (ac < 5)
	{
		Help();
		return 0;
	}
	try
	{
		// reflectors
		std::string ref(av[1]);
		for (auto r : ref)
		{
			if (!valid_reflector_id(r))
			{
				std::cerr << "Invalid reflector " << r << "\n\n";
				return -1;
			}
		}
		// wheels
		std::string whl(av[2]);
		if (whl.size() < 3)
		{
			std::cerr << "Insufficient wheels specified for a 3 rotor machine\n\n";
			return -1;
		}
		for (auto r : whl)
		{
			if (!valid_rotor_id(r))
			{
				std::cerr << "Invalid wheel " << r << "\n\n";
				return -1;
			}
		}
		std::sort(std::begin(ref), std::end(ref)); // overkill....
		std::sort(std::begin(whl), std::end(whl));

		std::cout << "\nReady\n";
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
		machine_settings_t mst;
		// load the invariants for this purpose
		Ring(mst, av[3]);
		Stecker(mst, av[4]);

		std::cout << "\nSearching\n";
		std::vector < job< std::vector<modalpha>::const_iterator>> vjb;
		int skip = factorial(whl.size() - 3);
		int cnt = skip;
		do
		{
			do
			{
				--cnt; // take out the permutations out of our sight and avoid duplication.
				if (cnt == 0)
				{
					mst.ref_ = ref[0];
					mst.w3_ = whl[0];
					mst.w2_ = whl[1];
					mst.w1_ = whl[2];
					vjb.emplace_back(mst, std::begin(ct), std::end(ct));
					cnt = skip;
				}
			} while (std::next_permutation(std::begin(whl), std::end(whl)));
		} while (std::next_permutation(std::begin(ref), std::end(ref)));
#if 0
		for (auto& r : vjb)
		{
			std::cout << r.mst_ << "\n";
		}
#endif
		std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [](auto& j) { linear_search(j); });
		for (auto& r : vjb)
		{
			std::cout << r.mst_ << " - " << *std::max_element(std::begin(r.r_), std::end(r.r_) - ct.size()) << "\n";
		}
#if 0
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);
		m3.Setting(alpha::A, alpha::A, alpha::A);
		Stecker(m3, av[3]);
		std::cout << "linear " << version << " configured : ";
		m3.ReportSettings(std::cout);
		modalpha E = alpha::E;
		if (ac == 5)
			E = from_printable(av[4][0]);
		fill_line(m3, l, E);
		std::cout << "Searching...\n";
		linear_search(std::begin(ct), std::end(ct), l.ln_, r);
		report_results(std::begin(ct), std::end(ct), l, r, m3);
#endif
		std::cout << "\nFinished\n";
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
