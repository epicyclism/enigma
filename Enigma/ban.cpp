#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string_view>

#include "modalpha.h"
#include "utility.h"

constexpr std::string_view version{ "0.01" };

struct matcher
{
	std::vector<modalpha> const& ct1_;
	std::vector<modalpha> const& ct2_;
	std::vector<modalpha> vct1_;
	matcher(std::vector<modalpha> const& ct1, std::vector<modalpha> const& ct2) : ct1_(ct1), ct2_(ct2), vct1_(ct1.size() + 2 * ct2.size())
	{
		std::fill(vct1_.begin(), vct1_.end(), alpha::SZ);
		std::copy(ct1.begin(), ct1.end(), vct1_.begin() + ct2.size());
	}
	[[nodiscard]] std::vector<short> do_match() const noexcept
	{
		// results storage
		std::vector<short> rv(vct1_.size());

		// all the parameters
		auto b1 = vct1_.begin();
		auto e1 = b1 + ct2_.size();
		auto o = rv.begin();

		// do the matching
		while (e1 != vct1_.end())
		{
			*o = std::transform_reduce(b1, e1, ct2_.begin(), 0, std::plus<>(), std::equal_to());
			++o;
			++b1;
			++e1;
		}
		return rv;
	}
	[[nodiscard]] short match_at(int off) const noexcept
	{
		if (off + ct2_.size() >= vct1_.size())
			return 0;
		auto b1 = vct1_.begin() + off;
		auto e1 = b1 + ct2_.size();

		return std::transform_reduce(b1, e1, ct2_.begin(), 0, std::plus<>(), std::equal_to());
	}
	[[nodiscard]] double ban_at(int off) const noexcept
	{
		if (off + ct2_.size() >= vct1_.size())
			return 0;
		auto b1 = vct1_.begin() + off;
		auto e1 = b1 + ct2_.size();

		return std::transform_reduce(b1, e1, ct2_.begin(), 0.0, std::plus<>(), [](auto l, auto r) { if (l == r) return 1.53; return -0.979; });
	}
	void print_at(int off, std::ostream& ostr) const noexcept
	{
		report_ciphertext(vct1_, ostr);
		std::string sps(off, ' ');
		ostr << sps;
		report_ciphertext(ct2_, ostr);
	}
};

void usage()
{
	std::cerr << "Usage : ban <msg1> <msg2>\n";
	std::cerr << "Where msg1 and msg2 are files containing Enigma messages, invalid characters are ignored.\n";
	std::cerr << "\nSlides one message over the other and reports matches.\n";
}

int main(int ac, char** av)
{
	std::cout << "ban v" << version << '\n';
	if (ac < 3)
	{
		usage();
		return 0;
	}
	// open the files
	std::ifstream if1(av[1]);
	if (!if1)
	{
		std::cerr << "First ciphertext from <" << av[1] << "> not found.\n";
		usage();
		return 0;
	}
	std::ifstream if2(av[2]);
	if (!if2)
	{
		std::cerr << "Second ciphertext from <" << av[2] << "> not found.\n";
		usage();
		return 0;
	}
	// read the texts
	auto ct1 = read_ciphertext(if1);
	auto ct2 = read_ciphertext(if2);
	// report
	std::cout << "Got " << ct1.size() << " characters from first and " << ct2.size() << " from second.\nMatching...\n";

	// perform the match
	matcher mch(ct1, ct2);

	auto res = mch.do_match();

	for (int n = 0; n < res.size(); ++n)
	{
		if (res[n])
			std::cout << n << " = " << res[n] << '\n';
	}
	auto mx = std::max_element(res.begin(), res.end());
	std::cout << "Max = " << *mx << " at " << std::distance(res.begin(), mx) << '\n';
	std::cout << "via match_at = " << mch.match_at(std::distance(res.begin(), mx)) << '\n';
//	mch.print_at(std::distance(res.begin(), mx), std::cout);
//	mch.print_at(114, std::cout);
	for (int off = 0; off < ct1.size() + ct2.size(); ++off)
	{
		std::cout << off << ", " << mch.match_at(off) << ", " << mch.ban_at(off) << '\n';
	}
}