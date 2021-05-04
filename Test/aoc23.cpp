#include <iostream>
#include <iterator>
#include <array>
#include <vector>
#include <execution>
#include <algorithm>
#include <numeric>

using cups_t = std::array<char, 9>;
using cups_t2 = std::array<int, 100>;

cups_t test_input{ 3, 8, 9, 1, 2, 5, 4, 6, 7 };
cups_t  input{ 7, 9, 2, 8, 4, 5, 1, 3, 6 };
//cups_t2 input2{ 3, 8, 9, 1, 2, 5, 4, 6, 7 };
cups_t2 input2{ 7, 9, 2, 8, 4, 5, 1, 3, 6 };

template<typename CT> void print(CT const& ct)
{
	std::copy(ct.begin(), ct.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';
}

template<typename CT>
void work(CT& ct, int moves)
{
	auto nextd = [&](int d) -> int { return d - 1 ? d - 1 : int(ct.size()); };
	for (int n = 0; n < moves; ++n)
	{
		// find dest
		auto d = nextd(ct.front());
		auto di = ct.end();
		while ((di = std::find(std::execution::par, ct.begin() + 4, ct.end(), d)) == ct.end())
			d = nextd(d);
		// move the three
		std::rotate(std::execution::par, ct.begin() + 1, ct.begin() + 4, di + 1);
		// and place
		std::rotate(std::execution::par, ct.begin(), ct.begin() + 1, ct.end());
		print(ct);
	}
	std::cout << '\n';
	auto d1 = std::find(ct.begin(), ct.end(), 1);
	std::rotate(std::execution::par, ct.begin(), d1 + 1, ct.end());
}

template<typename CT>
void work(CT& ct)
{
	auto nextd = [&](int d) -> int { return d - 1 ? d - 1 : int(ct.size()); };
	// find dest
	auto d = nextd(ct.front());
	auto di = ct.end();
	while ((di = std::find(std::execution::par, ct.begin() + 4, ct.end(), d)) == ct.end())
		d = nextd(d);
	// move the three
	std::rotate(std::execution::par, ct.begin() + 1, ct.begin() + 4, di + 1);
	// and place
	std::rotate(std::execution::par, ct.begin(), ct.begin() + 1, ct.end());
}

int main()
{
#if 0
	work(test_input, 100);
	print(test_input);
	work(input, 100);
	print(input);
	std::iota(input2.begin() + 9, input2.end(), 10);
	print(input2);	
	work(input2, 100);
	auto ai = std::find(input2.begin() + 9, input2.end(), 1);
	++ai;
	if (ai == input2.end())
		ai = input2.begin();
	int64_t n1 = *ai;
	++ai;
	if (ai == input2.end())
		ai = input2.begin();
	int64_t n2 = *ai;
	std::cout << n1 << " * " << n2 << " = " << n1 * n2 << '\n';
#else
	std::iota(input2.begin() + 9, input2.end(), 10);
	std::vector<std::array<int, 16>> history;
	cups_t2 wk {input2};
	int n = 0;
	while(n < 10000000)
	{
		work(wk);
//		if(auto it = std::find_if(history.begin(), history.end(), [&](auto& l){ return std::equal(l.begin(), l.end(), wk.begin());}); it != history.end())
//		{
//			std::cout << "Loop at " << std::distance(history.begin(), it) << " and " << n << '\n';
//		}
		std::array<int, 16> h;
		std::copy(wk.begin(), wk.begin() + 16, h.begin());
		history.emplace_back(h);
		++n;
	}
#endif
}
