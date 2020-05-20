#include "machine.h"
#include "arena.h"

// max message size, smaller the better I think for cache,
// power of two doesn't hurt.

constexpr size_t max_msg_size = 256;

class fast_decoder
{
private:
	arena_simple<max_msg_size> a_;
	std::vector<modalpha>      vo_;

public:
	fast_decoder() = delete;
	fast_decoder(machine3& m3) // we're going to mess with this machine...
	{
		fill_arena_simple(m3.Wheels(), a_);
		vo_.reserve(max_msg_size);
	}
	template<typename IC> std::vector<modalpha> const& decode(IC ctb, IC cte, stecker const& s)
	{
		vo_.resize(std::distance(ctb, cte));
		std::transform(ctb, cte, a_.arena_.begin(), vo_.begin(), [&](auto c, auto const& a)
			{
				// in stecker
				auto o = s.Eval(c);
				// rotor cache
				o = a[o.Val()];
				// out stecker 
				o = s.Eval(o);
				return o;
			});

		return vo_;
	}
};

// accepts an external pointer into a full arena
template<typename AI> class fast_decoder_ref
{
private:
	AI						ai_;
	std::vector<modalpha>   vo_;

public:
	fast_decoder_ref() = delete;
	fast_decoder_ref(AI ai) : ai_(ai) 
	{
		vo_.reserve(max_msg_size);
	}
	template<typename IC> std::vector<modalpha> const& decode(IC ctb, IC cte, stecker const& s)
	{
		vo_.resize(std::distance(ctb, cte));
		std::transform(ctb, cte, ai_, vo_.begin(), [&](auto c, auto const& a)
			{
				// in stecker
				auto o = s.Eval(c);
				// rotor cache
				o = a[o.Val()];
				// out stecker 
				o = s.Eval(o);
				return o;
			});

		return vo_;
	}
};