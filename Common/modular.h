#pragma once

#include <limits>

// attempt at a generic modular arithmetic type, add and subtract, range 0-(M-1)
//

template <int M, typename T = char> class mod_t
{
private :
	using base_t = std::conditional_t<std::is_enum_v<T>, std::underlying_type_t<T>, T>;
	base_t val_;

public:
	constexpr mod_t(T v) : val_{ static_cast<base_t>(v) }
	{		
	}
	constexpr mod_t() : val_{}
	{
	}
	constexpr mod_t ( mod_t const& t) : val_(t.val_)
	{}
	constexpr mod_t(int n) : val_(static_cast<base_t>(n % M))
	{
	}
	constexpr mod_t& operator++()
	{
		++val_;
		if (val_ == M)
			val_ = 0;
		return *this;
	}
	constexpr mod_t operator++(int)
	{
		mod_t t(*this);
		operator++();
		return t;
	}
	mod_t& operator--()
	{
		if (val_ == 0)
			val_ = M - 1;
		else
			--val_;
		return *this;
	}
	mod_t operator--(int)
	{
		mod_t t(*this);
		operator--();
		return t;
	}
	constexpr mod_t& operator+=(const mod_t& rhs)
	{
		val_ += rhs.val_;
		val_ %= M;
		return *this;
	}
	constexpr mod_t& operator-=(const mod_t& rhs)
	{
		if (rhs.val_ > val_)
			val_ += M;
		val_ -= rhs.val_;
		return *this;
	}
	constexpr friend mod_t operator+ (mod_t l, const mod_t& r)
	{
		l += r;
		return l;
	}
	constexpr friend mod_t operator- (mod_t l, const mod_t& r)
	{
		l -= r;
		return l;
	}
	constexpr friend bool operator< (const mod_t& l, const mod_t& r)
	{
		return l.val_ < r.val_;
	}
	friend bool operator> (const mod_t& l, const mod_t& r) { return r < l; }
	friend bool operator<=(const mod_t& l, const mod_t& r) { return !(l > r); }
	friend bool operator>=(const mod_t& l, const mod_t& r) { return !(l < r); }
	friend bool operator==(const mod_t& l, const mod_t& r) { return l.val_ == r.val_; }
	friend bool operator!=(const mod_t& l, const mod_t& r) { return !(l == r); }

	constexpr auto Val() const
	{
		return val_;
	}
};