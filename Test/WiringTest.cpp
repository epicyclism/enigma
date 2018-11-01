// ModTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <iostream>
#include <algorithm>

#include "modalpha.h"
#include "wiring.h"

// wheel wiring
//
constexpr wiring     I = FromDescWheel("EKMFLGDQVZNTOWYHXUSPAIBRCJ");
constexpr wiring    II = FromDescWheel("AJDKSIRUXBLHWTMCQGZNPYFVOE");
constexpr wiring   III = FromDescWheel("BDFHJLCPRTXVZNYEIWGAKMUSQO");
constexpr wiring    IV = FromDescWheel("ESOVPZJAYQUIRHXLNFTGKDCMWB");
constexpr wiring     V = FromDescWheel("VZBRGITYUPSDNHLXAWMJQOFECK");
constexpr wiring    VI = FromDescWheel("JPGVOUMFYQBENHZRDKASXLICTW");
constexpr wiring   VII = FromDescWheel("NZJHGRCXMYSWBOUFAIVLPEKQDT");
constexpr wiring  VIII = FromDescWheel("FKQHTLXOCBJSPDZRAMEWNIUYGV");
constexpr wiring  beta = FromDescWheel("LEYJVCNIXWPBQMDRTAKZGFUHOS");
constexpr wiring gamma = FromDescWheel("FSOKANUERHMBTIYCWLQPZXVGJD");
constexpr wiring  null = FromDescWheel("ABCDEFGHIJKLMNOPQRSTUVWXYZ");


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