#include <fstream>
#include <iostream>
#include <iterator>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

constexpr char version[]{ "0.01" };

void help()
{
	std::cerr << "make_bgtg : take text files containing sentences (in German), process them for language,\n";
	std::cerr << "            and produce bigram / trigam tables for importation into the system.\n";
	std::cerr << "            Files are presumed to be formatted line by line something like,\n";
	std::cerr << "            0000000_0000000_173644-173901 Staatsgebiet die Bundesländer\n";
	std::cerr << "            So a code, a space, space separated words.\n\n";
	std::cerr << "Usage is\n";
	std::cerr << "make_bgtg <file1> [file2...filen]\n";
	std::cerr << "Results to stdout.\n\n";
}

constexpr int stride_ = 32;
constexpr int alpha_max = 26;

// make these variadic templates for a laugh.
//
struct bg_table_t
{
	std::array<unsigned, stride_* alpha_max> tab_;

	[[nodiscard]] const unsigned get_offset(int a, int b) const noexcept
	{
		return a * stride_ + b;
	}
	// note unprotected from stupidity!
	//
	[[nodiscard]] const unsigned wt(int a, int b) const noexcept
	{
		unsigned off = a * stride_ + b;
		return tab_[get_offset(a, b)];
	}
	void inc(char a, char b)
	{
		++tab_[get_offset(a - 'A', b - 'A')];
	}
	bg_table_t()
	{
		tab_.fill(0);
	}
};

struct tg_table_t
{
	std::array<unsigned, stride_* stride_* alpha_max> tab_;

	[[nodiscard]] const unsigned get_offset(int a, int b, int c) const
	{
		return a * stride_ * stride_ + b * stride_ + c;
	}
	[[nodiscard]] const unsigned wt(int a, int b, int c) const noexcept
	{
		return tab_[get_offset(a, b, c)];
	}
	void inc(char a, char b, char c)
	{
		++tab_[get_offset(a - 'A', b - 'A', c - 'A')];
	}
	tg_table_t()
	{
		tab_.fill(0);
	}
};

bg_table_t bg_table;
tg_table_t tg_table;

// process to uppercase, return false if invalid
template <typename S> [[nodiscard]] bool valid_estring(S const& str) noexcept
{
	bool b = true;
	for (auto& c : str)
	{
		if (c < 'A' || c > 'Z')
			b = false;
	}
	return b;
}

// ugh
std::vector<char> process_string(std::string const& str)
{
	std::vector<char> vo;
	vo.reserve(str.size());
	int state = 0;
	for (auto c : str)
	{
		if (c >= 'a' && c <= 'z')
			vo.push_back(c - 'a' + 'A');
		else
			if (c >= 'A' && c <= 'Z')
				vo.push_back(c);
			else
			{
				switch (static_cast<unsigned char>(c))
				{
				case ' ':
					vo.push_back('X');
					break;
				case '.':
					vo.push_back('X');
					vo.push_back('X');
					break;
				case ',':
					vo.push_back('Y');
					break;
				case '?':
					vo.push_back('U');
					vo.push_back('D');
					break;
				case '*':
				case '\'':
				case '"':
				case '$':
				case ':':
				case '\t':
					break;
				case '1':
					vo.push_back('E');
					vo.push_back('I');
					vo.push_back('N');
					vo.push_back('S');
					break;
				case '2':
					vo.push_back('Z');
					vo.push_back('W');
					vo.push_back('E');
					vo.push_back('I');
					break;
				case '3':
					vo.push_back('D');
					vo.push_back('R');
					vo.push_back('E');
					vo.push_back('I');
					break;
				case '4':
					vo.push_back('V');
					vo.push_back('I');
					vo.push_back('E');
					vo.push_back('R');
					break;
				case '5':
					vo.push_back('F');
					vo.push_back('U');
					vo.push_back('E');
					vo.push_back('N');
					vo.push_back('F');
					break;
				case '6':
					vo.push_back('S');
					vo.push_back('E');
					vo.push_back('Q');
					vo.push_back('S');
					break;
				case '7':
					vo.push_back('S');
					vo.push_back('I');
					vo.push_back('E');
					vo.push_back('B');
					vo.push_back('E');
					vo.push_back('N');
					break;
				case '8':
					vo.push_back('A');
					vo.push_back('Q');
					vo.push_back('T');
					break;
				case '9':
					vo.push_back('N');
					vo.push_back('E');
					vo.push_back('U');
					vo.push_back('N');
					break;
				case '0':
					vo.push_back('N');
					vo.push_back('U');
					vo.push_back('L');
					vo.push_back('L');
					break;
				case '-':
				case '/':
					vo.push_back('S');
					vo.push_back('T');
					vo.push_back('R');
					vo.push_back('I');
					vo.push_back('Q');
					break;
					// deal with the utf-8
				case 0xc2:
					state = 1;
					break;
				case 0xc3:
					state = 1;
					break;
				case 0xc5:
					state = 1;
					break;
				case 0xc8:
					state = 1;
					break;
				case 0xcf:
					state = 1;
					break;
				case 0xe2:
					state = 2;
					break;
				case 0x84:
					if (state == 1)
					{
						vo.push_back('A');
						vo.push_back('E');
					}
					state = 1;
					break;
				case 0x93:
					if (state == 1)
					{
						vo.push_back('O');
						vo.push_back('E');
					}
					state = 1;
					break;
				case 0x96:
					if (state == 1)
					{
						vo.push_back('O');
						vo.push_back('E');
					}
					state = 1;
					break;
				case 0x9c:
					if (state == 1)
					{
						vo.push_back('U');
						vo.push_back('E');
					}
					state = 1;
					break;
				case 0x9f:
					if (state == 1)
					{
						vo.push_back('S');
						vo.push_back('S');
					}
					state = 0;
					break;
				case 0xa4:
					if (state == 1)
					{
						vo.push_back('A');
						vo.push_back('E');
					}
					state = 0;
					break;
				case 0xb6:
					if (state == 1)
					{
						vo.push_back('O');
						vo.push_back('E');
					}
					state = 0;
					break;
				case 0xbc:
					if (state == 1)
					{
						vo.push_back('U');
						vo.push_back('E');
					}
					state = 0;
					break;
				default:
					if (state == 0)
						vo.push_back(c);
					else
						--state;
					break;
				}
			}
	}

	return vo;
}

