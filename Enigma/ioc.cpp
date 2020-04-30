#include <iostream>
#include <iomanip>
#include <utility>

#include "modalpha.h"
#include "ioc.h"

constexpr  char version[] = "v0.01";

std::vector<modalpha> read_ciphertext()
{
	std::vector<modalpha> rv;
	while (1)
	{
		char c;
		std::cin >> c;
		if (!std::cin)
			break;
		if (valid_from_char(c))
		{
			rv.push_back(from_printable(c));
		}
	}
	return rv;
}

int main(int ac, char** av)
{
	if (ac > 1)
	{
		std::cerr << "ioc " << version << " - reads a text from stdin, computes and reports the character frequencies and index of coincidence\n";
		return -1;
	}

	auto ct = read_ciphertext();
	auto tab = gen_freq_tab(std::begin(ct), std::end(ct));

	auto N = ct.size();

	// report
	modalpha c = alpha::A;
	for (auto cnt : tab)
	{
		std::cout << c << std::setw(6) << cnt << '\n';
		++c;
	}
	std::cout << "Text has " << N << " characters.\n";
	std::cout << "ioc is " << ioc_wkr(tab, N) << '\n';
}