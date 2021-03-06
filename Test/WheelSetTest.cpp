// WheelSetTest.cpp 
//

#include <iostream>

#include "modalpha.h"
#include "wheelset.h"
#include "stecker.h"

int main()
{
	wheels3 w3( B, I, II, III );
	// set rings
	w3.Ring(alpha::B, alpha::B, alpha::B);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::A);
	w3.Report(std::cout);
	std::cout << "\n";

	// encode 'AAAAA'
	// expect 'EWTYX'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.PrintPosition(std::cout);
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
		w3.PrintPosition(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	// set rings
	w3.Ring(alpha::A, alpha::A, alpha::A);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::S);

	// encode 'AAAAA'
	// expect 'PBMUQ'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		w3.PrintPosition(std::cout);
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
		w3.PrintPosition(std::cout);
		std::cout << "  A  ";
		auto r = w3.Evaluate(alpha::A, std::cout);
		std::cout << "  " << r << "\n";
	}
	std::cout << "\n";

	return 0;
}