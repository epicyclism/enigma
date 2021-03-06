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
	stecker s_b_;
public:
	// must be constructed with wheel references!
	machine3() = delete;
	constexpr machine3(wiring const& ref, rotor const& w3, rotor const& w2, rotor const& w1) : w_(ref, w3, w2, w1)
	{}
	// access the wheelset alone
	[[nodiscard]] constexpr wheels3& Wheels() noexcept
	{
		return w_;
	}
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
	void ClearPlugs() noexcept
	{
		s_.Clear();
	}
	// retrieve
	[[nodiscard]] constexpr stecker GetPlugs() const noexcept
	{
		return s_;
	}
	// add a plug pair, this cannot fail, you can map from and too to
	// the same, but that's fine, like clear that pair...
	//
	void Stecker(modalpha f, modalpha t)
	{
		s_.Set(f, t);
	}
	[[nodiscard]] modalpha Transform(modalpha m) noexcept
	{
		if (m == modalpha(alpha::SZ))
			return modalpha(alpha::SZ);
		w_.Step();
		auto x = s_.Eval(m);
		x = w_.Evaluate(x);
		return s_.Eval(x);
	}
	[[nodiscard]] modalpha Transform(modalpha m, std::ostream& ostr) noexcept
	{
		if (m == modalpha(alpha::SZ))
			return modalpha(alpha::SZ);
		w_.Step(ostr);
		ostr << ' ' << m;
		auto x = s_.Eval(m, ostr);
		x = w_.Evaluate(x, ostr);
		return s_.Eval(x, ostr);
	}
	template<typename I, typename O> void Transform(I b, I e, O o) noexcept
	{
		std::transform(b, e, o, [&](auto in)
		{
			if (in == modalpha(alpha::SZ))
				return modalpha(alpha::SZ);
			w_.Step();
			auto v = s_.Eval(in);
			v = w_.Evaluate(v);
			return s_.Eval(v);
		});
	}
	template<typename I> std::vector<modalpha> Transform(I b, I e) noexcept
	{
		std::vector<modalpha> r;
		r.reserve(std::distance(b, e));
		Transform(b, e, std::back_inserter(r));
		return r;
	}
	template<typename I, typename OSTR> std::vector<modalpha> Transform(I b, I e, OSTR& ostr)
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
	template<typename OSTR> void ReportSettings(OSTR& ostr)
	{
		w_.Report(ostr);
		ostr << " ";
		s_.Report(ostr);
	}
	// access
	stecker const& Plugs() const
	{
		return s_;
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
	[[nodiscard]] size_t CycleLength() const noexcept 
	{
		return w_.CycleLen();
	}
	// get machine settings so can recreate as wanted.
	machine_settings_t machine_settings() const
	{
		machine_settings_t mst = w_.Config();
		mst.stecker_ = s_;
		mst.pos_ = Position();
		return mst;
	}
	void machine_settings(machine_settings_t const& mst)
	{
		s_ = mst.stecker_;
		w_.Ring(mst.r3_, mst.r2_, mst.r1_);
		w_.Position(mst.pos_);
	}
	// manipulate the plugs
	// get the set so we can restore
	void PushStecker() noexcept
	{
		s_b_ = s_;
	}
	// put the set back
	void PopStecker() noexcept
	{
		s_ = s_b_;
	}
	// apply destructively.
	void ApplyPlug(modalpha f, modalpha t) noexcept
	{
		if (s_.Is(f, t))
		{
			s_.Clear(f);
			s_.Clear(t);
		}
		s_.Set(f, t);
	}
	[[nodiscard]] auto SteckerCount() const noexcept
	{
		return s_.Count();
	}
	[[nodiscard]] modalpha Eval(modalpha m) noexcept
	{
		return s_.Eval(m);
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

// these take a char so we don't have to provide strings...
//
inline rotor const& rotor_from_name_throw(char nm)
{
	switch (nm)
	{
	case '1':
		return I;
	case '2':
		return II;
	case '3':
		return III;
	case '4':
		return IV;
	case '5':
		return V;
	case '6':
		return VI;
	case '7':
		return VII;
	case '8':
		return VIII;
	case 'b':
		return beta;
	case 'g':
		return gamma;
	default:
		break;
	}
	throw std::out_of_range("invalid wheel name");
}

inline wiring const& reflector_from_name_throw(char nm)
{
	switch (nm)
	{
	case 'B':
		return B;
	case 'C':
		return C;
	case 'b':
		return B_M4;
	case 'c':
		return C_M4;
	default:
		break;
	}
	throw std::out_of_range("invalid reflector name");
}

// helper to build a wheelset
//
inline wheels3 MakeWheels3(machine_settings_t const& mst)
{
	auto& ref = reflector_from_name_throw(mst.ref_);
	auto& w3 = rotor_from_name_throw(mst.w3_);
	auto& w2 = rotor_from_name_throw(mst.w2_);
	auto& w1 = rotor_from_name_throw(mst.w1_);
	wheels3 ws3(ref, w3, w2, w1);
	ws3.Ring(mst.r3_, mst.r2_, mst.r1_);
	ws3.Position(position());

	return ws3;
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

// build a machine from the settings
//
inline machine3 MakeMachine3(machine_settings_t const& mst)
{
	auto& ref = reflector_from_name_throw(mst.ref_);
	auto& w3  = rotor_from_name_throw(mst.w3_);
	auto& w2  = rotor_from_name_throw(mst.w2_);
	auto& w1  = rotor_from_name_throw(mst.w1_);
	machine3 m3(ref, w3, w2, w1);
	m3.machine_settings(mst);

	return m3;
}

// helper functions to set a machine up with readable parameters.
void Ring(machine3& m3, char const cfg[3]);
void Setting(machine3& m3, char const cfg[3]);
void Stecker(machine3& m3, char f, char t);
void Stecker(machine3& m3, char const* sS);

void Ring(machine_settings_t& mst, char const cfg[3]);
void Stecker(machine_settings_t& mst, char f, char t);
void Stecker(machine_settings_t& mst, char const* sS);

inline void ZeroRing(machine_settings_t& mst)
{
	mst.r1_ = modalpha(0);
	mst.r2_ = modalpha(0);
	mst.r3_ = modalpha(0);
}

inline bool AdvanceRing(machine_settings_t& mst)
{
	++mst.r1_;
	if (mst.r1_ == modalpha(0))
		return false; // finished!

	// don't care about r2 and r3.
	return true;
}

inline bool AdvanceRingAll(machine_settings_t& mst)
{
	++mst.r1_;
	if (mst.r1_ == modalpha(0))
	{
		++mst.r2_;
		if (mst.r2_ == modalpha(0))
			return false; // finished!
	}
	// don't care about r3.
	return true;
}

// trivial decoder
// takes output container by reference so it can be called repeatedly without reallocation
// (because from the callers pov the length is not changing...)
// leaves the machine in the state it found it
//
template<typename IC, typename O> void decode(IC ctb, IC cte, machine3& m3, O& o)
{
	o.clear();
	position pos = m3.Position();
	while (ctb != cte)
	{
		o.push_back(m3.Transform(*ctb));
		++ctb;
	}
	// reset machine
	m3.Position(pos);
}

// as above but assumes output buffer is iteratable
//
template<typename IC, typename IO> void decode_tx(IC ctb, IC cte, IO ot, machine3& m3)
{
	position pos = m3.Position();
	while (ctb != cte)
	{
		*ot = m3.Transform(*ctb);
		++ot;
		++ctb;
	}
	// reset machine
	m3.Position(pos);
}

// won't constexpr right now. damn it.
//
// when called with a string the trailing null is included in N.
// so adapt.
//
template<typename M, std::size_t N> constexpr auto make_alpha_array(const M(&msg)[N])
{
	static_assert(N > 1, "Trying to call 'make_alpha_array' with an empty argument.");

	std::array<modalpha, N-1> rv;
	epicyclism::ctransform(std::begin(msg), std::end(msg)-1, std::begin(rv),
		[](M c)
	{
		return from_printable(c);
	});

	return rv;
}
