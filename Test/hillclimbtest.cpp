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
	auto ns = hillclimb_base(ct.begin(), ct.end(), bigram_score_gen_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb_bg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
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

auto hillclimb3_bg(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_3(ct.begin(), ct.end(), bigram_score_gen_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb3_bg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb3_tg(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_3(ct.begin(), ct.end(), trigram_score_gen_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb3_tg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb3_bg41(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_3(ct.begin(), ct.end(), bigram_score_1941_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb3_bg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
}

auto hillclimb3_tg41(machine_settings_t mst, std::vector<modalpha> const& ct)
{
	auto start = std::chrono::steady_clock::now();
	auto ns = hillclimb_3(ct.begin(), ct.end(), trigram_score_1941_op(), mst);
	auto now = std::chrono::steady_clock::now();
	std::cout << "hillclimb3_tg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(ct.size());
	decode(ct.begin(), ct.end(), m3, vo);
	// report
	std::cout << mst << " = " << ns << " - ";
	report_ciphertext(vo, std::cout);
	return m3.machine_settings();
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
		if (ac > 4)
			Stecker(m3, av[4]);
		std::cout << "hillclimbtest " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady to read ciphertext...\n\n";
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is -\n";
		report_ciphertext(ct, std::cout);
		hillclimb_bg(m3.machine_settings(), ct);
		hillclimb_tg(m3.machine_settings(), ct);
		hillclimb3_bg(m3.machine_settings(), ct);
		hillclimb3_tg(m3.machine_settings(), ct);
		hillclimb3_bg41(m3.machine_settings(), ct);
		hillclimb3_tg41(m3.machine_settings(), ct);
		hillclimb3_tg41(hillclimb3_bg41(m3.machine_settings(), ct), ct);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}
}
