// ModTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <algorithm>

#include "modalpha.h"
#include "wiring.h"

// wheel wiring
//
constexpr wiring     I = FromDescWheel("EKMFLGDQVZNTOWYHXUSPAIBRCJ", '1');
constexpr wiring    II = FromDescWheel("AJDKSIRUXBLHWTMCQGZNPYFVOE", '2');
constexpr wiring   III = FromDescWheel("BDFHJLCPRTXVZNYEIWGAKMUSQO", '3');
constexpr wiring    IV = FromDescWheel("ESOVPZJAYQUIRHXLNFTGKDCMWB", '4');
constexpr wiring     V = FromDescWheel("VZBRGITYUPSDNHLXAWMJQOFECK", '5');
constexpr wiring    VI = FromDescWheel("JPGVOUMFYQBENHZRDKASXLICTW", '6');
constexpr wiring   VII = FromDescWheel("NZJHGRCXMYSWBOUFAIVLPEKQDT", '7');
constexpr wiring  VIII = FromDescWheel("FKQHTLXOCBJSPDZRAMEWNIUYGV", '8');
constexpr wiring  beta = FromDescWheel("LEYJVCNIXWPBQMDRTAKZGFUHOS", 'b');
constexpr wiring gamma = FromDescWheel("FSOKANUERHMBTIYCWLQPZXVGJD", 'g');
constexpr wiring  null = FromDescWheel("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 'n');


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
	PrintRow(I.rl_);
	PrintRow(I.lr_);
	PrintRowInt(I.rl_);
	PrintRowInt(I.lr_);

	PrintRow(B.rl_);
	PrintRow(B.lr_);
	PrintRowInt(B.rl_);
	PrintRowInt(B.lr_);
}