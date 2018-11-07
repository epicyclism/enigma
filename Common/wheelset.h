#pragma once

#include "wheel.h"
#include "position.h"

// wheel sets, cannot exist without appropriate wheel and reflector references.
//
class wheels3
{
private:
	// reflector
	wiring const&	ref_;
	// left to right
	wheel			w3_;
	wheel			w2_;
	wheel			w1_;
	// looking at the machine layout of wheels is,
	//
	// Ref W3 W2 W1 <-> plugboard <-> keyboard/lights
	//
public:
	// initialisation is reflector, left to right, as loaded into the actual machine
	constexpr wheels3( wiring const& ref, rotor const& w3, rotor const& w2, rotor const& w1  ) : ref_{ref}, w3_{w3}, w2_{w2}, w1_{w1}
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
	template <typename O> void PrintPosition(O& ostr) const
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
	position Position() const
	{
		return position(w3_.Base(), w2_.Base(), w1_.Base());
	}
	void Position(position const& p)
	{
		Setting(p.wp_[2], p.wp_[1], p.wp_[0]);
	}
	machine_settings_t Config() const
	{
		return machine_settings_t{ ref_.id_,
			w3_.rotor_.id_, w2_.rotor_.id_, w1_.rotor_.id_,
			w3_.ring_, w2_.ring_, w1_.ring_ };
	}
};