#pragma once

#include <numeric>
#include <tuple>

#include "modalpha.h"
#include "ioc.h"
#include "unigram.h"
#include "bigram.h"
#include "trigram.h"
#include "fast_decoder.h"

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
			m3.ClearPlugs(); // clears
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
	m3.ClearPlugs(); // clears
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

template<typename IC, typename F, size_t max_stecker = 10 > auto hillclimb_base_fast(IC ctb, IC cte, F eval_fn, fast_decoder const& fd, stecker& s_base)
{
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	stecker s = s_base;
	stecker s_b;
	vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	s_base.Report(std::cout);
	std::cout << " ioc = " << index_of_coincidence(vo.begin(), vo.end()) << '\n';
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
				s_b = s;
				s.Apply(f, t);
				vo = fd.decode(ctb, cte, s);
				auto scrn = eval_fn(std::begin(vo), std::end(vo));
				if (scrn > scr && s.Count() < max_stecker + 1)
				{
					mx = f;
					my = t;
					scr = scrn;
					improved = true;
				}
				s = s_b;
			}
		}
		if (improved)
			s.Apply(mx, my);
	}
	s_base = s;

	return scr;
}

template<typename IC, typename F, size_t max_stecker = 10 > auto hillclimb_filter(IC ctb, IC cte, F eval_fn, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	if (ioc < 0.044)
		return 0U;
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
//	std::array<modalpha, alpha_max> tst { alpha::A, alpha::B, alpha::C, alpha::D, alpha::E, alpha::F, alpha::G, alpha::H, alpha::I, alpha::J, alpha::K, alpha::L, alpha::M, alpha::N, alpha::O, alpha::P, alpha::Q, alpha::R, alpha::S, alpha::T, alpha::U, alpha::V, alpha::W, alpha::X, alpha::Y, alpha::Z };
	std::array<modalpha, alpha_max> tst = gen_freq_seq(ctb, cte);

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
//			std::next_permutation(std::begin(tst), std::end(tst));
		}
	}
	mst = m3.machine_settings();
	return scr;
}

// tries all connections from a 'f' including 'f' itself
// returns the score, modifies the machine_settings
//
template<typename IC, typename F> auto single_stecker(IC ctb, IC cte, F eval_fn, modalpha f, machine_settings_t& mst) 
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	modalpha mt = 0;
	for (int ti = 0; ti < alpha_max; ++ti)
	{
		modalpha t{ ti };
		m3.PushStecker();
		m3.ApplyPlug(f, t);
		decode(ctb, cte, m3, vo);
		auto scrn = eval_fn(std::begin(vo), std::end(vo));
		if (scrn > scr)
		{
			mt = t;
			scr = scrn;
		}
		m3.PopStecker();
	}

	return std::make_pair( scr, mt );
}

// assumes an 'E' connection has already been made, iterates another two exhaustively
//
template<typename IC, typename F> auto hillclimb_partial_exhaust2(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		if (t1 == alpha::E || ti1 == f2)
			continue;
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (ti2 == ti1 || t2 == alpha::E || t2 == f1 )
				continue;
			m3.PushStecker();
			m3.ApplyPlug(f2, t2);
			m3.ApplyPlug(f1, t1);
			auto mstt = m3.machine_settings();
			auto scrn = hillclimb_base(ctb, cte, eval_fn, mstt);
			if (scrn > scr)
			{
				mst = mstt;
				scr = scrn;
			}
			m3.PopStecker();
		}
	}
	return scr;
}

template<typename IC, typename F> auto hillclimb_partial_exhaust3(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, modalpha f3, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (ti1 == ti2 || ti2 == f1)
				continue;
			for (int ti3 = 0; ti3 < alpha_max; ++ti3)
			{
				if (ti3 == ti2 || ti3 == ti1 || ti3 == f1 || ti3 == f2)
					continue;
				modalpha t3{ ti3 };
				m3.PushStecker();
				m3.ApplyPlug(f3, t3);
				m3.ApplyPlug(f2, t2);
				m3.ApplyPlug(f1, t1);
				auto mstt = m3.machine_settings();
				auto scrn = hillclimb_base(ctb, cte, eval_fn, mstt);
				if (scrn > scr)
				{
					mst = mstt;
					scr = scrn;
				}
				m3.PopStecker();
			}
		}
	}
	return scr;
}

// use the fast decoder, for test
template<typename IC, typename F> auto hillclimb_partial_exhaust2_fast(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		if (t1 == alpha::E || ti1 == f2)
			continue;
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (ti2 == ti1 || t2 == alpha::E || t2 == f1)
				continue;
			s_b = s;
			s.Apply(f2, t2);
			s.Apply(f1, t1);
			auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, fd, s);
			if (scrn > scr)
			{
				s_best = s;
				scr = scrn;
			}
			s = s_b;
		}
	}
	mst.stecker_ = s_best;
	return scr;
}

template<typename IC, typename F> auto hillclimb_partial_exhaust3_fast(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, modalpha f3, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (ti1 == ti2 || ti2 == f1)
				continue;
			for (int ti3 = 0; ti3 < alpha_max; ++ti3)
			{
				if (ti3 == ti2 || ti3 == ti1 || ti3 == f1 || ti3 == f2)
					continue;
				modalpha t3{ ti3 };
				s_b = s;
				s.Apply(f3, t3);
				s.Apply(f2, t2);
				s.Apply(f1, t1);
				auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, fd, s);
				if (scrn > scr)
				{
					s_best = s;
					scr = scrn;
				}
				s = s_b;
			}
		}
	}
	mst.stecker_ = s_best;
	return scr;
}


template<typename IC, typename EC, typename F> auto hillclimb_partial_exhaust(IC ctb, IC cte, EC bsb, EC bse, F eval_fn, machine_settings_t& mst)
{
	constexpr modalpha f1 = alpha::E;
	constexpr modalpha f2 = alpha::N;
	constexpr modalpha f3 = alpha::S;

	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	while(bsb != bse)
	{
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (t2 == *bsb || t2 == f1)
				continue;
			for (int ti3 = 0; ti3 < alpha_max; ++ti3)
			{
				modalpha t3{ ti3 };
				if (t3 == t2 || t3 == *bsb || t3 == f1 || t3 == f2)
					continue;
				m3.PushStecker();
				m3.ApplyPlug(f3, t3);
				m3.ApplyPlug(f2, t2);
				m3.ApplyPlug(f1, *bsb);
				auto mstt = m3.machine_settings();
				auto scrn = hillclimb_filter(ctb, cte, eval_fn, mstt);
				if (scrn > scr)
				{
					mst = mstt;
					scr = scrn;
				}
				m3.PopStecker();
			}
		}
		++bsb;
	}
	return scr;
}