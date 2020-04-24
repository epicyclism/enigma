﻿// Enigma.cpp : command line 3 rotor Enigma emulator
//

#include <array>
#include <string_view>
#include <iostream>
#include <numeric>
#include "machine.h"
#include "match.h"
#include "ioc.h"
#include "bigram.h"
#include "trigram.h"

constexpr  char version[] = "v0.08";

template<typename I> unsigned count_ees(I ptb, I pte)
{
	return static_cast<unsigned>(std::count_if(ptb, pte, [](auto c) {return c == alpha::E; }));
}

void Help()
{
	std::cerr << "enigma " << version << " : processes stdin to stdout through an emulated Enigma machine.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./enigma B125 fvn XWB \"AH BO CG DP FL JQ KS MU TZ WY\" [-stats][-debug]\n\n";
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
	std::cerr << "EOF to finish (and print optional stats), Ctrl-Z on Windows, Ctrl-D on Unix\n";
	std::cerr << "The option -s(tats) causes the production of some interesting statistics.\n";
	std::cerr << "And the option -d(ebug) prints the path through the plugs, wheels and reflector for each letter.\n";
	std::cerr << "Copyright (c) 2018-2019, paul@epicyclism.com. Free to a good home.\n\n";
}

int main(int ac, char**av)
{
	if (ac < 5)
	{
		std::cerr << "Too few arguments - " << ac << "\n\n";

		Help();
		return 0;
	}
	bool stats = false;
	bool debug = false;
	int arg = 5;
	while ( arg < ac)
	{
		if (av[arg][0] == '-' && av[arg][1] == 's')
			stats = true;
		else
		if (av[arg][0] == '-' && av[arg][1] == 'd')
			debug = true;
		else
		{
			std::cerr << "Bad trailing argument '" << av[arg] << "'.\n\n";
			Help();
			return 0;
		}
		++arg;
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
		std::cout << "\n";
		if (debug)
			std::cout << "'debug' enabled\n";
		if (stats)
			std::cout << "'stats' enabled\n";
		std::cout << "\nReady\n\n";
		std::vector<modalpha> vi;
		std::vector<modalpha> vo;
		while (std::cin)
		{
			char c;
			std::cin >> c;
			if (valid_from_char(c))
			{
				auto cc = from_printable(c);
				vi.push_back(cc);
				if (debug)
				{
					auto x = m3.Transform(cc, std::cout);
					std::cout << x << "\n";
					vo.push_back(x);
				}
				else
				{
					auto x = m3.Transform(cc);
					std::cout << x;
					vo.push_back(x);
				}
			}
		}
		std::cout << "\nFinished\n";
		if (stats)
		{
			std::cout << "Statistics!\n";
			std::cout << "message length = " << vo.size() << "\n";
			auto cnt_e = count_ees(std::begin(vo), std::end(vo));
			std::cout << "e count = " << cnt_e << " (" << double(cnt_e) * 100 / vo.size() << "%)\n";
			std::cout << "ioc     = " << index_of_coincidence(std::begin(vo), std::end(vo)) << "\n";
			std::cout << "bigram gen   = " << bigram_score_gen(std::begin(vo), std::end(vo)) << "\n";
			std::cout << "bigram 1941  = " << bigram_score_1941(std::begin(vo), std::end(vo)) << "\n";
			std::cout << "trigram gen  = " << trigram_score_gen(std::begin(vo), std::end(vo)) << "\n";
			std::cout << "trigram 1941 = " << trigram_score_1941(std::begin(vo), std::end(vo)) << "\n";
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1; 
	}

	return 0;
}
