#pragma once

#include "wheel.h"

// wheel sets, cannot exist without appropriate wheel and reflector references.

class wheels3
{
private:
	// right to left
	wheel			w1_;
	wheel			w2_;
	wheel			w3_;
	// reflector
	wiring const&	ref_;
	// looking at the machine layout of wheels is,
	//
	// Ref W3 W2 W1 <-> plugboard <-> keyboard/lights
	//
public:
	// initialisation is left to right, followed by reflector. Potential for confusion...
	constexpr wheels3( rotor const& w3, rotor const& w2, rotor const& w1, wiring const& ref ) : w1_{w1}, w2_{w2}, w3_{w3}, ref_{ref}
	{}

	// note these are also left to right
	constexpr void Ring(modalpha w3, modalpha w2, modalpha w1)
	{
		w3_.Ring(w3);
		w2_.Ring(w2);
		w1_.Ring(w1);
	}
	// note left to right...
	constexpr void Setting(modalpha w3, modalpha w2, modalpha w1)
	{
		w3_.Base(w3);
		w2_.Base(w2);
		w1_.Base(w1);
	}
	// all we can do is step and evaluate. Expect to evaluate all inputs for each step.
	constexpr void Step()
	{
		if (w1_.Step())
		{
			if (w2_.Step())
			{
				w3_.Step();
			}
		}
		else
		if (w2_.VirtualStep())
		{
			w3_.Step();
		}
	}
	template<typename O> constexpr void Step(O& ostr)
	{
		ostr << "x";
		if (w1_.Step())
		{
			ostr << "x";
			if (w2_.Step())
			{
				ostr << "x";
				w3_.Step();
			}
			else
				ostr << "-";
		}
		else
		if (w2_.VirtualStep())
		{
			ostr << "Xx";
			w3_.Step();
		}
		else
			ostr << "--";
	}
	constexpr modalpha Evaluate(modalpha in) const
	{
		return w1_.LR(
				w2_.LR(
				 w3_.LR(
				  ref_.evalRef(
				 w3_.RL(
				w2_.RL(
			   w1_.RL(in)))))));
	}
	template<size_t N> constexpr std::array<modalpha, N> Evaluate(std::array<modalpha, N> const& in)
	{
		std::array<modalpha, N> out;
		std::transform(std::begin(in), std::end(in), std::begin(out), [&](auto m) { return Evaluate(m); });

		return out;
	}
	template <typename O> modalpha Evaluate(modalpha in, O& ostr) const
	{
		ostr << in;
		return w1_.LR(
				w2_.LR(
				 w3_.LR(
				  ref_.evalRef(
				 w3_.RL(
				w2_.RL(
			   w1_.RL(in, ostr), ostr), ostr), ostr), ostr), ostr), ostr);
	}
	template <typename O> void Position(O& ostr) const
	{
		ostr << w3_.pos_ << w2_.pos_ << w1_.pos_;
	}
	// produce a standard sort of machine settings report
	//
	template <typename O> void Report(O& ostr) const
	{
		// reflector and wheels
		ostr << ref_.id_ << w3_.rotor_.id_ << w2_.rotor_.id_ << w1_.rotor_.id_ << ' ';
		// ring
		ostr << to_printable_lower(w3_.ring_) << to_printable_lower(w2_.ring_) << to_printable_lower(w1_.ring_) << ' ';
		// base
		ostr << w3_.base_ << w2_.base_ << w1_.base_ << ' ';
	}
};