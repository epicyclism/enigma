#include "modalpha.h"

// unigram commonness from https://github.com/VE3NEA/enigma-cuda/blob/master/data/00unigr.gen
//                     and https://github.com/VE3NEA/enigma-cuda/blob/master/data/00unigr.cur
//

using unigram_table = std::array<unsigned, alpha_max>;

//                                     A      B      C      D      E      F      G      H      I      J      K      L      M      N      O      P      Q      R      S      T      U      V      W      X      Y      Z
constexpr unigram_table ug_cur = { 25831, 17970, 21355, 19560, 36379, 20830, 18718, 23242, 26537,  9390, 11600, 22052, 16677, 31982, 23067,  5628,     0, 28339, 28562, 26690, 26861, 15294, 16286, 14693, 16268, 14414 };
constexpr unigram_table ug_gen = { 35274, 25445,  7105, 34880, 47451, 25278, 30750, 23769, 37464,  5917, 23078, 30344, 26085, 40390, 27448, 17159, 28281, 38468, 37071, 35864, 33303, 17711, 20415, 45598,     0, 21224 };

// score with the given unigram table.
//
template<typename I> unsigned unigram_score(I b, I e, unigram_table const& bt)
{
	if (b == e)
		return 0;
	auto len = std::distance(b, e);
	auto score = std::accumulate(b, e, 0U, [&](auto sm, auto c) { return sm + bt[c.Val()]; });
	// normalise to message length here, right down at the source.
	return score / (static_cast<unsigned>(len) - 1);
}

// conveniences.
//
template<typename I> unsigned unigram_score_cur(I b, I e)
{
	return unigram_score(b, e, ug_cur);
}

struct unigram_score_cur_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return unigram_score(b, e, ug_cur);
	}
};

template<typename I> unsigned unigram_score_gen(I b, I e)
{
	return unigram_score(b, e, ug_gen);
}

struct unigram_score_gen_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return unigram_score(b, e, ug_gen);
	}
};

// default
//
template<typename I> unsigned unigram_score(I b, I e)
{
	return unigram_score(b, e, ug_gen);
}

struct unigram_score_op
{
	template<typename I> unsigned operator()(I b, I e)
	{
		return unigram_score(b, e, ug_gen);
	}
};

