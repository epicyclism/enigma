#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
//#include <numeric>
#include <vector>

constexpr  char version[] = "v0.01";

void Help()
{
	std::cerr << "textgen " << version << " : creates random text containing a specified proportion of 'E'.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./textgen 128 14\n\n";
	std::cerr << "emits 128 characters with a 14% 'E' content.\n";
	std::cerr << "Message is all caps, A-Z, since that suits the domain.\n";
	std::cerr << "Copyright (c) 2020, paul@epicyclism.com. Free to a good home.\n\n";
}

int main(int ac, char** av)
{
	if (ac < 3)
	{
		std::cerr << "Too few arguments - " << ac << "\n\n";
		Help();
		return -1;
	}
	int cnt  = ::atoi(av[1]);
	int prop = ::atoi(av[2]);
	if (cnt > 256)
		std::cout << "pseudo message seems a little long.\n";
	if (prop > 100)
	{
		std::cout << "It would be easier to simply type a line of 'E'.\n";
		prop = 100;
	}
	try
	{
		int ees = cnt * prop / 100;
		std::cerr << "Creating message with " << cnt << " characters including " << ees << " 'E's\n\n";
		std::vector<char> vo(cnt);
		// load the E's
		std::fill_n(vo.begin(), ees, 'E');
		// fill the rest
		char c = 'A';
		std::generate(vo.begin() + ees, vo.end(), [&c]() { auto cr = c; ++c; if (c == 'E') ++c; if (c == 'Z' + 1) c = 'A'; return cr; });
		// shuffle
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(vo.begin(), vo.end(), g);
		// report
		std::for_each(vo.begin(), vo.end(), [](auto ch) { std::cout << ch; });
		std::cout << '\n';
	}
	catch (std::exception& ex)
	{
		std::cerr << "Some sort of exception caught.. >" << ex.what() << "<\n\n";
		return -1;
	}
}
