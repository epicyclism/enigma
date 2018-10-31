// MachineTest.cpp 
//

#include <iostream>

#include "machine.h"

int main()
{
//	machine3 m3 = MakeMachine3("B234");
	machine3 m3(B, II, III, IV);
	m3.Ring("rit");
	m3.Setting("VOR");
	m3.Stecker('A', 'H');
	m3.Stecker('B', 'O');
	m3.Stecker('C', 'G');
	m3.Stecker('D', 'P');
	m3.Stecker('F', 'L');
	m3.Stecker('J', 'Q');
	m3.Stecker('K', 'S');
	m3.Stecker('M', 'U');
	m3.Stecker('T', 'Z');
	m3.Stecker('W', 'Y');

	constexpr modalpha ciphertext[]{ alpha::F, alpha::V, alpha::K, alpha::F, alpha::C,
										alpha::D, alpha::W, alpha::R, alpha::I, alpha::I, 
										alpha::C, alpha::Y, alpha::F, alpha::H, alpha::V, 
										alpha::S, alpha::K, alpha::Q, alpha::O, alpha::W, 
										alpha::Q, alpha::T, alpha::T, alpha::H };
	auto out = m3.Transform(std::begin(ciphertext), std::end(ciphertext), std::cout);
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";

	return 0;
}