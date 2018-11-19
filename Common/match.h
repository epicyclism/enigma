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
// for the outside world
//
struct plug
{
	modalpha t_ = 0;
	modalpha f_ = 0;
};

using plug_best = std::array<plug, 10>;

// this represents the actual estimate of the machine plugging for
// this message, private to our algorithm
struct plug_set
{
	std::array<plug_stat_chk, 10> pss_;
	plug_set()
	{
		clear();
	}
	void clear() noexcept
	{
		for (auto& p : pss_)
		{
			p.f_ = alpha::SZ;
			p.t_ = alpha::SZ;
			p.b_ = false;
			p.cnt_ = 0;
		}
	}
	void copy_out(plug_best& pb)
	{
		std::transform(std::begin(pss_), std::end(pss_), std::begin(pb), [](auto& p) { return plug{ p.f_, p.t_ }; });
	}
	bool apply(plug_stat_chk& psc, int n)
	{

	}
};

// assumed max message length
// collect all possible plugs for this message, with their frequency
class plug_set_msg
{
private:
	std::array<plug_stat_chk, 256> psm_;
	int end_;
public:
	plug_set_msg()
	{
		clear();
	}
	auto begin() noexcept { return psm_.begin(); }
	auto end()   noexcept { return psm_.begin() + end_; }

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
		std::sort(std::begin(psm_), std::end(psm_), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
		for (auto& s : psm_)
		{
			if ( s.cnt_ > 1)
				ostr << s.f_ << "<->" << s.t_ << " - " << s.cnt_ << "\n";
		}
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
public:
	linkset() noexcept
	{
		for (auto& l : links_)
			l.score_ = 0;
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
					// replace
					l.f_ = f;
					l.t_ = t;
					l.score_ = score;
					return true;
				}
				else
					return false;
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
			// replace
			(*lst).f_ = f;
			(*lst).t_ = t;
			(*lst).score_ = score;
			return true;
		}
		// failure to improve with this link
		return false;
	}
	int score() const noexcept
	{
		return std::accumulate(std::begin(links_), std::end(links_), 0, [](auto l, auto r) { return l + r.score_; });
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

	return ls.score();
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
//	psm.print(std::cout);
	// work out the 10 best...
	return nbest(psm.begin(), psm.end()) ;
}

template<typename I, size_t W> void match_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
#if 1
	while (itb != ite)
	{
		*ito += match_ciphertext(cb, ce, itb, bs);
		++ito;
		++itb;
	}
#else
	auto i = match_ciphertext(cb, ce, itb, bs);
	std::cout << i << "\n";
	i = match_ciphertext(cb, ce, itb + 1, bs);
	std::cout << i << "\n";
#endif
}
