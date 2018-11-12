// Enigma.cpp : Defines the entry point for the application.
//

#include <array>
#include <string_view>
#include <iostream>
#include "machine.h"
#include "arena.h"

constexpr  char version[] = "v0.02";

void Help()
{
	std::cerr << "enigma " << version << " : processes stdin to stdout through an emulated Enigma machine.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./enigma B125 fvn XWB \"AH BO CG DP FL JQ KS MU TZ WY\"\n\n";
	std::cerr << "configures a 'machine' with reflector B, rotors 1, 2, 5, ring setting fvn, initial setting XWB\n";
	std::cerr << "and plug board as indicated. The plug settings can optionally be condensed and the quotes omitted.";
	std::cerr << "Then when given\n";
	std::cerr << "BYHSQTPUWDCMXYGQWMTZZMPNTUFSVGASNAXGLHFHAOVT\n";
	std::cerr << "the original message,\n";
	std::cerr << "DERFUEHRERISTTODXDERKAMPFGEHTWEITERXDOENITZX\n";
	std::cerr << "will be produced. Or vice versa.\n";
	std::cerr << "Generally \'X\' is used for punctuation so this well known message reads,\n";
	std::cerr << "DER FUEHRER IST TOD. DER KAMPF GEHT WEITER. DOENITZ.\n";
	std::cerr << "etc....\n";
	std::cerr << "Copyright (c) 2018, paul@epicyclism.com. Free to a good home.\n\n";
}

int main(int ac, char**av)
{
	if (ac < 5)
	{
		Help();
		return 0;
	}
	try
	{
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);
		Setting(m3, av[3]);
		Stecker(m3, av[4]);
		std::cout << "enigma " << version << " configured : ";
		m3.ReportSettings(std::cout);
		std::cout << "\nReady\n";
		std::vector<modalpha> vo;
		while (std::cin)
		{
			char c;
			std::cin >> c;
			if (valid_from_char(c))
			{
				auto x = m3.Transform(from_printable(c));
				std::cout << x;
				vo.push_back(x);
			}
		}
		std::cout << "\nFinished - ioc = " << index_of_coincidence(std::begin(vo), std::end(vo)) << "\n";
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
