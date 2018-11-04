// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "wheelset.h"
#include "machine.h"
#include "arena.h"

constexpr  char version[] = "v0.01";

// slide the ciphertext along the row and count the matches.
//
template<typename I, size_t W> void linear_search(I cb, I ce, std::array<modalpha, W> const& row, std::array<unsigned, W>& counts)
{
	auto itb = std::begin(row);
	auto ite = std::end(row) - std::distance(cb, ce);
	auto ito = std::begin(counts);
	while (itb != ite)
	{
		*ito = std::transform_reduce(cb, ce, itb, 0, std::plus<>(), [](auto l, auto r) { return l == r ? 1 : 0; });
		++ito;
		++itb;
	}
}

template<typename I, typename A, typename R> void arena_search(I cb, I ce, A const& a, R& r)
{
	auto itr  = std::begin(r);

	std::for_each(std::begin(a.arena_), std::end(a.arena_), [&itr, &cb, &ce](auto r)
	{
		linear_search(cb, ce, r, *itr);
		++itr;
	});
}

template<typename A, typename R> void report_results2(A const& a, R const& r, size_t s)
{
	auto itp = std::begin(a.pos_);
	for (auto& rr : r)
	{
		auto itm = std::max_element(rr.begin(), rr.end() - s);
		std::cout << *(itp + std::distance(rr.begin(), itm)) << " - " << *itm << "\n";
	}
}

template<typename A, typename R> void report_results3(A const& a, R const& r, size_t s)
{
	auto itp = std::begin(a.pos_);
	std::for_each(r[4].begin(), r[4].end() - s, [](auto i) { std::cout << i << " "; }); // 'E'
	std::cout << "\n";
	std::for_each(r[23].begin(), r[23].end() - s, [](auto i) { std::cout << i << " "; }); // 'X'
	std::cout << "\n";
}

template<typename A, typename R> void report_results(A const& a, R const& r, size_t s)
{
	auto itp = std::begin(a.pos_);
	for (auto i = 0; i < A::Width - s; ++i)
	{
		int sum = 0;
		for (auto ita = std::begin(r); ita != std::end(r); ++ita)
		{
			sum += (*ita)[i];
		}
		std::cout << *itp << " - " << sum << "\n";
		++itp;
	}
}

template<typename R> void print_results(R const& r, size_t s)
{
	for (auto ita = std::begin(r); ita != std::end(r); ++ita)
	{
		std::for_each(std::begin(*ita), std::end(*ita) - s, [](auto x)
		{
			std::cout << x << " ";
		});
		std::cout << "\n";
	}
}

void Help()
{
	std::cerr << "arena " << version << " : experiments in brute force Enigma cracking.\n\n";
}

template<typename A> void ArenaTestPrint(A const& a)
{
	std::cout << "A ->\n";
	arena_print_r(a, alpha::A, 32, std::cout);
	std::cout << "Z ->\n";
	arena_print_r(a, alpha::Z, 32, std::cout);
}

arena_t a;
arena_t::results_t results;

int main(int ac, char**av)
{
	Help();
	auto ciphertext = make_alpha_array("EXTYYFJYICJYGKRWTZBCMGORXJQZXMQLQOMIFYTPWCAI");

	try
	{
		wheels3 w3{ I, II, V, B };
		w3.Ring( alpha::A, alpha::A, alpha::A );
		w3.Setting( alpha::A, alpha::A, alpha::A );
		fill_arena(w3, a, 0);
//		ArenaTestPrint(a);
		arena_search(std::begin(ciphertext), std::end(ciphertext), a, results);
//		print_results(results, ciphertext.size());
		report_results2(a, results, ciphertext.size());
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
