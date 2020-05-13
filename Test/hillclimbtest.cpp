#include <iostream>
#include <utility>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "const_helpers.h"
#include "machine.h"
#include "match.h"
#include "hillclimb.h"

constexpr  char version[] = "v0.03";

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

void report_ciphertext(std::vector<modalpha> const& ct, std::ostream& ostr)
{
	for (auto c : ct)
		ostr << c;
	ostr << '\n';
}

// machine by value.
//
auto hillclimb_bg(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_permuted(ct.begin(), ct.end(), bigram_score_gen_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_bg (gen) time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb_bg2(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_permuted(ct.begin(), ct.end(), bigram_score_cur_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_bg (cur) time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb_tg(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_permuted(ct.begin(), ct.end(), trigram_score_gen_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_tg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb_tg2(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_permuted(ct.begin(), ct.end(), trigram_score_cur_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_tg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

void hillclimb_test_single(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	// single for the 4 most common, one at a time.
	std::cout << "IOC, single, individual\n";
	constexpr modalpha tst_arr[]{ alpha::E, alpha::X, alpha::N, alpha::R, alpha::S, alpha::I };
	for (auto f : tst_arr)
	{
		auto [scr, t] = single_stecker(ct.begin(), ct.end(), index_of_coincidence_op(), f, mst);
		std::cout << "Single " << f << " to " << t << " {" << scr << "}\n";
	}
	std::cout << "\nunigram, single, individual\n";
	for (auto f : tst_arr)
	{
		auto [scr, t] = single_stecker(ct.begin(), ct.end(), unigram_score_op(), f, mst);
		std::cout << "Single " << f << " to " << t << " {" << scr << "}\n";
	}
	std::cout << "\n\n";
}

void hillclimb_test_iterative(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::cout << "IOC, iterative\n";
	constexpr modalpha tst_arr[]{ alpha::E, alpha::X, alpha::N, alpha::R, alpha::S, alpha::I };
	double scrm = 0;
	for (auto c = 0; c < 6; ++c)
	{
		modalpha mf = 0;
		modalpha mt = 0;
		for (auto f : tst_arr)
		{
			auto [scr, t] = single_stecker(ct.begin(), ct.end(), index_of_coincidence_op(), f, mst);
			if (scr > scrm)
			{
				std::cout << "    Mx " << f << " to " << t << " {" << scr << "}\n";
				mf = f;
				mt = t;
				scrm = scr;
			}
		}
		std::cout << "Apply " << mf << " to " << mt << " {" << scrm << "}\n";
		mst.stecker_.Apply(mf, mt);
	}
	std::cout << "Finally = " << mst << '\n';
#if 0
	std::cout << "\nunigram, iterative, individual\n";
	for (auto f : tst_arr)
	{
		auto [scr, t] = single_stecker(ct.begin(), ct.end(), unigram_score_op(), f, mst);
		std::cout << "Single " << f << " to " << t << " {" << scr << "}\n";
	}
#endif
	hillclimb_tg(mst, ct);

	std::cout << "\n\n";
}

void hillclimb_test_partial_ex (machine_settings_t mst, bool b3, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = b3 ? hillclimb_partial_exhaust3(ct.begin(), ct.end(), trigram_score_gen_op(), alpha::E, alpha::N, alpha::S, mst) :
					hillclimb_partial_exhaust2(ct.begin(), ct.end(), trigram_score_gen_op(), alpha::N, alpha::S, mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_test_partial_ex time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
}

void hillclimb_test_partial_ex_fast (machine_settings_t mst, bool b3, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = b3 ? hillclimb_partial_exhaust3_fast(ct.begin(), ct.end(), trigram_score_gen_op(), alpha::E, alpha::N, alpha::S, mst) :
					hillclimb_partial_exhaust2_fast(ct.begin(), ct.end(), trigram_score_gen_op(), alpha::N, alpha::S, mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_test_partial_ex_fast time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
}

void Help()
{
	std::cerr << "hillclimbtest " << version << " : test bed for hillclimb variations.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./hillclimbtest B125 fvn XWB [\"AB CD EF\"]\n\n";
	std::cerr << "configures a 'machine' with reflector B, rotors 1, 2, 5, ring setting fvn, initial setting XWB\n";
	std::cerr << "optionally preloaded plugs as specified and then hillclimbs, using the ciphertext supplied on stdin.\n\n";
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
		bool b3 = true;
		if (ac > 4)
		{
			Stecker(m3, av[4]);
			b3 = false;
		}
		std::cout << "hillclimbtest " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady to read ciphertext...\n\n";
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is -\n";
		report_ciphertext(ct, std::cout);
//		hillclimb_test_partial_ex(m3.machine_settings(), b3, ct);
		hillclimb_test_partial_ex_fast(m3.machine_settings(), b3, ct);
#if 0
		hillclimb_test_single(m3.machine_settings(), ct);
		hillclimb_test_triple(m3.machine_settings(), ct);
		hillclimb_test_iterative(m3.machine_settings(), ct);
		hillclimb_bg(m3.machine_settings(), ct);
		hillclimb_bg2(m3.machine_settings(), ct);
		hillclimb_tg(m3.machine_settings(), ct);
		hillclimb_tg2(m3.machine_settings(), ct);
		hillclimb_bg2(m3.machine_settings(), ct);
		hillclimb3_bg(m3.machine_settings(), ct);
		hillclimb3_tg(m3.machine_settings(), ct);
		hillclimb3_bg41(m3.machine_settings(), ct);
		hillclimb3_tg41(m3.machine_settings(), ct);
		hillclimb3_tg41(hillclimb3_bg41(m3.machine_settings(), ct), ct);
#endif
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}
}
