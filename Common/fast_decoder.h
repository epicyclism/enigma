#include "machine.h"
#include "arena.h"

// max message size, smaller the better I think for cache,
// power of two doesn't hurt.

constexpr size_t max_msg_size = 256;

class fast_decoder
{
private:
	arena_simple<max_msg_size> a_;
public:
	fast_decoder() = delete;
	fast_decoder(machine3& m3) // we're going to mess with this machine...
	{
		fill_arena(m3.Wheels(), a_, 0);
	}
	template<typename IC> std::vector<modalpha> decode(IC ctb, IC cte, stecker const& s) const
	{
		std::vector<modalpha> vo;
		size_t pos = 0;
		std::for_each(ctb, cte, [&](auto c) {
			// in stecker
			auto o = s.Eval(c);
			// rotor cache
			o = a_.arena_[o.Val()][pos];
			// out stecker 
			o = s.Eval(o);
			vo.push_back(o);
			++pos;
			});

		return vo;
	}
};