// Matchtest.cpp
//
// test the matching function between Enigma output at a steady input and the ciphertext looking for stecker sets
// that maximise the match
//

#include <iostream>
#include <iomanip>
#include <chrono>
#include "const_helpers.h"
#include "machine.h"
#include "arena.h"
#include "ioc.h"
#include "bigram.h"
#include "match.h"

using arena_t = arena_base<26 * 26 * 26 + 256>;

arena_t a;

void reportwkr(arena_t::results_t const& r, std::array<int, 61>& hi)
{
	auto itb = std::begin(r);
	while (itb != std::end(r))
	{
		int score = ((*itb & 0x0000ffff) - (*itb >> 16));
		if (score >= 60)
			++hi[60];
		else
		if (score < 0)
			++hi[0];
		else
			++hi[score];
		++itb;
	}
}

void report(arena_t::results_t const& r)
{
	std::array<int, 61> hi;
	hi.fill(0);

	reportwkr(r, hi);

	int i = 0;
	for (auto n : hi)
	{
		std::cout << i << " " << n << "\n";
		++i;
	}
	std::cout << "\n";
}

void reportall(arena_t const& a)
{
	std::array<int, 61> hi;
	hi.fill(0);

	for (auto& r : a.results_)
		reportwkr(r, hi);

	int i = 0;
	for (auto n : hi)
	{
		std::cout << i << " " << n << "\n";
		++i;
	}
	std::cout << "\n";
}

template<typename I> void operate( I ctb, I cte, machine3 const& m3, arena_t& a)
{
	std::vector<result_ioc_t>          vr;
	a.results_[0].fill(0);
	match_search(ctb, cte, a.arena_[0], a.results_[0], modalpha(0));
	auto rb = std::begin(a.results_[0]);
	int cnt = 0;
	auto threshold = std::distance(ctb, cte) / 11;
	while (rb != std::end(a.results_[0]))
	{
		auto score = *rb;

		if (score > threshold) // decode!
		{
			++cnt;
			auto off = std::distance(std::begin(a.results_[0]), rb);
			use_ees(ctb, cte, std::begin(a.arena_[0]) + off, *(std::begin(a.pos_) + off), modalpha(0), m3.machine_settings(), vr);
		}
		++rb;
	}
	std::cout << "threshold = " << threshold << ", " << cnt << " qualified.\n";
	std::vector<result_scr_t>          vr2;
	for (auto r : vr)
	{
		if (r.ioc_ > 0.055)
			hillclimb(ctb, cte, r.mst_, vr2);
	}
	for (auto r : vr2)
	{
		if (r.scr_ > 45000)
		{
			machine3 m3 = MakeMachine3(r.mst_);
			std::vector<modalpha> vo;
			vo.reserve(std::distance(ctb, cte));
			decode(ctb, cte, m3, vo);
			// report
			std::cout << r.mst_ << " = " << r.scr_ << " - ";
			for (auto c : vo)
				std::cout << c;
			std::cout << "\n";
		}
	}
}

int main()
{
	// B251 bcn UED "AO BV DS EX FT HZ IQ JW KU PR"
	auto ct1 = make_alpha_array("UPONTXBBWFYAQNFLZTBHLBWXSOZUDCDYIZNRRHPPBNSV");

	machine3 m3 = MakeMachine3("B251");
	Ring(m3, "bcn");
	m3.Setting(alpha::A, alpha::A, alpha::A);
	std::cout << "# ";
	m3.ReportSettings(std::cout);
	std::cout << "\n# Ready\n";

	auto start = std::chrono::steady_clock::now();
	fill_arena(m3.Wheels(), a, 0);
	auto now = std::chrono::steady_clock::now();
	std::cout << "Fill arena time: " << std::chrono::duration<double, std::nano>(now - start).count() << "ns\n";

	start = std::chrono::steady_clock::now();
	operate(std::begin(ct1), std::end(ct1), m3, a);
	now = std::chrono::steady_clock::now();
	std::cout << "time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	std::cout << "time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
}