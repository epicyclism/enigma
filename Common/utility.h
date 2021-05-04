#pragma once

// common small functions
// assumes the appropriate includes.
//
std::vector<modalpha> read_ciphertext(std::istream& is = std::cin)
{
	std::vector<modalpha> rv;
	while (1)
	{
		char c;
		is >> c;
		if (!is)
			break;
		if (valid_from_char(c))
		{
			rv.push_back(from_printable(c));
		}
	}
	return rv;
}

void report_ciphertext(std::vector<modalpha> const& ct, std::ostream& ostr)
{
	for (auto c : ct)
		ostr << c;
	ostr << '\n';
}
