// MachineTest.cpp 
//

#include <iostream>

#include "machine.h"

void Test1()
{
	machine3 m3 = MakeMachine3("B432");
	Ring(m3, "rit");
	Setting(m3, "VOR");
	Stecker(m3, 'A', 'H');
	Stecker(m3, 'B', 'O');
	Stecker(m3, 'C', 'G');
	Stecker(m3, 'D', 'P');
	Stecker(m3, 'F', 'L');
	Stecker(m3, 'J', 'Q');
	Stecker(m3, 'K', 'S');
	Stecker(m3, 'M', 'U');
	Stecker(m3, 'T', 'Z');
	Stecker(m3, 'W', 'Y');

	constexpr modalpha ciphertext[]{ alpha::F, alpha::V, alpha::K, alpha::F, alpha::C,
										alpha::D, alpha::W, alpha::R, alpha::I, alpha::I, 
										alpha::C, alpha::Y, alpha::F, alpha::H, alpha::V, 
										alpha::S, alpha::K, alpha::Q, alpha::O, alpha::W, 
										alpha::Q, alpha::T, alpha::T, alpha::H };
	auto out = m3.Transform(std::begin(ciphertext), std::end(ciphertext));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

void Test2()
{
	machine3 m3 = MakeMachine3("B125");
	Ring(m3, "fvn");
	Setting(m3, "XWB");
	Stecker(m3, 'P', 'O');
	Stecker(m3, 'M', 'L');
	Stecker(m3, 'I', 'U');
	Stecker(m3, 'K', 'J');
	Stecker(m3, 'N', 'H');
	Stecker(m3, 'Y', 'T');
	Stecker(m3, 'G', 'B');
	Stecker(m3, 'V', 'F');
	Stecker(m3, 'R', 'E');
	Stecker(m3, 'D', 'C');

	constexpr modalpha ciphertext[]{ alpha::Q, alpha::B, alpha::L, alpha::T, alpha::W,
		alpha::L, alpha::D, alpha::A, alpha::H, alpha::H,
		alpha::Y, alpha::E, alpha::O, alpha::E, alpha::F,
		alpha::P, alpha::T, alpha::W, alpha::Y, alpha::B,
		alpha::L, alpha::E, alpha::N, alpha::D, alpha::P,
		alpha::M, alpha::K, alpha::O, alpha::X, alpha::L,
		alpha::D, alpha::F, alpha::A, alpha::M, alpha::U,
		alpha::D, alpha::W, alpha::I, alpha::J, alpha::D,
		alpha::X, alpha::R, alpha::J, alpha::Z };

	auto out = m3.Transform(std::begin(ciphertext), std::end(ciphertext));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

int main()
{
	Test1();
	Test2();

	return 0;
}