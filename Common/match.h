#pragma once

#include "modalpha.h"

struct stkr
{
	modalpha f_;
	modalpha t_;
	short    cnt_;
};

// assumed max message length
struct stkrset
{
	std::array<stkr, 256> sss_;
	int direct_;
};

void clear(stkrset& ss)
{
	for (auto& s : ss.sss_)
	{
		s.f_ = alpha::SZ;
		s.t_ = alpha::SZ;
		s.cnt_ = 0;
	}
	ss.direct_ = 0;
}

void set(stkrset& ss, modalpha f, modalpha t)
{
	std::cout << f << ", " << t << "\n";
	if (f == t)
	{
		++ss.direct_;
		return;
	}
	if (f > t)
	{
		std::swap(f, t);
	}
	auto& sss = ss.sss_;
	auto it = std::begin(sss);
	while ( it != std::end(sss))
	{
		auto& pg = *it;
		if (pg.f_ == alpha::SZ)
		{
			pg.f_ = f;
			pg.t_ = t;
			++pg.cnt_;
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

void print(stkrset const& ss)
{
	std::cout << "direct = " << ss.direct_ << "\n";
	for (auto& s : ss.sss_)
	{
		std::cout << s.f_ << "<->" << s.t_ << " - " << s.cnt_ << "\n";
	}
}

template<typename IC, typename IA> void match_ciphertext(IC ctb, IC cte, IA base, stkrset& ss)
{
	clear(ss);
	// collect stecker possibles
	std::for_each(ctb, cte, [&base, &ss](auto const c) { set(ss, c, *base); ++base; });

}

