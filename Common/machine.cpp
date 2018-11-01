#include "machine.h"

// helper functions, throw on invalid
modalpha from_printable_throw(char ch)
{
	if (ch >= 'a' && ch <= 'z')
		return modalpha(ch - 'a');
	if (ch >= 'A' && ch <= 'Z')
		return modalpha(ch - 'A');
	// cannot be done as constexpr...
	throw std::out_of_range("requires a-z or A-Z");
}

void Ring(machine3& m3, char const cfg[3])
{
	const modalpha r3 = from_printable_throw(cfg[0]);
	const modalpha r2 = from_printable_throw(cfg[1]);
	const modalpha r1 = from_printable_throw(cfg[2]);
	m3.Ring(r3, r2, r1);
}

void Setting(machine3& m3, char const cfg[3])
{
	const modalpha s3 = from_printable_throw(cfg[0]);
	const modalpha s2 = from_printable_throw(cfg[1]);
	const modalpha s1 = from_printable_throw(cfg[2]);
	m3.Setting(s3, s2, s1);
}

void Stecker(machine3& m3, char f, char t)
{
	auto from = from_printable_throw(f);
	auto to = from_printable_throw(t);
	m3.Stecker(from, to);
}

// this expects a string of the form
// AB CD EF
// a string like
// ABDCEF
// will probably also work.
//
void Stecker(machine3& m3, char const* sS)
{
	std::string_view sv(sS);
	if (sv.empty())
		return;
	auto fst = sv.begin();
	auto snd = fst;
	++snd;
	int inc = 2;
	while (snd != sv.end())
	{
		if (inc > 1 && valid_from_char(*fst) && valid_from_char(*snd))
		{
			Stecker(m3, *fst, *snd);
			inc = 0;
		}
		// move on
		++snd;
		++fst;
		++inc;
	}
}

