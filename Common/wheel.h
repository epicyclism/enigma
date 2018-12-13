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
	modalpha ring_; 
	// the base setting
	modalpha base_;
	// the current position
	modalpha pos_;
	// steps to next rollover
	modalpha roll_;

	// cannot have a wheel without a rotor...
	constexpr wheel ( rotor const& r) : rotor_ { r}, ring_{alpha::A}, base_(alpha::A), pos_(alpha::A), roll_{r.to_}
	{}
	constexpr wheel (rotor const& r, alpha ring, alpha base) : rotor_{ r }, ring_{ ring }, base_(base), pos_(alpha::A), roll_{ r.to_ - base}
	{}

	constexpr modalpha RL(modalpha in) const
	{
		return in + rotor_.evalRL(pos_ + in);
	}
	constexpr modalpha LR(modalpha in) const
	{
		return in + rotor_.evalLR(pos_ + in);
	}
	template<typename O> modalpha RL(modalpha in, O& ostr) const
	{
		auto r = in + rotor_.evalRL(pos_ + in);
		ostr << "-" << r;
		return r;
	}
	template<typename O> modalpha LR(modalpha in, O& ostr) const
	{
		auto r = in + rotor_.evalLR(pos_ + in);
		ostr << "-" << r ;
		return r;
	}
	constexpr void Ring(modalpha r)
	{
		ring_ = r;
	}
	constexpr void Base(modalpha b)
	{
		base_ = b;
		pos_ = b - ring_ ;
		roll_ = rotor_.to_ - b;
	}
	constexpr modalpha Base() const
	{
		return pos_ + ring_;
	}
	constexpr bool Step()
	{
		++pos_;
		if (roll_ == 0)
		{
			roll_ = rotor_.dual_ ? alpha_max / 2 : alpha_max; // align mechanism with Base calculation? TODO
			--roll_;
			return true;
		}
		--roll_;
		return false;
	}
	constexpr bool VirtualStep()
	{
		if (roll_ == 0)
		{
			roll_ = rotor_.dual_ ? alpha_max / 2 : alpha_max; // align mechanism with Base calculation? TODO
			--roll_;
			++pos_;
			return true;
		}
		return false;
	}
};

inline bool valid_ring_id(char ch)
{
	if ( ch >= 'a' && ch <= 'z' )
		return true;
	return false;
}