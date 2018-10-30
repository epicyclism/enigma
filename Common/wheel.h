#pragma once

// the variable parts of a wheel.
// wheels get assembled into machines...
//

#include "rotor.h"

struct wheel
{
	// constant part, the internal wiring and turnover details
	//
	rotor const& rotor_;
	// variable bits
	//
	// the base ring setting.
	alpha ring_; 
	// the base setting
	alpha base_;
	// steps to next rollover
	alpha roll_;

	// cannot have a wheel without a rotor...
	wheel ( rotor const& r) : rotor_ { r}, ring_{alpha::A}, base_(alpha::A), roll_{r.to_}
	{}
	wheel (rotor const& r, alpha ring, alpha base) : rotor_{ r }, ring_{ ring }, base_(base), roll_{ r.to_ - base}
	{}

	alpha RL(alpha in)
	{
		alpha off = base_ + in;
		return in + rotor_.rl_[off];
	}

	alpha LR(alpha in)
	{
		alpha off = base_ + in;
		return in + rotor_.lR_[off];
	}

	bool Step()
	{
		++base_;
		if (roll_ == 0)
		{
			roll_ = rotor_.dual_ ? alpha_max / 2 : alpha_max;
			--roll_;
			return true;
		}
		--roll_;
		return false;
	}
};