#pragma once

// the remaining fixed, const,  parts of a wheel.
//

#include "wiring.h"

struct rotor : public wiring
{
	// turnover
	modalpha to_;
	// dual turnover
	bool     dual_;

	constexpr rotor() = delete;
	constexpr rotor (wiring const& w, modalpha to, bool d) noexcept : wiring(w), to_(to), dual_(d)
	{}
};

constexpr rotor     I { FromDescWheel("EKMFLGDQVZNTOWYHXUSPAIBRCJ", '1'), alpha::Q, false };
constexpr rotor    II { FromDescWheel("AJDKSIRUXBLHWTMCQGZNPYFVOE", '2'), alpha::E, false };
constexpr rotor   III { FromDescWheel("BDFHJLCPRTXVZNYEIWGAKMUSQO", '3'), alpha::V, false };
constexpr rotor    IV { FromDescWheel("ESOVPZJAYQUIRHXLNFTGKDCMWB", '4'), alpha::J, false };
constexpr rotor     V { FromDescWheel("VZBRGITYUPSDNHLXAWMJQOFECK", '5'), alpha::Z, false };
constexpr rotor    VI { FromDescWheel("JPGVOUMFYQBENHZRDKASXLICTW", '6'), alpha::Z, true };
constexpr rotor   VII { FromDescWheel("NZJHGRCXMYSWBOUFAIVLPEKQDT", '7'), alpha::Z, true };
constexpr rotor  VIII { FromDescWheel("FKQHTLXOCBJSPDZRAMEWNIUYGV", '8'), alpha::Z, true };
constexpr rotor  beta { FromDescWheel("LEYJVCNIXWPBQMDRTAKZGFUHOS", 'b'), alpha::Z, true };
constexpr rotor gamma { FromDescWheel("FSOKANUERHMBTIYCWLQPZXVGJD", 'g'), alpha::Z, true };

inline bool valid_rotor_id(char ch)
{
	switch (ch)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case 'b':
	case 'g':
		return true;
	default:
		break;
	}
	return false;
}