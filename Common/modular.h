#pragma once

#include <limits>

#if !defined(CONSTEXPR)
#if defined (__NVCC__)
#define CONSTEXPR
#else
#define CONSTEXPR constexpr
#endif
#endif

// attempt at a generic modular arithmetic type, add and subtract, range 0-(M-1)
//

template <int M, typename T = char> class mod_t
{
private :
	using base_t = std::conditional_t<std::is_enum_v<T>, std::underlying_type_t<T>, T>;
	base_t val_;

public:
	CONSTEXPR mod_t(T v) noexcept : val_{ static_cast<base_t>(v) }
	{		
	}
	CONSTEXPR mod_t() noexcept : val_{}
	{
	}
	CONSTEXPR mod_t ( mod_t const& t) noexcept : val_(t.val_)
	{}
	CONSTEXPR mod_t(int n) noexcept : val_(static_cast<base_t>(n % M))
	{
	}
	CONSTEXPR mod_t& operator++() noexcept
	{
		++val_;
		if (val_ == M)
			val_ = 0;
		return *this;
	}
	CONSTEXPR mod_t operator++(int) noexcept
	{
		mod_t t(*this);
		operator++();
		return t;
	}
	CONSTEXPR mod_t& operator--() noexcept
	{
		if (val_ == 0)
			val_ = M - 1;
		else
			--val_;
		return *this;
	}
	CONSTEXPR mod_t operator--(int) noexcept
	{
		mod_t t(*this);
		operator--();
		return t;
	}
	CONSTEXPR mod_t& operator+=(const mod_t& rhs) noexcept
	{
		val_ += rhs.val_;
#if 0
		val_ %= M;
#else
		if (val_ > M - 1)
			val_ -= M;
#endif
		return *this;
	}
	CONSTEXPR mod_t& operator-=(const mod_t& rhs) noexcept
	{
		if (rhs.val_ > val_)
			val_ += M;
		val_ -= rhs.val_;
		return *this;
	}
	CONSTEXPR friend mod_t operator+ (mod_t l, const mod_t& r) noexcept
	{
		l += r;
		return l;
	}
	CONSTEXPR friend mod_t operator- (mod_t l, const mod_t& r) noexcept
	{
		l -= r;
		return l;
	}
	CONSTEXPR friend bool operator< (const mod_t& l, const mod_t& r) noexcept
	{
		return l.val_ < r.val_;
	}
	friend CONSTEXPR bool operator> (const mod_t& l, const mod_t& r) noexcept { return r < l; }
	friend CONSTEXPR bool operator<=(const mod_t& l, const mod_t& r) noexcept { return !(l > r); }
	friend CONSTEXPR bool operator>=(const mod_t& l, const mod_t& r) noexcept { return !(l < r); }
	friend CONSTEXPR bool operator==(const mod_t& l, const mod_t& r) noexcept { return l.val_ == r.val_; }
	friend CONSTEXPR bool operator!=(const mod_t& l, const mod_t& r) noexcept { return !(l == r); }

	CONSTEXPR auto Val() const noexcept 
	{
		return val_;
	}
	auto Val_() const noexcept 
	{
		return val_;
	}
	CONSTEXPR explicit operator size_t() const noexcept
	{
		return val_;
	}
};
