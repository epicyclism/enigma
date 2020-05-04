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
#include "match.h"
#include "hillclimb.h"

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

template<typename I, typename C> void print_all(I ctb, I cte, C const& c)
{
	for (auto r : c)
	{
		machine3 m3 = MakeMachine3(r.mst_);
		std::vector<modalpha> vo;
		vo.reserve(std::distance(ctb, cte));
		decode(ctb, cte, m3, vo);
		// report
		std::cout << r.mst_ << " = " << get_score(r) << " - ";
		for (auto c : vo)
			std::cout << c;
		std::cout << "\n";
	}
}

template<typename I> void operate( I ctb, I cte, machine3 const& m3, modalpha bs, arena_t& a)
{
	std::vector<result_ioc_t>          vr;
	a.results_[0].fill(0);
	auto row = static_cast<size_t>(bs);
	match_search_exp(ctb, cte, a.arena_[row], a.active_width_, a.results_[row], bs);
	auto rb = std::begin(a.results_[row]);
	int cnt = 0;
	int cp = 0;
	auto threshold = 3U;
	while (rb != std::end(a.results_[row]))
	{
		auto score = *rb;
		if (score > threshold) // decode!
		{
			++cnt;
#if 0
			auto off = std::distance(std::begin(a.results_[row]), rb);
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
#endif
		}
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::Q, alpha::A, alpha::Y))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			auto start = std::chrono::steady_clock::now();
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
			auto now = std::chrono::steady_clock::now();
			std::cout << "use_ees time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
			std::cout << "QAY score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0 
		if (*(std::begin(a.pos_) + cp) == position(alpha::Q, alpha::U, alpha::Y))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			auto start = std::chrono::steady_clock::now();
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
			auto now = std::chrono::steady_clock::now();
			std::cout << "use_ees time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
			std::cout << "QUY score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::G, alpha::A, alpha::B))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			std::cout << "cp = " << cp << ", off = " << off << '\n';
#if 1
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
#else
			machine3 m3a = MakeMachine3(m3.machine_settings());
			m3a.Position(*(std::begin(a.pos_) + off));
			m3a.ApplyPlug(alpha::E, bs);
			vr.emplace_back(m3a.machine_settings(), 0.5);
#endif
			std::cout << "GAB score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::F, alpha::A, alpha::N))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			std::cout << "cp = " << cp << ", off = " << off << '\n';
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
			std::cout << "FAN score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 1
		if (*(std::begin(a.pos_) + cp) == position(alpha::U, alpha::H, alpha::L))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
#if 0
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
#else
			machine3 m3a = MakeMachine3(m3.machine_settings());
			m3a.Position(*(std::begin(a.pos_) + off));
			m3a.ApplyPlug(alpha::H, alpha::T);
			m3a.ApplyPlug(alpha::C, alpha::M);
			m3a.ApplyPlug(alpha::E, bs);
			vr.emplace_back(m3a.machine_settings(), 0.5);
#endif
			std::cout << "UHL score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::U, alpha::E, alpha::D))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
#if 0
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
#else
			machine3 m3a = MakeMachine3(m3.machine_settings());
			m3a.Position(*(std::begin(a.pos_) + off));
			m3a.ApplyPlug(alpha::E, bs);
			vr.emplace_back(m3a.machine_settings(), 0.5);
#endif
			std::cout << "UED score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::B, alpha::L, alpha::Q))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
			std::cout << "BLQ score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
#if 0
		if (*(std::begin(a.pos_) + cp) == position(alpha::A, alpha::G, alpha::I))
		{
			auto off = std::distance(std::begin(a.results_[row]), rb);
			use_ees(ctb, cte, std::begin(a.arena_[row]) + off, *(std::begin(a.pos_) + off), bs, m3.machine_settings(), vr);
			std::cout << "AGI score = " << score << '\n';
			std::cout << vr.back().mst_ << '\n';
			std::cout << "ioc = " << vr.back().ioc_ << '\n';
		}
#endif
		++cp;
		++rb;
	}
