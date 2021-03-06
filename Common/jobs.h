// 
// jobs.h
//
// make a job list from a set of wheels and reflectors
//

#pragma once

#include <exception>
#include <string_view>
#include <vector>
#include <algorithm>
#include <limits>

#include "wiring.h"
#include "rotor.h"

// string contains supported selection of reflectors?
// throws on error.
//
void check_reflectors(std::string_view ref)
{
	for (auto r : ref)
	{
		if (!valid_reflector_id(r))
			throw std::out_of_range("invalid reflector name");
	}
}

// string contains supported selection of wheels?
// throws on error.
//
void check_wheels(std::string_view whls)
{
	if (whls.size() < 3)
	{
		throw std::out_of_range("Insufficient wheels specified for a 3 rotor machine.");
	}
	for (auto w : whls)
	{
		if (!valid_rotor_id(w))
		{
			throw std::out_of_range("Invalid wheel.");
		}
	}
}

// string contains valid ring setting
// throws on error.
//
void check_ring(std::string_view ring)
{
	if (ring.size() < 3)
	{
		throw std::out_of_range("Invalid ring settings for a 3 rotor machine.");
	}
	for (auto r : ring)
	{
		if (!valid_ring_id(r))
		{
			throw std::out_of_range("Invalid ring.");
		}
	}
}

// duh
template<typename F> F factorial(F n)
{
	static_assert(std::is_integral_v<F>, "factorial only makes sense with an integral type, preferably unsigned!");
	F f = 1;
	while (n > 1)
	{
		f *= n;
		--n;
	}
	return f;
}

// fills out the 'mst_' member of a 'J'ob and returns a vector containing the relevant
// permutations given the supplied sets.
// assumes valid reflectors and wheels.
//
// eg. auto vj = MakeJobList("BC"sv, "12345"sv);
//

template<typename J, typename... ARGS> auto make_job_list(std::string_view reflectors, std::string_view wheels, int offset, int endoff, ARGS... args) -> std::vector<J>
{
	// reflectors
	std::string ref(reflectors);
	// wheels
	std::string whl(wheels);
	// sorted is a condition of permutations
	std::sort(std::begin(ref), std::end(ref)); // overkill....
	std::sort(std::begin(whl), std::end(whl));

	machine_settings_t mst;
	ZeroRing(mst);

	std::vector <J> vjb;
	auto skip = factorial(whl.size() - 3); // whl.size() is at least 3...
	auto cnt = skip;
	auto jobs = endoff - offset; // endoff tested for -1 later...
	do
	{
		do
		{
			--cnt; // take out the permutations of the wheels we're not taking to avoid duplication.
			if (cnt == 0)
			{
				if (offset == 0) // skip the first 'offset' of the possible orders, lets us either restart or share the effort.
				{
					mst.ref_ = ref[0];
					mst.w3_ = whl[0];
					mst.w2_ = whl[1];
					mst.w1_ = whl[2];
					vjb.emplace_back(mst, args...);
				}
				else
					--offset;
				cnt = skip;
			}
		} while (std::next_permutation(std::begin(whl), std::end(whl)));
	} while (std::next_permutation(std::begin(ref), std::end(ref)));

	if (endoff != -1)
		// erase the overspill
		vjb.erase(vjb.begin() + jobs, vjb.end());

	return vjb;
}

// fix usage of this, it's redundant to carry the ct here.
//
template<typename CI> struct job_wheels_ct
{
	machine_settings_t mst_;

	CI ctb_;
	CI cte_;

	job_wheels_ct(machine_settings_t const& mst, CI ctb, CI cte)
		: mst_(mst), ctb_(ctb), cte_(cte)
	{}
};

struct job_wheels
{
	machine_settings_t mst_;
	job_wheels(machine_settings_t const& mst)
		: mst_(mst)
	{}
};