void print_vec(std::vector<char> const& v, std::ostream& ostr)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<char>(ostr));
}

void process_file(char const* fn, bg_table_t& bgt, tg_table_t& tgt) noexcept
{
	std::ifstream inf(fn);
	if (!inf)
	{
		std::cerr << "File <" << fn << "> failed to open, skipping.\n";
		return;
	}
	std::string ln;
	std::string lnp;
	std::vector<char> vo;
	while (std::getline(inf, ln))
	{
		// trim the front
		auto sp = ln.find(' ');
		lnp = ln.substr(sp);
		vo = process_string(lnp);
		if (valid_estring(vo))
		{
			// tabulate
			if (vo.size() > 1)
			{
				auto a = vo.begin();
				auto b = a + 1;
				while (b != vo.end())
				{
					bgt.inc(*a, *b);
					++a; ++b;
				}
			}
			if (vo.size() > 2)
			{
				auto a = vo.begin();
				auto b = a + 1;
				auto c = b + 1;
				while (c != vo.end())
				{
					tgt.inc(*a, *b, *c);
					++a; ++b; ++c;
				}
			}
		}
		else
		{
			std::cerr << "Input <" << ln << "> processed to <";
			print_vec(vo, std::cerr);
			std::cerr << "> and is not valid.\n";
		}
	}
}

void write_table(bg_table_t const& tab)
{
	std::cout << "constexpr struct bg_def bg_def_xxx[]\n{\n";
	for (int a = 0; a < alpha_max; ++a)
		for (int b = 0; b < alpha_max; ++b)
		{
			auto wt = tab.wt(a, b);
			if (wt)
				std::cout << "{ '" << char(a + 'A') << "', '" << char(b + 'A') << "', " << wt << " },\n";
		}
	std::cout << "};\n";
}

void write_table(tg_table_t const& tab)
{
	std::cout << "constexpr struct tg_def tg_def_xxx[]\n{\n";
	for (int a = 0; a < alpha_max; ++a)
		for (int b = 0; b < alpha_max; ++b)
			for (int c = 0; c < alpha_max; ++c)
			{
				auto wt = tab.wt(a, b, c);
				if (wt)
					std::cout << "{ '" << char(a + 'A') << "', '" << char(b + 'A') << "', '" << char(c + 'A') << "', " << wt << " },\n";
			}
	std::cout << "};\n";
}

int main(int ac, char** av)
{
	std::ios::sync_with_stdio(false);
	std::cerr << "make_bgtg v" << version << "\n";
	if (ac == 1 || av[1][0] == '-'
#if defined(WIN32)
		|| av[1][0] == '/'
#endif
		)
	{
		help();
		return -1;
	}
	for (auto arg = 1; arg < ac; ++arg)
	{
		process_file(av[arg], bg_table, tg_table);
	}
	write_table(bg_table);
	write_table(tg_table);
}