#if 1
	print_all(ctb, cte, vr);
#if 0
	std::vector<result_ioc_t>          vr2;
	for (auto r : vr)
	{
		auto start = std::chrono::steady_clock::now();
		auto ns = hillclimb_base(ctb, cte, index_of_coincidence_op(), r.mst_);
//		auto ns = hillclimb_permuted(ctb, cte, index_of_coincidence_op(), r.mst_);
		vr2.emplace_back( r.mst_, ns );
		auto now = std::chrono::steady_clock::now();
		std::cout << "hillclimb_ioc time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	}
	std::cout << vr2.size() << " survived hillclimb_ioc, max score = " << get_score((*std::max_element(vr2.begin(), vr2.end(), [](auto& l, auto& r) { return get_score(l) < get_score(r); }))) << '\n';
	print_all(ctb, cte, vr2);
#endif
	std::vector<result_scr_t>          vr3;
	for (auto r : vr)
	{
		auto start = std::chrono::steady_clock::now();
		auto ns = hillclimb_base(ctb, cte, bigram_score_gen_op(), r.mst_);
//		auto ns = hillclimb_permuted(ctb, cte, bigram_score_1941_op(), r.mst_);
		vr3.emplace_back( r.mst_, ns );
		auto now = std::chrono::steady_clock::now();
		std::cout << "hillclimb_bg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	}
	std::cout << vr3.size() << " survived hillclimb_bg, max score = " << (*std::max_element(vr3.begin(), vr3.end(), [](auto& l, auto& r) { return l.scr_ < r.scr_; })).scr_ << '\n';
	print_all(ctb, cte, vr3);
#endif
#if 1
	std::vector<result_scr_t>          vr4;
	for (auto r : vr3)
	{
		auto start = std::chrono::steady_clock::now();
		auto ns = hillclimb_base(ctb, cte, trigram_score_gen_op(), r.mst_);
//		auto ns = hillclimb_permuted(ctb, cte, trigram_score_1941_op(), r.mst_);
		vr4.emplace_back(r.mst_, ns);
		auto now = std::chrono::steady_clock::now();
		std::cout << "hillclimb_tg time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
	}
	std::cout << vr4.size() << " survived hillclimb_tg, max score = " << (*std::max_element(vr4.begin(), vr4.end(), [](auto& l, auto& r) { return l.scr_ < r.scr_; })).scr_ << '\n';
	print_all(ctb, cte, vr4);
#endif
}

int main()
{
	// B425 agm QAY "DM EP FL HI JR KY NQ OU SW TZ"
	//auto ct1 = make_alpha_array("QKXETVPZQOHSXMBIZPHTCTRMAUZYSTJIMDUYOZBFRTZOUHBGOROUVRQEJRDRJHZPZIBQQHKMMJZCIIRCUOLXLCIOQKHRLIGGFJFTLLGDRARDZQUQKLTKXXXYKRUVFULBQLAYRZVJFULCGQJXFJURMURSELYFVFOKUHYUHSYLOMEFYAIIP");
	//auto ct1 = make_alpha_array("QKXETVPZQOHSXMBIZPHTCTRMAUZYSTJIMDUYOZBFRTZOUHBGOROUVRQEJRDRJHZPZIBQQHKMMJZCIIRCUOLXLCIOQKHRLIGGFJFTLLGDRARDZQUQKL");
	//machine3 m3 = MakeMachine3("B425");
	//Ring(m3, "agm");
	//modalpha erow = alpha::P;

	// B347 aaa AAA  "DM EP FL HI JR KY NQ OU SW TZ"
	//auto ct1 = make_alpha_array("PVDZOVSRUXBKOWUJANXDNVEXHDDDHFZNSWJDKSYCLZWOKYTPQQFYLRNPGONTJTGGRSINQCLRBVWGMKJHFRCLFSILDUBADOSPFPIFNQVTOGSHHHYGIBEJUIVBZFWXSITTWKLFYCDUPYYUJABJSBLFHPFFGDHIFMGEFFMCVHWFJZDXXYMSZ");
	//machine3 m3 = MakeMachine3("B347");
	//Ring(m3, "aaa");
	//modalpha erow = alpha::P;

	// B345 aaa AAA  "DM EP FL HI JR KY NQ OU SW TZ"
	//auto ct1 = make_alpha_array("VDJKUZNBZQQVRJLHSZVDVLGYEYOVUOYBCKJGRVPHVUOYFAGRZBFCNTMMQXXHMYTGIPHHKUNDPNBDUBVSVYRPPFVUQRWBHUMUTVVVYHLFHNYPVJULRRPFOIDIPPPVJBAFXFSFPXWVNEICFUPWEFIYAKIWGOLMCERTCFDUOYMMISSAGZPNA");
	//machine3 m3 = MakeMachine3("B345");
	//Ring(m3, "aaa");
	//modalpha erow = alpha::P;

	// B251 bcn UED "AO BV DS EX FT HZ IQ JW KU PR"
	//auto ct1 = make_alpha_array("UPONTXBBWFYAQNFLZTBHLBWXSOZUDCDYIZNRRHPPBNSV");
	//machine3 m3 = MakeMachine3("B251");
	//Ring(m3, "bcn");
	//modalpha erow = alpha::X;
	
	// B213 zwd AGI "IU JO RW MV EZ BL PX"
	//auto ct1 = make_alpha_array("QKRQWUQTZKFXZOMJFOYRHYZWVBXYSIWMMVWBLEBDMWUWBTVHMRFLKSDCCEXIYPAHRMPZIOVBBRVLNHZUPOSYEIPWJTUGYOSLAOXRHKVCHQOSVDTRBPDJEUKSBBXHTYGVHGFICACVGUVOQFAQWBKXZJSQJFZPEVJRO");
	//machine3 m3 = MakeMachine3("B213");
	//Ring(m3, "zwd");
	//modalpha erow = alpha::Z;

	//  Banbury 1
	// B152 nht TGB "BG CM DY EX FO HT IL KV NW PS"
	// B152 abt GAB "BG CM DY EX FO HT IL KV NW PS"
	//auto ct1 = make_alpha_array("DNGXQPZKKPPJSKNRTGJOTRYNFSEVEBWQAAJHTVYRWAGPRIEOPNLPSMOXQNKVYDPWCOXRRCYPAFNFSAYTEGWGUYXGHHDZHWTXWQMELJSURHMOYOLBTD");
	//machine3 m3 = MakeMachine3("B152");
	//Ring(m3, "abt");
	//modalpha erow = alpha::X;

	// ??? Banbury 2
	// B152 aat UGL "BG CM DY EX FO HT IL KV NW PS"
	// B152 abt UHL "BG CM DY EX FO HT IL KV NW PS"
	// B152 nht HNL "BG CM DY EX FO HT IL KV NW PS"
	auto ct1 = make_alpha_array("TGPIUEYDKHIBWOYJQGSSQYJXZSIRJUJGCISXWXAFSURPPCPAUXJRQTXKTCHAPWUDILAMBFBCMGRZAQYWKHRQBEVSNBZYBEOPLYZXRTNKWMCLOSQIGPVSUHFPPSOK");
	machine3 m3 = MakeMachine3("B152");
	Ring(m3, "abt");
	modalpha erow = alpha::X;
	
	m3.Setting(alpha::A, alpha::A, alpha::A);

	std::cout << "# ";
	m3.ReportSettings(std::cout);
	std::cout << "\n# Ready\n";

	auto start = std::chrono::steady_clock::now();
	fill_arena(m3.Wheels(), a, 0);
	auto now = std::chrono::steady_clock::now();
	std::cout << "Fill arena time: " << std::chrono::duration<double, std::nano>(now - start).count() << "ns\n";

	start = std::chrono::steady_clock::now();
	operate(std::begin(ct1), std::end(ct1), m3, erow, a);
	now = std::chrono::steady_clock::now();
	std::cout << "time: " << std::chrono::duration<double, std::milli>(now - start).count() << "ms\n";
}