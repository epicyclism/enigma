#pragma once

#include "wheel.h"

// wheel sets, cannot exist without appropriate wheel and reflector references.

class wheels3
{
private:
	// right to left
	wheel& w1_;
	wheel& w2_;
	wheel& w3_;
	// reflector
	wiring& ref_;
public:
	// initialisation is left to right, followed by reflector. Potential for confusion...
	wheels3( wheel& w3, wheel& w2, wheel& w1, wiring& ref ) : w1_{w1}, w2_{w2}, w3_{w3}, ref_{ref}
	{}
	// note these are also left to right
	void Ring(alpha w3, alpha w2, alpha w1)
	{
		w3_.ring_ = w3;
		w2_.ring_ = w2;
		w1_.ring_ = w1;
	}
	// note left to right...
	void Setting(alpha w3, alpha w2, alpha w1)
	{
		w3_.base_ = w3;
		w2_.base_ = w2;
		w2_.base_ = w1;
	}
	// all we can do is step and evaluate. Expect to evaluate all inputs for each step.
	void Step()
	{
		if (w1_.Step())
		{
			if (w2_.Step())
			{
				w3_.Step();
			}
		}
	}
	alpha Evaluate(alpha in) const
	{
		return w1_.LR(
				w2_.LR(
				 w3_.LR(
				  ref_.Tx(
				 w3_.RL(
				w2_.RL(
			   w1_.RL(in)))))));
	}
};