#pragma once

#include "modalpha.h"
//#include "ioc.h"
//#include "bigram.h"
//#include "trigram.h"

// count possible plug, includes a flag to work the matching algorithm
//
struct plug_stat_chk
{
	modalpha f_;
	modalpha t_;
	bool     b_;
	unsigned cnt_;
	double   ioc_;
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
	unsigned sig_lim_; // number of matches considered significant, related to the ciphertext size for now.
public:
	plug_set_msg()
	{
		clear();
	}
	auto begin() noexcept { return psm_.begin(); }
	auto end()   noexcept { return psm_.begin() + end_; }
	auto begin() const noexcept  { return psm_.begin(); }
	auto end()   const noexcept  { return psm_.begin() + end_; }
	auto size()  const noexcept { return end_; }
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
	void merge_direct_exp(modalpha f, modalpha t)
	{
		if (f == t)
			return;
		if (f > t)
			std::swap(f, t);
		auto it = std::begin(psm_);
		while (it != std::end(psm_))
		{
			auto& pg = *it;
			if (pg.f_ == f && pg.t_ == t)
			{
				pg.cnt_ += direct_;
				direct_ = 0;
				break;
			}
			else
			if (pg.f_ == alpha::SZ)
			{
				pg.f_ = f;
				pg.t_ = t;
				pg.cnt_ = direct_ ;
				direct_ = 0;
				++end_;
				break;
			}
			++it;
		}
	}
	void merge_direct_force(modalpha f, modalpha t)
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
				pg.cnt_ = total_;
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
	void unique()
	{
		unsigned unique_ = 0;
		std::for_each(std::begin(psm_), std::begin(psm_) + end_,
			[&](auto& v)
			{
				if (v.cnt_ < 2)
					v.cnt_ = 0;
				else
				if (modalpha_is_bit(v.f_, unique_) || modalpha_is_bit(v.t_, unique_))
					v.cnt_ = 0;
				else
				{
					unique_ = modalpha_set_bit(v.f_, unique_);
					unique_ = modalpha_set_bit(v.t_, unique_);
				}
			});
		set_end(std::remove_if(std::begin(psm_), std::begin(psm_) + end_, [](auto& v) { return v.cnt_ == 0; }));
	}
	void prune(unsigned mx)
	{
		std::for_each(std::begin(psm_), std::begin(psm_) + end_,
			[&](auto& v)
			{
				if (v.cnt_ < mx)
					v.cnt_ = 0;
			});
		set_end(std::remove_if(std::begin(psm_), std::begin(psm_) + end_, [](auto& v) { return v.cnt_ == 0; }));
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
	template<typename O> void print_ioc(O& ostr)
	{
		std::for_each(std::begin(psm_), std::begin(psm_) + end_, 
			[&](auto const& v)
			{
				if(v.cnt_)
					ostr << v.f_ << "<->" << v.t_ << " - " << v.cnt_ << " : " << v.ioc_ << "\n"; 
			});
		ostr << "Count  = " << total_ << "\n";
	}
};

// returns our pseudo 'E' score, and fills out the plug set that corresponds
// the caller can then judge whether to record or discard
// bs is the 'letter' attached to the input stream
//
template<typename IC, typename IA> unsigned match_ciphertext(IC ctb, IC cte, IA base, modalpha bs)
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
	// remove all cnt < ? and make others unique
	psm.unique();
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	auto ctl = std::distance(ctb, cte);
	return static_cast<unsigned>((std::accumulate(psm.begin(), pr, 0, [](auto& l, auto& r) { return l + r.cnt_; }) * 100 + ctl/2) / ctl );
}

template<typename IC, typename IA> unsigned match_ciphertext_exp(IC ctb, IC cte, IA base, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
		{
			psm.set(c, *base);
			++base;
		});
	if (bs != alpha::E)
	{
		psm.merge_direct_exp(bs, alpha::E);
	}
	// sort highest cnt first
	std::sort(std::begin(psm), std::end(psm), [](auto const& l, auto const& r) { return l.cnt_ > r.cnt_; });
	// remove all cnt < 2 and make others unique
	psm.unique();
	//psm.print(std::cout);
	auto pr = psm.begin() + (psm.size() > 10 ? 10 : psm.size());
	auto ctl = std::distance(ctb, cte);
	double dbit = 10.0 / (0.14 * ctl);
	return static_cast<unsigned>(std::accumulate(psm.begin(), pr, 0.0, [dbit](auto& l, auto& r) { return l + dbit * r.cnt_ * r.cnt_; }));
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
//	psm.merge_direct_force(bs, alpha::E); // force ensures that the bs->E pair gets priority, whatever the evidence. It's our core assumption...
	psm.merge_direct_exp(bs, alpha::E); // force ensures that the bs->E pair gets priority, whatever the evidence. It's our core assumption...

	return psm ;
}

template<typename I, size_t W> void match_search(I cb, I ce, std::array<modalpha, W> const& row, size_t len, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = itb + len - std::distance(cb, ce);
	auto ito = std::begin(counts);

	while (itb != ite)
	{
		*ito = match_ciphertext(cb, ce, itb, bs);
		++ito;
		++itb;
	}
}

template<typename I, size_t W> void match_search_exp(I cb, I ce, std::array<modalpha, W> const& row, size_t len, std::array<unsigned, W>& counts, modalpha bs)
{
	auto itb = std::begin(row);
	auto ite = itb + len - std::distance(cb, ce);
	auto ito = std::begin(counts);

	while (itb != ite)
	{
		*ito = match_ciphertext_exp(cb, ce, itb, bs);
		++ito;
		++itb;
	}
}

