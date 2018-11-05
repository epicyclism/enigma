// linear.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "machine.h"

constexpr  char version[] = "v0.01";

// slide the ciphertext along the row and count the matches.
//
template<typename I, size_t W> void linear_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
	while (itb != ite)
	{
		*ito = std::transform_reduce(cb, ce, itb, 0, std::plus<>(), [](auto l, auto r) { return l == r ? 1 : 0; });
		++ito;
		++itb;
	}
}

template<typename I, typename L, typename R> void report_results(I cb, I ce, L const& a, R& r, machine3& m3)
{
	auto sz = std::distance(cb, ce);
	auto itp = std::begin(l.pos_);

	auto itm = std::max_element(std::begin(r), std::end(r) - sz);
	std::cout << *(itp + std::distance(std::begin(r), itm)) << " - " << *itm << " - ";
	// decode it!
	m3.Position(*(itp + std::distance(std::begin(r), itm)));
	m3.ReportSettings(std::cout);
	auto cbc = cb;
	while (cbc != ce)
	{
		std::cout << m3.Transform(*cbc);
		++cbc;
	}
	std::cout << "\n";
}

void Help()
{
	std::cerr << "linear " << version << " : experiments in brute force Enigma cracking, given wheels, ring and stecker.\n\n";
}

template<size_t W> struct line_base
{
	static const size_t Width = W;
	// position of each column
	std::array<position, W> pos_;
	// line.
	std::array<modalpha, W> ln_;

	using results_t = std::array<unsigned, W>;
};

template<typename L> void fill_line(machine3& m3, L& l, modalpha ch)
{
	auto itp = std::begin(l.pos_);
	std::cout << m3.Position() << "\n";
	std::generate(std::begin(l.ln_), std::end(l.ln_), [&]()
	{
		*itp = m3.Position();
		++itp;
		return m3.Transform(ch);
	});
}

using line_t = line_base<26 * 26 * 27>;
//using line_t = line_base<256>;
line_t l;
line_t::results_t r;

int main(int ac, char**av)
{
	Help();
	auto ciphertext3 = make_alpha_array("ZJTPLTJNETNLLGOPQVSWXSRHC"
		"OSHUTFGUSHHTVPOUMBMVGKLAAF"
		"DUBNUVCUVPOCFJXDMIQCCAUCBQ"
		"OKPHUMCIZAJVIQESVGCFHDTISR"
		"EHFCMBPJCRTWTTMXCNOIEUWRPO"
		"MCEMSUNBBCTWZZRBLFLUFIFBNY"
		"OYJGXUMNKPTCQHTGVYWSQDFFMS"
		"WVECIDWILZBYLIPRXYICFCLPDQ"
		"ZNOZWSKVNJURTGKMWUNFPNLEPO"
		"FQLJMEDEFNMLRRRRJYTBVRKBQQ"
		"GSUWVAWAFUUWFLMPKPHLDML");

	auto ciphertext9 = make_alpha_array("INAVHYMVHIAGMJOPKVJHSGJYY"   
		"KNHLFKRZWHWLAKKEGGHZFEAKV"
		"VIDDSYYVEYQFQJPVYHLFUZESA"
		"OLGNHTXTTBDZJVOAGEAWHBBWC"
		"ADYYTHSLRXMPEDICATSMALBZY"
		"LBPZMQDSXZHPFSXVYCBKGEBTG"
		"QGZIIDQJDBYDACSWJGXUCUXLT"
		"RTMZHHWXZPESSYEEPFCQAOWOS"
		"PLUZUCVOKYJXCPYGNJHSPNCFS"
		"WTLLMSGACQBSUTPSAVGUYFVKS"
		"UBSQEGVZKVNRLXFIXZQWFKSXC"
		"PPFRIMWQHTQSB");

	try
	{
#if 1
		machine3 m3{ C, V, II, III };
		m3.Ring(alpha::H, alpha::S, alpha::C);
		m3.Setting(alpha::A, alpha::A, alpha::A);
		Stecker(m3, "AL FP HX JO KT NV QR SU WY");
		m3.ReportSettings(std::cout);
		fill_line(m3, l, alpha::E);
		linear_search(std::begin(ciphertext3), std::end(ciphertext3), l.ln_, r);
		report_results(std::begin(ciphertext3), std::end(ciphertext3), l.ln_, r, m3);
#else
		machine3 m3{ C, I, III, IV };
		m3.Ring(alpha::G, alpha::X, alpha::O);
		m3.Setting(alpha::A, alpha::A, alpha::A);
		Stecker(m3, "AT BG DV EW FR HN IQ JX KZ LU");
		m3.ReportSettings(std::cout);
		fill_line(m3, l, alpha::E);
		linear_search(std::begin(ciphertext9), std::end(ciphertext9), l.ln_, r);
		report_results(std::begin(ciphertext9), std::end(ciphertext9), l.ln_, r, m3);
#endif
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
