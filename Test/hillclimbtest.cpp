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
#include "utility.h"

constexpr  char version[] = "v0.03";

void report_ct_stats(std::vector<modalpha> const& ct, std::ostream& ostr)
{
	ostr << "ct has " << ct.size() << " characters.\n";
	auto tab = gen_freq_tab_seq(ct.begin(), ct.end());
	for (auto& l : tab)
	{
		ostr << l.first << " : " << l.second << '\n';
	}
	ostr << '\n';
}

auto hillclimb_bg(machine_settings_t& mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_base(ct.begin(), ct.end(), bigram_score_gen_op(), mst);
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

auto hillclimb_bg2(machine_settings_t& mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_base(ct.begin(), ct.end(), bigram_score_cur_op(), mst);
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

auto hillclimb_tg(machine_settings_t& mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_base(ct.begin(), ct.end(), trigram_score_gen_op(), mst);
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
	auto ns = hillclimb_base(ct.begin(), ct.end(), trigram_score_cur_op(), mst);
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
	hillclimb_tg(mst, ct);

	std::cout << "\n\n";
}

void hillclimb_test_consecutive(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::cout << "IOC, consecutive\n";
	constexpr modalpha tst_arr[]{ alpha::E, alpha::X, alpha::N, alpha::R, alpha::S, alpha::I };
	double scrm = 0;
	modalpha mf = 0;
	modalpha mt = 0;
	for (auto f : tst_arr)
	{
		auto [scr, t] = single_stecker(ct.begin(), ct.end(), index_of_coincidence_op(), f, mst);
		std::cout << "    Mx " << f << " to " << t << " {" << scr << "}\n";
		if (scr > scrm)
		{
			mf = f;
			mt = t;
			scrm = scr;
		}
	}
	std::cout << "Apply " << mf << " to " << mt << " {" << scrm << "}\n";
	mst.stecker_.Apply(mf, mt);

	std::cout << "\n\n";
}

void hillclimb_test_partial_ex_fast_flex (machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::array<modalpha, 2> ees{ alpha::X, alpha::F };
	std::cout << "hillclimb_test_partial_ex_fast_flex testing EX in the outer loop.\n";
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_partial_exhaust_fast(std::begin(ct), std::end(ct), ees.begin(), ees.begin() + 2, trigram_score_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_test_partial_ex_fast_flex time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
}


void hillclimb_test_partial_ex_fast2 (machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::cout << "hillclimb_test_partial_ex_fast.\n";
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_partial_exhaust2_fast(std::begin(ct), std::end(ct), trigram_score_op(), alpha::E, alpha::S, mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_partial_exhaust2_fast time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
}

void hillclimb_test_partial_ex_fast3 (machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::cout << "hillclimb_test_partial_ex_fast.\n";
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_partial_exhaust3_fast(std::begin(ct), std::end(ct), trigram_score_op(), alpha::E, alpha::N, alpha::S, mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_partial_exhaust3_fast time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
}

void hillclimb_test_specific_exhaust(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	// the fixed popular plaintext letters
	constexpr modalpha tst_arr[]{ alpha::E, alpha::X, alpha::N, alpha::R, alpha::S, alpha::I };
	constexpr modalpha tst_prs[]{ alpha::B, alpha::G, alpha::P, alpha::S };
	// the computed popular ciphertext letters
	auto tab = gen_freq_seq(ct.begin(), ct.end());
	// make the plug combiations
//	auto plugsets = make_plug_list2(std::begin(tst_arr), std::end(tst_arr), std::begin(tab), std::begin(tab) + 10);
	auto plugsets = make_plug_list2(std::begin(tab), std::begin(tab) + 11);
//	auto plugsets = make_plug_list2(std::begin(tst_prs), std::end(tst_prs));
	for (auto& c : tab)
	{
		std::cout << c << ' ';
	}
	std::cout << '\n';
#if 0
	for (auto& ps : plugsets)
	{
		std::cout << ps[0].first << "<->" << ps[0].second << ", " << ps[1].first << "<->" << ps[1].second << '\n';
	}
	std::cout << '\n';
#endif
	std::cout << "Plugset size = " << plugsets.size() << '\n';
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);

	std::cout << "hillclimb_test_specific_exhaust.\n";
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_specific_exhaust_fast(std::begin(ct), std::end(ct), std::begin(plugsets), std::end(plugsets), trigram_score_op(), fd.get_ref(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_test_specific_exhaust time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	machine3 m3a = MakeMachine3(mst);
	decode(ct.begin(), ct.end(), m3a, vo);
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
		std::cout << "stats - \n";
		report_ct_stats(ct, std::cout);
//		hillclimb_test_partial_ex(m3.machine_settings(), b3, ct);
//		hillclimb_test_partial_ex_fast2(m3.machine_settings(), ct);
//		hillclimb_test_partial_ex_fast3(m3.machine_settings(), ct);
//		hillclimb_test_fd_ref(m3.machine_settings(), ct);
//		hillclimb_test_partial_ex_fast_flex(m3.machine_settings(), ct);
//		hillclimb_test_consecutive(m3.machine_settings(), ct);
		hillclimb_test_specific_exhaust(m3.machine_settings(), ct);
#if 0
		hillclimb_test_iterative(m3.machine_settings(), ct);
		hillclimb_test_single(m3.machine_settings(), ct);

		hillclimb_test_triple(m3.machine_settings(), ct);
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
