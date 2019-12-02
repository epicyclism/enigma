#include <iostream>
#include <iomanip>
#include <vector>
#include <execution>
#include <algorithm>
#include <numeric>
#include <string>

#include "machine.h"
#include "arena.h"
#include "match.h"

// B423 gto SDV "AD EH GY IM KN LR OZ QV TX UW"
// FDZCJJDKVWPYFDWPOQZGTJQYYXAFRHSQESERKGJBWBYPEOOKFMMPOMKQDDOLCPKHYPGUZYXBZYANYSAXIPXVQCPJBFFFDRDXFIJJPPPEYALCYKVLKXQHWIRZANGWUJBWVJYCKESMJQRYKQHCQKMMYWMCKVLZJDVZXRUMRMNWFDZBQGXJQAPFFFZTAHJQZPWQWNIVZWUIJTHOYXGDCOJUW

// B425 agm QAY "DM EP FL HI JR KY NQ OU SW TZ"
// QKXETVPZQOHSXMBIZPHTCTRMAUZYSTJIMDUYOZBFRTZOUHBGOROUVRQEJRDRJHZPZIBQQHKMMJZCIIRCUOLXLCIOQKHRLIGGFJFTLLGDRARDZQUQKLTKXXXYKRUVFULBQLAYRZVJFULCGQJXFJURMURSELYFVFOKUHYUHSYLOMEFYAIIP

constexpr  char version[] = "v0.01";

// version 1, use the highest 10 as an estimate.
//
template<typename IC, typename IA>
unsigned match_worker1(IC ctb, IC cte, IA base, modalpha bs)
{
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);
	psm.trim();

//	return std::accumulate(psm.begin(), psm.end(), 0, [](auto& l, auto& r) { return l + r.cnt_; }) ;
	return std::accumulate(psm.begin(), psm.begin() + 10, 0, [](auto& l, auto& r) { return l + r.cnt_; }) ;
}

template<typename I, size_t W> void ees_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts, modalpha E)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
	while (itb != ite)
	{
		*ito = match_worker1(cb, ce, itb, E);
		++ito;
		++itb;
	}
}

template<size_t W> void report_results(std::array<unsigned, W> const& cnts)
{
	std::array<unsigned, 64> hist{ 0 };
	unsigned mx = 0;
	for (auto cnt : cnts)
	{
		if (cnt > mx)
			mx = cnt;
		++hist[cnt];
	}
	for(int n = 0; n < mx + 1; ++n)
		std::cout << n << ' ' << hist[n] << '\n';
}

void Help()
{
	std::cerr << "linearE " << version << " : searches for the ciphertext supplied on stdin in the enigma machines configuration supplied.\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./linearE B123 fvn [E]\n\n";
	std::cerr << "Configures a 'machine' with rotors B123 and ring setting fvn\n";
	std::cerr << "then generates an entire output cycle given the input character, defaulting to 'E'.\n";
	std::cerr << "Then when given a cipher text, for example\n";
	std::cerr << "BYHSQTPUWDCMXYGQWMTZZMPNTUFSVGASNAXGLHFHAOVT\n";
	std::cerr << "the best match positions are evaluated, on the basis of guessing plug board links to make 'E' match.\n";
	std::cerr << "An alternate input character is treated as a plug from E to that character.\n";
	std::cerr << "(It seems that a 'manual' entry of the ciphertext on Windows can be terminated with \'Enter Ctrl-Z Enter\')\n";
	std::cerr << "\nCopyright (c) 2018, paul@epicyclism.com. Free to a good home.\n\n";
}

// an entire enigma cycle plus a reasonable max message length.
//
using line_t = line_base<26 * 26 * 26 + 256>;
// global to avoid allocation.
line_t l;
line_t::results_t r;

int main(int ac, char**av)
{
	if (ac < 3)
	{
		Help();
		return 0;
	}
	try
	{
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);

		modalpha testch = alpha::E;
		if (ac > 3)
		{
			if (valid_from_char(av[3][0]))
				testch = from_printable(av[3][0]);
			else
			{
				std::cerr << "Invalid test character <" << av[3] << ">\n\n";
				Help();
				return 0;
			}
		}
		std::cout << "Ready - \n";
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
		std::cout << "\nInitialising search\n";
		fill_line(m3, l, testch);
		std::cout << "Searching...\n";
		r.fill(0);
		ees_search(std::begin(ct), std::end(ct), l.ln_, r, testch);
		report_results(r);
		std::cout << "\nFinished\n";
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
