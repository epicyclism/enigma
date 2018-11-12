//
// encode the position of the Enigma rotors in four modalpha values.
// 
// can be printed, retrieved from the wheelset or applied to a wheelset.
//

#pragma once

#include <cstdint>
#include <iostream>
#include "modalpha.h"
#include "stecker.h"

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
	position(position const& other)
	{
		u_ = other.u_; // safe???????
	}
};

inline std::ostream& operator<<( std::ostream& o, position const& p)
{
	if (p.wp_[3] == alpha_max)
		o << p.wp_[2] << p.wp_[1] << p.wp_[0];
	else
		o << p.wp_[3] << p.wp_[2] << p.wp_[1] << p.wp_[0];
	return o;
}

// store a complete machine setup
// so we can permute rotors and get back to good stuff
//
struct machine_settings_t
{
	// reflector
	char ref_;
	// wheels
	char w3_;
	char w2_;
	char w1_;
	// ring
	modalpha     r3_;
	modalpha     r2_;
	modalpha     r1_;
	// plugs
	stecker stecker_;
	// position
	position pos_;
};

inline std::ostream& operator<<(std::ostream& o, machine_settings_t const& m)
{
	o << m.ref_ << m.w3_ << m.w2_ << m.w1_ << " ";
	o << to_printable_lower(m.r3_) << to_printable_lower(m.r2_) << to_printable_lower(m.r1_) ;
	o << m.pos_ << " \"";
	m.stecker_.Report(o);
	o << "\"";

	return o;
}

// a result for consideration.
//
struct result_t
{
	machine_settings_t mst_;
	double ioc_;
	result_t(machine_settings_t const& mst, double ioc) : mst_(mst), ioc_(ioc)
	{}
};