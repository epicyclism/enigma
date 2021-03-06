#include <iostream>
#include <utility>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "const_helpers.h"
#include "machine.h"
#include "match.h"
#include "utility.h"

constexpr  char version[] = "v0.04";

template<typename IC, typename IA> double test_score(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
		{
			psm.set(c, *base);
			++base;
		});
	if (bs != alpha::E)
	{
		psm.merge_direct_exp(bs, alpha::E);
	}
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt < 2 and make others unique
	psm.unique();
//	psm.print(std::cout);
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	auto ctl = std::distance(ctb, cte);
	double dbit = 10.0 / (0.14 * ctl);
	double direct = psm.direct() * dbit;
	return std::accumulate(psm.begin(), pr, direct, [dbit](auto& l, auto& r) { return l + dbit * r.cnt_* r.cnt_ ; }) ;
}

template<typename IC, typename IA> unsigned test_score2(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
		{
			psm.set(c, *base);
			++base;
		});
	if (bs != alpha::E)
	{
		psm.merge_direct(bs, alpha::E);
	}
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt < 2 and make others unique
	return (*psm.begin()).cnt_ ;
}

template<typename IC, typename IA> double test_score3(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
		{
			psm.set(c, *base);
			++base;
		});
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt < 2 and make others unique
//	psm.print(std::cout);
	psm.unique();
//	psm.print(std::cout);
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	auto ctl = std::distance(ctb, cte);
	double dbit = 10.0 / (0.14 * ctl);
	double direct = psm.direct() * dbit;
	return std::accumulate(psm.begin(), pr, direct, [dbit](auto& l, auto& r) { return l + dbit * r.cnt_; });
}

template<typename IC, typename IA> unsigned test_score4(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
		{
			psm.set(c, *base);
			++base;
		});
	psm.prune(2);
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt < 2 
//	psm.print(std::cout);
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	return std::accumulate(psm.begin(), pr, psm.direct(), [](auto& l, auto& r) { return l + r.cnt_; });
}

template<typename CT> void do_solo(machine3& m3, modalpha bs, CT const& ct)
{
	// compute the 'score'
	typename std::remove_cv<CT>::type et(ct.size(), bs);
	// generate encrypted (plugged) 'E's
	m3.Transform(et.begin(), et.end(), et.begin());
	std::cout << "\nEncrypted 'E's are -\n";
	for (auto c : et)
		std::cout << c;
	std::cout << '\n';
//	// match and score
	auto scr1 = match_ciphertext(ct.begin(), ct.end(), et.begin(), bs);
	auto scr2 = test_score3(ct.begin(), ct.end(), et.begin(), bs);
	auto scr4 = test_score4(ct.begin(), ct.end(), et.begin(), bs);
	std::cout << modalpha(bs) << std::setw(5) << scr1 << ", " << std::setw(8) << scr2 << ", " << std::setw(8) << scr4 << '\n';
}

template<typename CT> void do_slice(machine3& m3, CT const& ct)
{
//	unsigned scr1_ioc[alpha_max];
//	unsigned scr2_ioc[alpha_max];
	unsigned scr4_acc[alpha_max];
	for (int bsi = 0; bsi < 26; ++bsi)
	{
		// compute the 'score'
		typename std::remove_cv<CT>::type et(ct.size(), bsi);
		// generate encrypted (plugged) 'E's
		auto pos = m3.Position();
		m3.Transform(et.begin(), et.end(), et.begin());
		m3.Position(pos);
		// match and score
		auto scr1 = match_ciphertext(ct.begin(), ct.end(), et.begin(), bsi);
		auto scr2 = test_score(ct.begin(), ct.end(), et.begin(), bsi);
		auto scr4 = test_score4(ct.begin(), ct.end(), et.begin(), bsi);
		std::cout << modalpha(bsi) << std::setw(5) << scr1 << ", " << std::setw(8) << scr2 << ", " << std::setw(8) << scr4 << '\n';
//		scr1_ioc[bsi] = scr1;
//		scr2_ioc[bsi] = static_cast<unsigned>(scr2);
		scr4_acc[bsi] = scr4;
	}
#if 0
	std::cout << "ioc scr1 = " << std::transform_reduce( std::begin(scr1_ioc), std::end(scr1_ioc), 0.0, std::plus{}, [](auto n)
		{
			return double(n * (n - 1)) / (26*25);
		}) << '\n';
	std::cout << "ioc scr2 = " << std::transform_reduce( std::begin(scr2_ioc), std::end(scr2_ioc), 0.0, std::plus{}, [](auto n)
		{
			return double(n * (n - 1)) / (26*25);
		}) << '\n';
#endif
	std::sort(std::begin(scr4_acc), std::end(scr4_acc), [](auto l, auto r) {return r < l; });
	std::cout << "Slice score = " << std::accumulate(std::begin(scr4_acc), std::begin(scr4_acc) + 6, 0) << '\n';
}

template<typename CT> void do_slice_cum(machine3& m3, CT const& ct)
{
	plug_set_msg psm;
	for (int bsi = 0; bsi < 26; ++bsi)
	{
		typename std::remove_cv<CT>::type et(ct.size(), bsi);
		auto pos = m3.Position();
		m3.Transform(et.begin(), et.end(), et.begin());
		for (auto c : et)
			std::cout << c;
		std::cout << '\n';

		m3.Position(pos);
		// collect stecker possibles
		auto base = et.begin();
		std::for_each(std::begin(ct), std::end(ct), [&base, &psm](auto const c)
			{
				psm.set(c, *base);
				++base;
			});
	}
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	psm.print(std::cout);
}

void Help()
{
	std::cerr << "score " << version << " : evaluates the score at the given machine rotor state with the supplied ciphertest.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./score B125 fvn XWB [E]\n\n";
	std::cerr << "configures a 'machine' with reflector B, rotors 1, 2, 5, ring setting fvn, initial setting XWB\n";
	std::cerr << "and then computes and reports the 'score' for this position, with the ciphertext supplied on stdin.\n";
	std::cerr << "If the trailingn 'E' (or whatever) is provided then score computed just for 'row' of this value\n";
	std::cerr << "otherwise an entire slice, A-Z, is reported upon.\n";
	std::cerr << "Copyright (c) 2020, paul@epicyclism.com. Free to a good home.\n\n";
}

int main(int ac, char** av)
{
	if (ac < 4)
	{
		std::cerr << "Too few arguments - " << ac << "\n\n";
		Help();
		return -1;
	}
	try
	{
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);
		Setting(m3, av[3]);
		std::cout << "score " << version << " configured : ";
		m3.ReportSettings(std::cout);
		auto mst = m3.machine_settings();
		std::cout << "\nReady to read ciphertext...\n\n";
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is -\n";
		for (auto c : ct)
			std::cout << c;
		std::cout << '\n';
		if( ac > 4)
			do_solo(m3, from_printable(av[4][0]), ct);
		else
			do_slice_cum(m3, ct);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}
}
