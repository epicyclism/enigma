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
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	for (auto& s : psm)
	{
		m3.PushStecker();
		m3.ApplyPlug(s.f_, s.t_);
		decode(ctb, cte, m3, vo);
		s.ioc_ = index_of_coincidence(std::begin(vo), std::end(vo));
	}
//	psm.print_ioc(std::cout);
	// sort good->bad
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.ioc_ > r.ioc_; });
	// remove all detrimental options
	psm.set_end(std::find_if(std::begin(psm), std::end(psm), [ioc](auto& v) { return v.ioc_ < ioc; }));
//	psm.print_ioc(std::cout);
	// apply
	auto pr = psm.begin() + 10;
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


template<typename IC, typename IA, typename R> void use_ees_back(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const& mst_j, R& r)
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
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	auto iocb = ioc;
	// now for each candidate, remove a clash, apply it, if ioc improves keep it.
	// go from back, least interesting, to front, most interesting
	auto pr = psm.end();
	do
	{
		--pr;
		auto& s = *pr;
		m3.PushStecker();
		m3.ApplyPlug(s.f_, s.t_);
		decode(ctb, cte, m3, vo);
		auto iocn = index_of_coincidence(std::begin(vo), std::end(vo));
		if (ioc > iocn) // undo
			m3.PopStecker();
		else // keep!
			ioc = iocn;
	} while (pr != psm.begin());
#if 0
	std::cout << "ioc from " << iocb << " to " << ioc << '\n';
	// report
	m3.ReportSettings(std::cout);
	std::cout << " = ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";
#endif
	r.emplace_back(m3.machine_settings(), ioc);
}

template<typename IC, typename IA, typename R> void use_ees_forward(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const& mst_j, R& r)
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
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	auto iocb = ioc;
	// now for each candidate, remove a clash, apply it, if ioc improves keep it.
	for (auto& s : psm)
	{
		m3.PushStecker();
		m3.ApplyPlug(s.f_, s.t_);
		decode(ctb, cte, m3, vo);
		auto iocn = index_of_coincidence(std::begin(vo), std::end(vo));
		if (ioc > iocn) // undo
			m3.PopStecker();
		else // keep!
			ioc = iocn;
	}
	std::cout << "ioc from " << iocb << " to " << ioc << '\n';
	// report
	m3.ReportSettings(std::cout);
	std::cout << " = ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";

	r.emplace_back(m3.machine_settings(), ioc);
}

template<typename IC> void hillclimb_test(IC ctb, IC cte, position const& pos, modalpha bs, machine_settings_t const& mst_j)
{
	// prepare a machine
	machine_settings_t mst(mst_j);
	machine3 m3 = MakeMachine3(mst);
	m3.Position(pos);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	for (int cnt = 0; cnt < 5; ++cnt)
	{
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi + 1; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto iocn = index_of_coincidence(std::begin(vo), std::end(vo));
#if 0
				m3.ReportSettings(std::cout);
				std::cout << " - ";
				std::cout << iocn << " - ";
				for (auto c : vo)
					std::cout << c;
				std::cout << "\n";
#endif
				if (iocn > ioc)
				{
					mx = f;
					my = t;
					ioc = iocn;
				}
				m3.PopStecker();
			}
		}
		m3.ApplyPlug(mx, my);
		m3.ReportSettings(std::cout);
		decode(ctb, cte, m3, vo);
		std::cout << " - ";
		std::cout << ioc << " - ";
		for (auto c : vo)
			std::cout << c;
		std::cout << "\n";
	}
}

template<typename IC, typename R> void hillclimb(IC ctb, IC cte, machine_settings_t mst, R& r)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = bigram_score(std::begin(vo), std::end(vo));
	auto scrb = scr;
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
//			for (int ti = fi + 1; ti < alpha_max; ++ti)
			for (int ti = fi; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = bigram_score(std::begin(vo), std::end(vo));
				if (scrn > scr)
				{
//					std::cout << f << t << " " << scr << " -> " << scrn << '\n';
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
	std::cout << "bg from " << scrb << " to " << scr << '\n';
	r.emplace_back(m3.machine_settings(), scr);
}

template<typename IC, typename R> void hillclimb_tg(IC ctb, IC cte, machine_settings_t mst, R& r)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = trigram_score(std::begin(vo), std::end(vo));
	auto scrb = scr;
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
//			for (int ti = fi + 1; ti < alpha_max; ++ti)
			for (int ti = fi; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = trigram_score(std::begin(vo), std::end(vo));
				if (scrn > scr)
				{
					std::cout << f << t << " " << scr << " -> " << scrn << '\n';
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
//	std::cout << "bg from " << scrb << " to " << scr << '\n';
	r.emplace_back(m3.machine_settings(), scr);
}

template<typename IC, typename F, typename R> void hillclimb(IC ctb, IC cte, machine_settings_t mst, F fn, R& r)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = fn(std::begin(vo), std::end(vo));
	auto scrb = scr;
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi + 1; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = fn(std::begin(vo), std::end(vo));
				if (scrn > scr)
				{
					std::cout << f << t << " " << scr << " -> " << scrn << '\n';
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
	std::cout << "bg from " << scrb << " to " << scr << '\n';
	r.emplace_back(m3.machine_settings(), scr);
}

// in and out on the machine settings and the score
//
template<typename IC> void hillclimb2(IC ctb, IC cte, machine_settings_t& mst, unsigned& scr_out)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = bigram_score(std::begin(vo), std::end(vo));
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi + 1; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				//				if (fi == from_printable('F') && ti == from_printable('L'))
				//					std::cout << "Trying FL\n";
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = bigram_score(std::begin(vo), std::end(vo));
				if (scrn > scr)
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
	scr_out = scr;
}

// in and out on the machine settings and the score
//
template<typename IC> void hillclimb3(IC ctb, IC cte, machine_settings_t& mst, unsigned& scr_out)
{
	// prepare a machine
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto scr = trigram_score(std::begin(vo), std::end(vo));
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi + 1; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				m3.PushStecker();
				m3.ApplyPlug(f, t);
				decode(ctb, cte, m3, vo);
				auto scrn = trigram_score(std::begin(vo), std::end(vo));
				if (scrn > scr)
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
	scr_out = scr;
}

