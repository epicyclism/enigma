// RotorTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <algorithm>

#include "modalpha.h"
#include "rotor.h"

// just proves that all the rotors are defined as expected.
//

void PrintRow(als_t const& als)
{
	for (auto const& a : als)
	{
		std::cout << a;
	}
	std::cout << '\n';
}

void PrintRotor(const char * nm, rotor const& r)
{
	std::cout << nm << "\n";
	PrintRow(r.rl_);
	PrintRow(r.lr_);
	std::cout << r.to_ << ", " << (r.dual_ ? "dual":"single") << "\n";
}

int main()
{
	PrintRotor("I", I);
	PrintRotor("II", II);
	PrintRotor("III", III);
	PrintRotor("IV", IV);
	PrintRotor("V", V);
	PrintRotor("VI", VI);
	PrintRotor("VII", VII);
	PrintRotor("VIII", VIII);
	PrintRotor("beta", beta);
	PrintRotor("gamma", gamma);
}