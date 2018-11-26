#pragma once

#include "modalpha.h"

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
public:
	plug_set_msg()
	{
		clear();
	}
	auto begin() noexcept { return psm_.begin(); }
	auto end()   noexcept { return psm_.begin() + end_; }
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
	}
	void set(modalpha f, modalpha t) noexcept
	{
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
			return;
		if (f > t)
			std::swap(f, t);
		int cnt = 0;
		for (auto& n : psm_)
		{
			if (n.f_ == n.t_ && n.cnt_ > 1)
			{
				cnt += n.cnt_;
				n.cnt_ = 0;
			}
		}
		auto it = std::begin(psm_);
		while (it != std::end(psm_))
		{
			auto& pg = *it;
			if (pg.f_ == alpha::SZ)
			{
				pg.f_ = f;
				pg.t_ = t;
				pg.cnt_ = cnt;
				++end_;
				break;
			}
			if (pg.f_ == f && pg.t_ == t)
			{
				pg.cnt_ += cnt;
				break;
			}
			++it;
		}
	}
	template<typename O> void print(O& ostr)
	{
		std::sort(std::begin(psm_), std::begin(psm_) + end_,
			[](auto const& l, auto const& r)
			{
				return l.cnt_ > r.cnt_;
			});
		std::for_each(std::begin(psm_), std::begin(psm_) + end_, 
			[&ostr](auto const& v)
			{
				ostr << v.f_ << "<->" << v.t_ << " - " << v.cnt_ << "\n"; 
			});
	}
};

struct link
{
	modalpha f_, t_;
	int score_;
	bool is(modalpha f, modalpha t)
	{
		return f == f_ || f == t_ || t == f_ || t == t_;
	}
};

class linkset
{
private:
	std::array<link, 10> links_;
	int anti_score_;
public:
	linkset() noexcept
	{
		for (auto& l : links_)
			l.score_ = 0;
		anti_score_ = 0;
	}
	auto begin() const
	{
		return std::begin(links_);
	}
	auto end() const
	{
		return std::end(links_);
	}
	// returns true if link inserted.
	//
	bool insertlink(modalpha f, modalpha t, int score) noexcept
	{
		// look for an existing connection to one of our ends
		for (auto& l : links_)
		{
			if (l.is(f, t))
			{
				if (l.score_ < score)
				{
					anti_score_ += l.score_;
					// replace
					l.f_ = f;
					l.t_ = t;
					l.score_ = score;
					return true;
				}
				else
				{
					anti_score_ += score;
					return false;
				}
			}
		}
		// then look for an empty entry
		for (auto& l : links_)
		{
			if (l.score_ == 0)
			{
				l.f_ = f;
				l.t_ = t;
				l.score_ = score;
				return true;
			}
		}
		// last look for a lesser entry
		auto lst = std::min_element(std::begin(links_), std::end(links_), [](auto& l, auto& r) { return l.score_ < r.score_; });
		if ((*lst).score_ < score)
		{
			anti_score_ += (*lst).score_;
			// replace
			(*lst).f_ = f;
			(*lst).t_ = t;
			(*lst).score_ = score;
			return true;
		}
		// failure to improve with this link
		anti_score_ += score;
		return false;
	}
	int score() const noexcept
	{
		return std::accumulate(std::begin(links_), std::end(links_), 0, [](auto l, auto r) { return l + r.score_; });
	}
	int anti_score() const noexcept
	{
		return anti_score_;
	}
	template<typename O> void report(O& ostr)
	{
		for (auto& l : links_)
		{
			ostr << l.f_ << l.t_ << " ";
		}
		ostr << "\n";
	}
};

template<typename I> int nbest(I b, I e)
{
	linkset ls;
	bool bchg = false;
	do
	{
		bchg = false;
		auto bb = b;
		while (bb != e)
		{
			// try and insert *bb to the advantage
			if ((*bb).cnt_ > 1 && ls.insertlink((*bb).f_, (*bb).t_, (*bb).cnt_))
				bchg = true;
			++bb;
		}
	} while (bchg);
//	ls.report(std::cout);
//	std::cout << "nbest " << ls.score() << ", " << ls.anti_score() << "\n";
	return ls.score() - (ls.anti_score() / 16);
}

template<typename I> linkset nbest_get(I b, I e)
{
	linkset ls;
	bool bchg = false;
	do
	{
		bchg = false;
		auto bb = b;
		while (bb != e)
		{
			// try and insert *bb to the advantage
			if ((*bb).cnt_ > 1 && ls.insertlink((*bb).f_, (*bb).t_, (*bb).cnt_))
				bchg = true;
			++bb;
		}
	} while (bchg);

	return ls;
}

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
	return nbest(psm.begin(), psm.end()) ;
}

template<typename IC, typename IA> linkset match_ciphertext_get(IC ctb, IC cte, IA base, modalpha bs)
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
//	psm.print(std::cout);
	// work out the 10 best...
	return nbest_get(psm.begin(), psm.end()) ;
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
	if (bs != alpha::E)
	{
		psm.merge_direct(bs, alpha::E);
	}
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt = '1' entries
	psm.set_end(std::find_if(std::begin(psm), std::end(psm), [](auto& v) { return v.cnt_ == 1; }));
//	psm.print(std::cout);

	return psm ;
}

template<typename I, size_t W> void match_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);

	while (itb != ite)
	{
		*ito += match_ciphertext(cb, ce, itb, bs);
		++ito;
		++itb;
	}
}

template<typename IC, typename O> void decode(IC ctb, IC cte, machine3& m3, O& o)
{
	o.clear();
	position pos = m3.Position();
	auto ct = ctb;
	while (ct != cte)
	{
		o.push_back(m3.Transform(*ct));
		++ct;
	}
	// reset machine
	m3.Position(pos);
}

template<typename IC, typename IA, typename R> void use_ees(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const & mst_j, R& r)
{
	// collect the likely candidate pairs
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);
	// prepare a machine
	machine_settings_t mst(mst_j);
	machine3 m3 = MakeMachine3(mst);
	m3.Position(pos);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
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
	if ( ioc > 0.070)
		r.emplace_back(m3.machine_settings(), ioc);
}

template<typename IC, typename IA> machine_settings_t use_ees_test(IC ctb, IC cte, IA base, position const& pos, modalpha bs, machine_settings_t const & mst_j)
{
	// collect the likely candidate pairs
	auto psm = match_ciphertext_psm(ctb, cte, base, bs);
	// prepare a machine
	machine_settings_t mst(mst_j);
	machine3 m3 = MakeMachine3(mst);
	m3.Position(pos);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	// establish the baseline
	decode(ctb, cte, m3, vo);
	auto ioc = index_of_coincidence(std::begin(vo), std::end(vo));
	// now for each candidate, remove a clash, apply it, if ioc improves keep it.
	for (auto& s : psm)
	{
		m3.PushStecker();
		m3.ApplyPlug(s.f_, s.t_);
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
		if (ioc > iocn) // undo
		{
			m3.PopStecker();
		}
		else // keep!
		{
			ioc = iocn;
		}
	}
	m3.ReportSettings(std::cout);
	decode(ctb, cte, m3, vo);
	std::cout << " - ";
	std::cout << ioc << " - ";
	for (auto c : vo)
		std::cout << c;
	std::cout << "\n";
	return m3.machine_settings();
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

template<typename I, size_t W> void match_test(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
	auto i = match_ciphertext(cb, ce, itb, bs);
	std::cout << i << "\n";
	i = match_ciphertext(cb, ce, itb + 1, bs);
	std::cout << i << "\n";
}
