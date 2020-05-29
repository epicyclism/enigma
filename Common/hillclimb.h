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

template<typename IC, typename F, typename FD > auto hillclimb_base_fast(IC ctb, IC cte, F eval_fn, double iocb, FD& fd, stecker& s_base)
{
	constexpr size_t max_stecker = 10;
	stecker s = s_base;
	stecker s_b;
	auto vo = fd.decode(ctb, cte, s);
	auto iocs = index_of_coincidence(vo.begin(), vo.end());
	if (iocs * .85 < iocb)
		return 0U;
	// establish the baseline
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
		std::cout << "        tst " << f << " to " << t << " {" << scrn << "}\n";
		if (scrn > scr)
		{
			mt = t;
			scr = scrn;
		}
		m3.PopStecker();
	}

	return std::make_pair( scr, mt );
}

template<typename IC, typename F, typename FD > auto single_pass_fast(IC ctb, IC cte, F eval_fn, FD& fd, stecker& s_base)
{
	stecker s = s_base;
	stecker s_b;
	auto vo = fd.decode(ctb, cte, s);
	// establish the baseline
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	bool improved =  false;
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
			if (scrn > scr)
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
	s_base = s;

	return scr;
}


// use the fast decoder
template<typename IC, typename F> auto hillclimb_partial_exhaust2_fast(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (t2 == t1 || t2 == f1)
				continue;
			s.Set(f2, t2);
			s.Set(f1, t1);
			auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
			if(scrn > scr)
			{
				s_best = s;
				scr = scrn;
			}
			s.Clear();
		}
	}
	mst.stecker_ = s_best;
	return scr;
}

