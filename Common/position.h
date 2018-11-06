//
// encode the position of the Enigma rotors in four modalpha values.
// 
// can be printed, retrieved from the wheelset or applied to a wheelset.
//

#pragma once

#include <cstdint>
#include <iostream>
#include "modalpha.h"

struct position
{
	union
	{
		modalpha wp_[4];
		uint_fast32_t u_;
	};
	// cater for four wheels, in 3 wheel case the fourth wheel is at alpha_max and so invalid.
	// left to right in the machine, so reverse of how we might write it.
	//
	position() : wp_{ alpha::A, alpha::A, alpha::A, alpha::SZ }
	{}
	position(modalpha w3, modalpha w2, modalpha w1) : wp_{ w1, w2, w3, alpha_max}
	{}
	position(modalpha w4, modalpha w3, modalpha w2, modalpha w1) : wp_{ w1, w2, w3, w4}
	{}
};

inline std::ostream& operator<<( std::ostream& o, position const& p)
{
	if (p.wp_[3] == alpha_max)
		o << p.wp_[2] << p.wp_[1] << p.wp_[0];
	else
		o << p.wp_[3] << p.wp_[2] << p.wp_[1] << p.wp_[0];
	return o;
}