// Enigma.cpp : Defines the entry point for the application.
//

#include <array>
#include <string_view>
#include <iostream>
#include <numeric>
#include "machine.h"
#include "match.h"
#include "ioc.h"
#include "bigram.h"

constexpr  char version[] = "v0.04";

template<typename I> unsigned calc_ees(I ctb, I cte, machine_settings_t mst)
{
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> ees(std::distance(ctb, cte), alpha::E);
	std::vector<modalpha> enc_ees = m3.Transform(std::begin(ees), std::end(ees));

	return match_ciphertext(ctb, cte, std::begin(enc_ees), m3.Eval(alpha::E));
}

void Help()
{
	std::cerr << "enigma " << version << " : processes stdin to stdout through an emulated Enigma machine.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./enigma B125 fvn XWB \"AH BO CG DP FL JQ KS MU TZ WY\" [-stats]\n\n";
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
	std::cerr << "The option -stats causes the production of some interesting statistics.\n";
	std::cerr << "Copyright (c) 2018, paul@epicyclism.com. Free to a good home.\n\n";
}

int main(int ac, char**av)
{
	if (ac < 5)
	{
		Help();
		return 0;
	}
	bool stats = false;
	if (ac > 5)
	{
		if (av[5][0] == '-' && av[5][1] == 's')
			stats = true;
		else
		{
			std::cerr << "Bad trailing argument.\n\n";
			Help();
			return 0;
		}
	}

	try
	{
		machine3 m3 = MakeMachine3(av[1]);
		Ring(m3, av[2]);
		Setting(m3, av[3]);
		Stecker(m3, av[4]);
		std::cout << "enigma " << version << " configured : ";
		m3.ReportSettings(std::cout);
		auto mst = m3.machine_settings();
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
		std::cout << "\nFinished\n";
		if (stats)
		{
			std::cout << "Statistics!\n";
			std::cout << "message length = " << vo.size() << "\n";
			std::cout << "ees    = " << calc_ees(std::begin(vo), std::end(vo), mst) << "\n";
			std::cout << "ioc    = " << index_of_coincidence(std::begin(vo), std::end(vo)) << "\n";
			std::cout << "bigram = " << bigram_score(std::begin(vo), std::end(vo)) / (vo.size() * 2 - 2) << "\n";
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