template<typename IC, typename F> auto hillclimb_partial_exhaust3_fast(IC ctb, IC cte, F eval_fn, modalpha f1, modalpha f2, modalpha f3, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
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
				auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
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
#if 0
template<typename IC, typename EC, typename F> auto hillclimb_partial_exhaust_fast(IC ctb, IC cte, EC bsb, EC bse, F eval_fn, machine_settings_t& mst)
{
	constexpr modalpha f1 = alpha::E;
	constexpr modalpha f2 = alpha::N;
	constexpr modalpha f3 = alpha::S;

	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
	while (bsb != bse)
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
				s_b = s;
				s.Apply(f3, t3);
				s.Apply(f2, t2);
				s.Apply(f1, *bsb);
				auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
				if (scrn > scr)
				{
					s_best = s;
					scr = scrn;
				}
				s = s_b;
			}
		}
		++bsb;
	}
	mst.stecker_ = s_best;
	return scr;
}
#else
template<typename IC, typename EC, typename F> auto hillclimb_partial_exhaust_fast(IC ctb, IC cte, EC bsb, EC bse, F eval_fn, machine_settings_t& mst)
{
	constexpr modalpha f1 = alpha::E;
	constexpr modalpha f2 = alpha::N;
	constexpr modalpha f3 = alpha::S;

	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
	for (int ti2 = 0; ti2 < alpha_max; ++ti2)
	{
		modalpha t2{ ti2 };
		if (t2 == f1)
			continue;
		for (int ti3 = 0; ti3 < alpha_max; ++ti3)
		{
			modalpha t3{ ti3 };
			if (t3 == t2 || t3 == f1 || t3 == f2)
				continue;
			for (auto bs = bsb; bs != bse; ++bs)
			{
				m3.PushStecker();
				s_b = s;
				s.Apply(f3, t3);
				s.Apply(f2, t2);
				s.Apply(f1, *bs);
				auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
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
#endif
template<typename IC, typename F> auto hillclimb_partial_exhaust_all(IC ctb, IC cte, F eval_fn, machine_settings_t& mst)
{
	constexpr modalpha f1 = alpha::E;
	constexpr modalpha f2 = alpha::N;
	constexpr modalpha f3 = alpha::S;

	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	stecker s_b;
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
	for (int ti2 = 0; ti2 < alpha_max; ++ti2)
	{
		modalpha t2{ ti2 };
		if (t2 == f1)
			continue;
		for (int ti3 = 0; ti3 < alpha_max; ++ti3)
		{
			modalpha t3{ ti3 };
			if (t3 == t2 || t3 == f1 || t3 == f2)
				continue;
			for (int ti1 = 0; ti1 != alpha_max; ++ti1)
			{
				modalpha t1{ ti1 };
				m3.PushStecker();
				s_b = s;
				s.Apply(f3, t3);
				s.Apply(f2, t2);
				s.Apply(f1, t1);
				auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
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
// fast(er) version of the arena practice of bg->tg pair
//
// viz
//
// 	         hillclimb_base(std::begin(ct), std::end(ct), bigram_score_op(), r.mst_);
//  r.btg_ = hillclimb_base(std::begin(ct), std::end(ct), trigram_score_op(), r.mst_);
//
template<typename IC> auto hillclimb_bgtg_fast(IC ctb, IC cte, machine_settings_t& mst)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	fast_decoder fd(m3);
	stecker s = mst.stecker_;
	hillclimb_base_fast(ctb, cte, bigram_score_op(), 0.0, fd, s);
	auto rv = hillclimb_base_fast(ctb, cte, trigram_score_op(), 0.0, fd, s);
	mst.stecker_ = s;
	return rv;
}

template<typename IC, typename AI> auto hillclimb_bgtg_fast(IC ctb, IC cte, AI ai, machine_settings_t& mst)
{
	fast_decoder_ref fd(ai);
	stecker s = mst.stecker_;
	hillclimb_base_fast(ctb, cte, bigram_score_op(), 0.0, fd, s);
	auto rv = hillclimb_base_fast(ctb, cte, trigram_score_op(), 0.0, fd, s);
	mst.stecker_ = s;
	return rv;
}


template<typename IF, typename IT> std::vector<std::array<std::pair<modalpha, modalpha>, 2>> make_plug_list2(IF fb, IF fe, IT tb, IT te)
{
	std::vector<std::array<std::pair<modalpha, modalpha>, 2>> rv;
	if (std::distance(fb, fe) > 1 && std::distance(tb, te) > 1) // single plug doesn't apply
	{
		auto fb1 = fb;
		while (fb1 != fe)
		{
			auto tb1 = tb;
			while (tb1 != te)
			{
				auto fb2 = fb1 + 1;
				while (fb2 != fe)
				{
					auto tb2 = tb1 + 1;
					while (tb2 != te)
					{
						if (!(fb1 == fb2 || tb1 == tb2))
						{
							std::array<std::pair<modalpha, modalpha>, 2> a;
							a[0] = std::make_pair(*fb1, *tb1);
							a[1] = std::make_pair(*fb2, *tb2);
							rv.emplace_back(a);
						}
						++tb2;
					}
					++fb2;
				}
				++tb1;
			}
			++fb1;
		}
	}

	return rv;
}


template<typename IT> std::vector<std::array<std::pair<modalpha, modalpha>, 2>> make_plug_list2(IT tb, IT te)
{
	std::vector<std::array<std::pair<modalpha, modalpha>, 2>> rv;
	auto fb1 = tb;
	while (fb1 != te)
	{
		auto tb1 = fb1 + 1;
		while (tb1 != te)
		{
			auto fb2 = tb;
			while (fb2 != te)
			{
				auto tb2 = fb2 + 1;
				while (tb2 != te)
				{
					if (!(fb1 == fb2 || tb1 == tb2))
					{
						std::array<std::pair<modalpha, modalpha>, 2> a;
						a[0] = std::make_pair(*fb1, *tb1);
						a[1] = std::make_pair(*fb2, *tb2);
						rv.emplace_back(a);
					}
					++tb2;
				}
				++fb2;
			}
			++tb1;
		}
		++fb1;
	}

	return rv;
}

// SC is an iterator to an array of arrays of stecker pairs.
//
template<typename IC, typename SC, typename F, typename AI> auto hillclimb_specific_exhaust_fast(IC ctb, IC cte, SC sb, SC se, F eval_fn, AI ai, machine_settings_t& mst)
{
	fast_decoder_ref fd(ai);
	stecker s_best;
	// establish the baseline
	auto vo = fd.decode(ctb, cte, s_best);
	auto scr = eval_fn(std::begin(vo), std::end(vo));
	auto iocb = index_of_coincidence(vo.begin(), vo.end());
	std::for_each(sb, se, [&](auto const& sa)
		{
			stecker s;
			for (auto const& p : sa)
			{
				s.Set(p.first, p.second);
			}
			auto scrn = hillclimb_base_fast(ctb, cte, eval_fn, iocb, fd, s);
			if (scrn > scr)
			{
				s_best = s;
				scr = scrn;
			}
		});
	mst.stecker_ = s_best;
	return scr;
}
