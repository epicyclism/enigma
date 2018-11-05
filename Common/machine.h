#pragma once

#include <stdexcept>
#include <string_view>
#include <vector>
#include <algorithm>
#include "wheelset.h"
#include "stecker.h"

class machine3
{
private:
	wheels3 w_;
	stecker s_;
public:
	// must be constructed with wheel references!
	machine3() = delete;
	constexpr machine3(wiring const& ref, rotor const& w3, rotor const& w2, rotor const& w1 ) : w_(ref, w3, w2, w1)
	{}
	// direct settings
	// note left to right...
	constexpr void Ring(modalpha r3, modalpha r2, modalpha r1) noexcept
	{
		w_.Ring(r3, r2, r1);
	}
	// note left to right...
	constexpr void Setting(modalpha s3, modalpha s2, modalpha s1) noexcept
	{
		w_.Setting(s3, s2, s1);
	}
	// clear
	void Stecker()
	{
		s_.Clear();
	}
	// add a plug pair, this cannot fail, you can map from and too to
	// the same, but that's fine, like clear that pair...
	//
	void Stecker(modalpha f, modalpha t)
	{
		s_.Set(f, t);
	}
	modalpha Transform(modalpha m)
	{
		if (m == modalpha(alpha::SZ))
			return modalpha(alpha::SZ);
		w_.Step();
		auto x = s_.Eval(m);
		x = w_.Evaluate(x);
		return s_.Eval(x);
	}
	template<typename I> std::vector<modalpha> Transform(I b, I e)
	{
		std::vector<modalpha> r;
		std::transform(b, e, std::back_inserter(r), [&](auto in)
		{
			if (in == modalpha(alpha::SZ))
				return modalpha(alpha::SZ);
			w_.Step();
			auto v = s_.Eval(in);
			v = w_.Evaluate(v);
			return s_.Eval(v);
		});
		return r;
	}
	template<typename I, typename O> std::vector<modalpha> Transform(I b, I e, O& ostr)
	{
		std::vector<modalpha> r;
		std::transform(b, e, std::back_inserter(r), [&](auto in)
		{
			if (in == modalpha(alpha::SZ))
				return modalpha(alpha::SZ);
			w_.Step(ostr);
			ostr << in;
			auto v = s_.Eval(in, ostr);
			v = w_.Evaluate(v, ostr);
			auto r = s_.Eval(v, ostr);
			ostr << "\n";
			return r;
		});
		return r;
	}
	template<typename O> void ReportSettings(O& ostr)
	{
		w_.Report(ostr);
		ostr << " ";
		s_.Report(ostr);
	}
	// forward on
	position Position() const
	{
		return w_.Position();
	}
	void Position(position const& p)
	{
		w_.Position(p);
	}
};

// these take a string_view so we can provide strings...
//
inline rotor const& rotor_from_name_throw(std::string_view nm)
{
	// yawn...
	if (nm == "1" || nm == "I")
		return I;
	if (nm == "2" || nm == "II")
		return II;
	if (nm == "3" || nm == "III")
		return III;
	if (nm == "4" || nm == "IV")
		return IV;
	if (nm == "5" || nm == "V")
		return V;
	if (nm == "6" || nm == "VI")
		return VI;
	if (nm == "7" || nm == "VII")
		return VII;
	if (nm == "8" || nm == "VIII")
		return VIII;
	if (nm == "b" || nm == "beta")
		return beta;
	if (nm == "g" || nm == "gamma")
		return gamma;
	throw std::out_of_range("invalid wheel name");
}

inline wiring const& reflector_from_name_throw(std::string_view nm)
{
	if (nm == "B")
		return B;
	if (nm == "C")
		return C;
	if (nm == "b" || nm == "B_M4")
		return B_M4;
	if (nm == "c" || nm == "C_M4")
		return C_M4;
	throw std::out_of_range("invalid reflector name");
}

// take a string like 'B321' and return a suitably configured machine.
// throws on nonsense.
//
// reflector/wheel/wheel/wheel in the order they appear in an actual machine
//
inline machine3 MakeMachine3(char const desc[4])
{
	auto& ref = reflector_from_name_throw(std::string_view(desc, 1));
	auto& w3  = rotor_from_name_throw(std::string_view(desc + 1, 1));
	auto& w2  = rotor_from_name_throw(std::string_view(desc + 2, 1));
	auto& w1  = rotor_from_name_throw(std::string_view(desc + 3, 1));
	return machine3(ref, w3, w2, w1);
}

// helper functions to set a machine up with readable parameters.
void Ring(machine3& m3, char const cfg[3]);
void Setting(machine3& m3, char const cfg[3]);
void Stecker(machine3& m3, char f, char t);
void Stecker(machine3& m3, char const* sS);

// won't constexpr right now. damn it.
//
template<typename M, std::size_t N> constexpr auto make_alpha_array(const M(&msg)[N])
{
	std::array<modalpha, N> rv;
	epicyclism::ctransform(std::begin(msg), std::end(msg), std::begin(rv),
		[](M c)
	{
		return from_printable(c);
	});

	return rv;
}
