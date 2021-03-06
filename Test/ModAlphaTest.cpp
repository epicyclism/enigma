// ModTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <algorithm>

#include "modalpha.h"

void PrintRow(als_t const& als)
{
	for (auto const& a : als)
	{
		std::cout << a;
	}
	std::cout << '\n';
}
void PrintRowInt(als_t const& als)
{
	for (auto const& a : als)
	{
		std::cout << static_cast<int>(a.Val()) << " ";
	}
	std::cout << '\n';
}

int main()
{
	als_t al;
	modalpha m{ 0 };
	for (auto& a : al)
	{
		a = m;
		++m;
	}
	PrintRow(al);
	for (auto& a : al)
	{
		a += 13;
	}
	PrintRow(al);
	for (auto& a : al)
	{
		a += 13;
	}
	PrintRow(al);
	for (auto& a : al)
	{
		a -= 13;
	}
	PrintRow(al);
	for (auto& a : al)
	{
		a -= 13;
	}
	PrintRow(al);
	als_t bl ;
	std::transform(std::begin(al), std::end(al), std::begin(bl), [](auto const& l) { return l + 13; });
	PrintRow(bl);
	als_t cl;
	std::transform(std::begin(al), std::end(al), std::begin(bl), std::begin(cl), [](auto const& l, auto const& r) { return r - l; });
	PrintRow(cl);
	std::transform(std::begin(al), std::end(al), std::begin(bl), std::begin(cl), [](auto const& l, auto const& r) { return l - r; });
	PrintRow(cl);
	std::transform(std::begin(al), std::end(al), std::begin(bl), std::begin(cl), [](auto const& l, auto const& r) { return l + r; });
	PrintRow(cl);
	std::transform(std::begin(al), std::end(al), std::begin(bl), std::begin(cl), [](auto const& l, auto const& r) { return l < r ? 0 : 13 ; });
	PrintRow(cl);

	als_t dl{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 };
	PrintRow(dl);
}