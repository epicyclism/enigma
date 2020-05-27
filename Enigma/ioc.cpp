#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

#include "modalpha.h"
#include "ioc.h"
#include "utility.h"

constexpr  char version[] = "v0.01";

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