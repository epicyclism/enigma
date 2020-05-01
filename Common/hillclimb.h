#pragma once

#include "modalpha.h"
#include "ioc.h"
#include "bigram.h"
#include "trigram.h"

template<typename IC, typename IA, typename R> void use_ees(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const& mst_j, R& r)
{
	// collect the likely candidate pairs
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);
//	psm.print(std::cout);
	// prepare a machine
	machine_settings_t mst(mst_j);
	machine3 m3 = MakeMachine3(mst);
	m3.Position(pos);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
//	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	for (auto& s : psm)
	{
		if (s.cnt_ < 2)
			s.ioc_ = 0;
		else
		{
			m3.Stecker(); // clears
			m3.ApplyPlug(s.f_, s.t_);
			decode(ctb, cte, m3, vo);
			s.ioc_ = index_of_coincidence(std::begin(vo), std::end(vo));
		}
	}
//	std::cout << "base ioc = " << ioc << '\n';
//	psm.print_ioc(std::cout);
	// sort good->bad
//	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { if (l.cnt_ == r.cnt_) return l.ioc_ > r.ioc_; else return l.cnt_ > r.cnt_; });
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.ioc_ > r.ioc_; });
//	psm.print_ioc(std::cout);
	psm.unique();
//	psm.print_ioc(std::cout);
	// apply
	m3.Stecker(); // clears
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	do
	{
		--pr;
		auto& s = *pr;
		m3.ApplyPlug(s.f_, s.t_);
	} while (pr != psm.begin());

	decode(ctb, cte, m3, vo);
	auto iocn = index_of_coincidence(std::begin(vo), std::end(vo));
#if 0
	std::cout << "ioc from " << ioc << " to " << iocn << '\n';
	// report
	m3.ReportSettings(std::cout);
	std::cout << " = ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";
#endif
	r.emplace_back(m3.machine_settings(), iocn);
}

template<typename IC, typename F, size_t max_stecker = 10 > auto hillclimb_base(IC ctb, IC cte, F eval_fn, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = eval_fn(std::begin(vo), std::end(vo));
				if (scrn > scr && m3.SteckerCount() < max_stecker + 1)
				{
					mx = f;
					my = t;
					scr = scrn;
					improved = true;
				}
				m3.PopStecker();
			}
		}
		if (improved)
			m3.ApplyPlug(mx, my);
	}
	mst = m3.machine_settings();
	return scr;
}

template<typename IC, typename F, size_t max_stecker = 10 > auto hillclimb_permuted(IC ctb, IC cte, F eval_fn, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	// the order we're going to examine with
	std::array<modalpha, alpha_max> tst { alpha::A, alpha::B, alpha::C, alpha::D, alpha::E, alpha::F, alpha::G, alpha::H, alpha::I, alpha::J, alpha::K, alpha::L, alpha::M, alpha::N, alpha::O, alpha::P, alpha::Q, alpha::R, alpha::S, alpha::T, alpha::U, alpha::V, alpha::W, alpha::X, alpha::Y, alpha::Z };

	bool improved = true;
	while (improved)
	{
//		std::cout << "Base = " << scr << '\n';
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ tst[fi] };
			for (int ti = fi; ti < alpha_max; ++ti)
			{
				modalpha t{ tst[ti] };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = eval_fn(std::begin(vo), std::end(vo));
				if (scrn > scr && m3.SteckerCount() < max_stecker + 1)
				{
//					std::cout << "Step " << f << " - " << t << " get " << scrn << '\n';
					mx = f;
					my = t;
					scr = scrn;
					improved = true;
				}
				m3.PopStecker();
			}
		}
		if (improved)
		{
//			std::cout << "Apply " << mx << " - " << my << " get " << scr << '\n';
			m3.ApplyPlug(mx, my);
			std::next_permutation(std::begin(tst), std::end(tst));
		}
	}
	mst = m3.machine_settings();
	return scr;
}
