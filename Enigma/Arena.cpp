// Enigma.cpp : Defines the entry point for the application.
//

#include <array>
#include <algorithm>
#include <string_view>
#include <iostream>
#include "wheelset.h"

constexpr  char version[] = "v0.01";

using arena_t = std::array<std::array<modalpha, 1024>, alpha_max> ;

constexpr std::array<modalpha, alpha_max> alphabet{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };

void fill_arena(wheels3& w, arena_t& a, size_t off)
{
	// for each wheel state we have to go down all 26 arrays
	for (auto i = off; i < 1024; ++i)
	{
		w.Step();
		auto col = w.Evaluate(alphabet);
		auto itc = std::begin(col);
		for (auto ita = std::begin(a); ita != std::end(a); ++ita, ++itc)
		{
			(*ita)[i] = *itc;
		}
	}
}

void shuffle_down_arena(arena_t& a, size_t left)
{
	for (auto& r : a)
	{
		std::copy(std::begin(r) + left, std::end(r), std::begin(r));
	}
}

void Help()
{
	std::cerr << "arena " << version << " : experiments in brute force Enigma cracking.\n\n";
}

int main(int ac, char**av)
{
	Help();
	try
	{
		wheels3 w3{ V, II, I, B };
		arena_t a;
		fill_arena(w3, a, 0);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
