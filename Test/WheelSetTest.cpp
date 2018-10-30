// WheelSetTest.cpp 
//

#include <iostream>

#include "mod26.h"
#include "wheelset.h"

int main()
{
	wheels3 w3{ I, II, III, B };
	// set rings
	w3.Ring(alpha::B, alpha::B, alpha::B);
	// set initial
	w3.Setting(alpha::A, alpha::A, alpha::A);
	// encode 'AAAAA'
	for (int n = 0; n < 5; ++n)
	{
		w3.Step();
		std::cout << to_printable(w3.Evaluate(alpha::A);
	}
	std::cout << "\n";
}