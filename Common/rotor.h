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
	constexpr rotor() : wiring{}, to_ (0), dual_(false)
	{}
	constexpr rotor (wiring const& w, modalpha to, bool d) : wiring(w), to_(to), dual_(d)
	{}
};

constexpr rotor     I { FromDescWheel("EKMFLGDQVZNTOWYHXUSPAIBRCJ"), alpha::Q, false};
constexpr rotor    II { FromDescWheel("AJDKSIRUXBLHWTMCQGZNPYFVOE"), alpha::E, false };
constexpr rotor   III { FromDescWheel("BDFHJLCPRTXVZNYEIWGAKMUSQO"), alpha::V, false };
constexpr rotor    IV { FromDescWheel("ESOVPZJAYQUIRHXLNFTGKDCMWB"), alpha::J, false };
constexpr rotor     V { FromDescWheel("VZBRGITYUPSDNHLXAWMJQOFECK"), alpha::Z, false };
constexpr rotor    VI { FromDescWheel("JPGVOUMFYQBENHZRDKASXLICTW"), alpha::Z, true };
constexpr rotor   VII { FromDescWheel("NZJHGRCXMYSWBOUFAIVLPEKQDT"), alpha::Z, true };
constexpr rotor  VIII { FromDescWheel("FKQHTLXOCBJSPDZRAMEWNIUYGV"), alpha::Z, true };
constexpr rotor  beta { FromDescWheel("LEYJVCNIXWPBQMDRTAKZGFUHOS"), alpha::Z, true };
constexpr rotor gamma { FromDescWheel("FSOKANUERHMBTIYCWLQPZXVGJD"), alpha::Z, true };
