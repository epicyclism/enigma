#pragma once

#include "modalpha.h"
#include "ioc.h"
#include "bigram.h"
#include "trigram.h"

// count possible plug, includes a flag to work the matching algorithm
//
struct plug_stat_chk
{
	modalpha f_;
	modalpha t_;
	bool     b_;
	int      cnt_;
};

// assumed max message length
// collect all possible plugs for this message, with their frequency
class plug_set_msg
{
private:
	std::array<plug_stat_chk, 256> psm_;
	ptrdiff_t end_;
	unsigned direct_;
	unsigned total_;
public:
	plug_set_msg()
	{
		clear();
	}
	auto begin() noexcept { return psm_.begin(); }
	auto end()   noexcept { return psm_.begin() + end_; }
	auto begin() const noexcept  { return psm_.begin(); }
	auto end()   const noexcept  { return psm_.begin() + end_; }
	template<typename I> void set_end(I e) noexcept
	{
		end_ = std::distance(psm_.begin(), e);
	}
	void clear() noexcept
	{
		for (auto& s : psm_)
		{
			s.f_ = alpha::SZ;
			s.t_ = alpha::SZ;
			s.cnt_ = 0;
			s.b_ = false;
		}
		end_ = 0;
		direct_ = 0;
		total_ = 0;
	}
	void set(modalpha f, modalpha t) noexcept
	{
		++total_;
		if (f == t)
		{
			++direct_;
			return;
		}
		if (f > t)
		{
			std::swap(f, t);
		}
		auto it = std::begin(psm_);
		while (it != std::end(psm_))
		{
			auto& pg = *it;
			if (pg.f_ == alpha::SZ)
			{
				pg.f_ = f;
				pg.t_ = t;
				++pg.cnt_;
				++end_;
				break;
			}
			if ((pg.f_ == f) && (pg.t_ == t))
			{
				++pg.cnt_;
				break;
			}
			++it;
		}
	}
	void merge_direct(modalpha f, modalpha t)
	{
		if (f == t)
		{
			direct_ *= 5;
			direct_ /= 25;
			return;
		}
		if (f > t)
			std::swap(f, t);
		auto it = std::begin(psm_);
		while (it != std::end(psm_))
		{
			auto& pg = *it;
			if (pg.f_ == alpha::SZ)
			{
				pg.f_ = f;
				pg.t_ = t;
				pg.cnt_ = (direct_ * 7) / 25;
				direct_ = 0;
				++end_;
				break;
			}
			++it;
		}
	}
	unsigned direct() const
	{
		return direct_;
	}
	template<typename O> void print(O& ostr)
	{
		std::for_each(std::begin(psm_), std::begin(psm_) + end_, 
			[&](auto const& v)
			{
				ostr << v.f_ << "<->" << v.t_ << " - " << v.cnt_ << "\n"; 
			});
		ostr << "Direct = " << direct_ << "\n";
		ostr << "Count  = " << total_ << "\n";
	}
};

// returns our pseudo 'E' score, and fills out the plug set that corresponds
// the caller can then judge whether to record or discard
// bs is the 'letter' attached to the input stream
//
template<typename IC, typename IA> int match_ciphertext(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
	{
		psm.set( c, *base);
		++base;
	});
	if (bs != alpha::E)
	{
		psm.merge_direct(bs, alpha::E);
	}
	// work out the 10 best...
//	return nbest(psm.begin(), psm.end()) ;
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt = '1' entries
	psm.set_end(std::find_if(std::begin(psm), std::end(psm), [](auto& v) { return v.cnt_ == 1; }));
	return std::accumulate(psm.begin(), psm.begin() + 10, 0, [](auto& l, auto& r) { return l + r.cnt_; }) * 100 / std::distance(ctb, cte);
}

template<typename IC, typename IA> plug_set_msg match_ciphertext_psm(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
	{
		psm.set( c, *base);
		++base;
	});
	psm.merge_direct(bs, alpha::E);
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt = '1' entries
	psm.set_end(std::find_if(std::begin(psm), std::end(psm), [](auto& v) { return v.cnt_ == 1; }));
//	psm.print(std::cout);

	return psm ;
}

template<typename IC, typename IA>
unsigned match_worker(IC ctb, IC cte, IA base, modalpha bs)
{
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);

	return std::accumulate(psm.begin(), psm.begin() + 10, 0, [](auto& l, auto& r) { return l + r.cnt_; }) * 100 / std::distance(ctb, cte);
}

template<typename I, size_t W> void match_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);

	while (itb != ite)
	{
//		*ito += match_ciphertext(cb, ce, itb, bs);
		*ito = match_worker(cb, ce, itb, bs);
		++ito;
		++itb;
	}
}

template<typename IC, typename IA, typename R> void use_ees(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const & mst_j, R& r)
{
	// collect the likely candidate pairs
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);
	psm.print(std::cout);
	// prepare a machine
	machine_settings_t mst(mst_j);
	machine3 m3 = MakeMachine3(mst);
	m3.Position(pos);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
#if 0
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
#endif
	r.emplace_back(m3.machine_settings(), ioc);
}

template<typename IC> void hillclimb_test(IC ctb, IC cte, position const& pos, modalpha bs, machine_settings_t const & mst_j)
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
	auto scr = bigram_score(std::begin(vo), std::end(vo)) ;
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
				auto scrn = bigram_score(std::begin(vo), std::end(vo)) ;
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
	r.emplace_back(m3.machine_settings(), scr);
}

// in and out on the machine settings and the score
//
template<typename IC> void hillclimb(IC ctb, IC cte, machine_settings_t& mst, unsigned& scr_out)
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

