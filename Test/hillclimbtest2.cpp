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

constexpr  char version[] = "v0.01";

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

template<typename IC> auto hillclimb_exhaust3_ioc(IC ctb, IC cte, machine_settings_t& mst)
{
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s_best;
    stecker s_best_bg;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s_best);
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
    auto bgb  = bigram_score(vo.begin(), vo.end());
    std::cout << "starting at ioc = " << iocb << ", bg = " << bgb << '\n';
	for (int f1 = 0; f1 < alpha_max - 1; ++f1)
	{
        for(int t1 = f1 + 1; t1 < alpha_max; ++t1)
        {
            for (int f2 = f1 + 1; f2 < alpha_max - 1; ++f2)
            {
                for(int t2 = f2 + 1; t2 < alpha_max; ++t2)
                {
                    for (int f3 = f2 + 1; f3 < alpha_max - 1; ++f3)
                    {
                        for(int t3 = f3 + 1; t3 < alpha_max; ++t3)
                        {
                            stecker s;
                            s.Apply(modalpha(f3), modalpha(t3));
				            s.Apply(modalpha(f2), modalpha(t2));
				            s.Apply(modalpha(f1), modalpha(t1));
	                        vo = fd.decode(ctb, cte, s);
	                        auto iocn = index_of_coincidence(vo.begin(), vo.end());
                            auto bgn  = bigram_score(vo.begin(), vo.end());
                            if(iocn > iocb)
                            {
                                s_best = s;
                                iocb = iocn;
                            }
                            if(bgn > bgb)
                            {
                                s_best_bg = s;
                                bgb = bgn;
                            }
                        }
                    }
                }
            }
        }
    }
    mst.stecker_ = s_best_bg;
    return std::make_pair(iocb, bgb);
}

void hillclimb_test_wrap (machine_settings_t mst, std::vector<modalpha> const& ct)
{
	std::cout << "hillclimb_test.\n";
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_exhaust3_ioc(std::begin(ct), std::end(ct), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_test time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns.first << ", " << ns.second << " - ";
	report_ciphertext(vo, std::cout);
}

void Help()
{
	std::cerr << "hillclimbtest2 " << version << " : test bed for hillclimb.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./hillclimbtest2 B125 fvn XWB\n\n";
	std::cerr << "configures a 'machine' with reflector B, rotors 1, 2, 5, ring setting fvn, initial setting XWB\n";
	std::cerr << "and then hillclimbs searching for plugs, using the ciphertext supplied on stdin.\n\n";
	std::cerr << "Copyright (c) 2021, paul@epicyclism.com. Free to a good home.\n\n";
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
		std::cout << "hillclimbtest2 " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady to read ciphertext...\n\n";
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is -\n";
		report_ciphertext(ct, std::cout);
		std::cout << "stats - \n";
		report_ct_stats(ct, std::cout);
        hillclimb_test_wrap(m3.machine_settings(), ct);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}
    return 0;
}
