// WheelSetTest.cpp 
//

#include <iostream>

#include "mod26.h"
#include "wheelset.h"
#include "stecker.h"

int main()
{
	wheels3 w3( I, II, III, B );
	// set rings
	w3.Ring(alpha::B, alpha::B, alpha::B);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::A);
	// encode 'AAAAA'
	// expect 'EWTYX'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.Position(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	// set rings
	w3.Ring(alpha::A, alpha::A, alpha::A);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::A);

	// encode 'AAAAA'
	// expect 'BDZGO'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.Position(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	// set rings
	w3.Ring(alpha::A, alpha::A, alpha::A);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::T);

	// encode 'AAAAA'
	// expect 'PBMUQ'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.Position(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	// set rings
	w3.Ring(alpha::A, alpha::A, alpha::A);
	// set initial
	w3.Setting(alpha::A, alpha::D, alpha::T);

	// encode 'AAAAA'
	// expect 'EEQIB'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.Position(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	// decode a real message! Stecker test.
	{
		wheels3 w3(IV, III, II, B);
		w3.Ring(alpha::R, alpha::I, alpha::T);
		w3.Setting(alpha::V, alpha::O, alpha::R);
		stecker brd;
		brd.Set(alpha::A, alpha::H);
		brd.Set(alpha::B, alpha::O);
		brd.Set(alpha::C, alpha::G);
		brd.Set(alpha::D, alpha::P);
		brd.Set(alpha::F, alpha::L);
		brd.Set(alpha::J, alpha::Q);
		brd.Set(alpha::K, alpha::S);
		brd.Set(alpha::M, alpha::U);
		brd.Set(alpha::T, alpha::Z);
		brd.Set(alpha::W, alpha::Y);
		std::cout << "Stecker - ";
		brd.Report(std::cout);
		std::cout << "\n";
		constexpr char ciphertext[]{ 'F', 'V', 'K', 'F', 'C', 'D', 'W', 'R', 'I', 'I', 'C', 'Y', 'F', 'H', 'V', 'S', 'K', 'Q', 'O', 'W', 'Q', 'T', 'T', 'H' };
		for (auto c : ciphertext)
		{
			w3.Step();
			w3.Position(std::cout);
			std::cout << " " << c << " ";
			auto in = brd.Eval(from_printable(c), std::cout);
			auto r = w3.Evaluate(in, std::cout);
			std::cout << brd.Eval(r, std::cout) << "\n" ;
		}
		std::cout << "\n";
	}
}