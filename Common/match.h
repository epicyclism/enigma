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
	modalpha f_ = 0
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
	int direct_;
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
		direct_ = 0;
		end_ = 0;
	}
	void set(modalpha f, modalpha t) noexcept
	{
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
	void set_direct(modalpha f, modalpha t)
	{
		if (f == t)
			return;
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
				pg.cnt_ = direct_;
				++end_;
				break;
			}
			++it;
		}
	}
	// where the magic happens
	//
	plug_set algorithm()
	{
		plug_set ps;
		bool again = false;
		do
		{
			auto b = psm_.begin();
			auto e = psm_.end();
			int n = 0;
			while (b != e)
			{
				again = ps.apply(*b, n);
				++b;
				++n;
			}
		} while (again);

		return ps;
	}
	template<typename O> void print(O& ostr)
	{
		ostr << "direct = " << direct_ << "\n";
		for (auto& s : psm_)
		{
			ostr << s.f_ << "<->" << s.t_ << " - " << s.cnt_ << "\n";
		}
	}
};

// returns our pseudo 'E' score, and fills out the plug set that corresponds
// the caller can then judge whether to record or discard
// bs is the 'letter' attached to the input stream
//
template<typename IC, typename IA> int match_ciphertext(IC ctb, IC cte, IA base, plug_best& ps, modalpha bs)
{
	plug_set_msg psm;
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &psm](auto const c)
	{
		psm.set( c, *base);
		++base;
	});
	// add the 'direct'
	psm.set_direct(bs, alpha::E);
#if 1
	// build the best list

#else
	// order for each possible letter
	plug_prune pp;
	
	// install implicit plug from base
	if (bs != alpha::E)
	{
		pp[modalpha(alpha::E).Val()].t_ = bs;
		pp[modalpha(alpha::E).Val()].cnt_ = ps.direct_;
	}
	for (auto& plug : ps)
	{
		if (pp[plug.f_.Val()].cnt_ < plug.cnt_)
		{
			pp[plug.f_.Val()].t_    = plug.t_;
			pp[plug.f_.Val()].cnt_  = plug.cnt_;
		}
		else
		if (pp[plug.t_.Val()].cnt_ < plug.cnt_)
		{
			pp[plug.t_.Val()].t_    = plug.f_;
			pp[plug.t_.Val()].cnt_  = plug.cnt_;
		}
	}
	print(pp);

	// want ten best
#endif
	return 0;
}

