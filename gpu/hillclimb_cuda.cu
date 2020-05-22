#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>

#include <cuda.h>

#include "hillclimb_cuda.h"
#include <thrust/copy.h>
#include <thrust/for_each.h>

cudaWrap::cudaWrap(bigram_table const& bgt, trigram_table const& tgt, std::vector<modalpha> const& ct)
{
	bgt_ = nullptr;
	tgt_ = nullptr;
	adt_ = nullptr;
	int deviceCount = 0;
	cudaError_t err_id = cudaGetDeviceCount(&deviceCount);
	if (err_id != cudaSuccess)
	{
		std::cout << "cudaGetDeviceCount returned " << err_id << "(" << cudaGetErrorString(err_id) << '\n';
		return ;
	}
	if (deviceCount < 1)
	{
		std::cout << "No Cuda devices found\n";
		return ;
	}
	std::cout << "CUDA deviceCount = " << deviceCount << '\n';

	// copy in the bigram table of the day
	cudaMalloc(reinterpret_cast<void**>(&bgt_), sizeof(bigram_table));
	cudaMemcpy(bgt_, &bgt, sizeof(bigram_table), cudaMemcpyHostToDevice);

	// copy in the trigram table of the day
	cudaMalloc(reinterpret_cast<void**>(&tgt_), sizeof(trigram_table));
	cudaMemcpy(tgt_, &tgt, sizeof(trigram_table), cudaMemcpyHostToDevice);

	// reserve space for the arena
	cudaMalloc(reinterpret_cast<void**>(&adt_), sizeof(arena_decode_t));

	// copy the ciphertext to the device
	ct_ = ct;
}

cudaWrap::~cudaWrap()
{
	cudaFree(adt_);
	cudaFree(tgt_);
	cudaDeviceReset();
}

bool cudaWrap::cudaGood() const
{
	return tgt_ != nullptr && adt_ != nullptr;
}

void cudaWrap::set_arena(arena_decode_t const& a)
{
	// copy in the arena for this wheelorder/ring setting
	cudaMemcpy(adt_, &a, sizeof(arena_decode_t), cudaMemcpyHostToDevice);
}

void cudaWrap::sync_joblist_to_device(std::vector<cudaJob> const& jl)
{
	vjd_ = vjh_;
}

void cudaWrap::sync_joblist_from_device(std::vector<cudaJob>& jl)
{
	vjh_ = vjd_;
}

// localise the hillclimb code(s) to here for now as functors
//
class fast_decoder_ref
{
private:
	arena_decode_t::pos_t const*	ai_;
	thrust::device_vector<modalpha> vo_;

public:
	fast_decoder_ref() = delete;
	__device__
	fast_decoder_ref(arena_decode_t::pos_t const* ai) : ai_(ai)
	{
	}
	__device__
	thrust::device_vector<modalpha> const& decode(thrust::device_vector<modalpha> const& ct, stecker const& s)
	{
		vo_.resize(ct.size());
		std::transform(ct.begin(), ct.end(), ai_, vo_.begin(), [&](auto c, auto const& a)
			{
				// in stecker
				auto o = s.Eval(c);
				// rotor cache
				o = a[o.Val_()];
				// out stecker 
				o = s.Eval(o);
				return o;
			});

		return vo_;
	}
};
#if 0
double index_of_coincidence(thrust::device_vector<modalpha> const& pt)
{
	std::array<unsigned, alpha_max> tab;
	tab.fill(0);

	// count
	std::for_each(pt.begin(), pt.end(), [&tab](auto c)
		{
			++tab[c.Val()];
		});
	// calculate
	double nn = double(pt.size()) * double(pt.size() - 1);

	return 	std::inner_product(std::begin(tab), std::end(tab), 0.0, std::plus<double>{}, [nn](auto n)
		{
			return double(n * (n - 1)) / nn;
		});
}
#endif
struct bigram_score_op
{
	bigram_table const* bgt_;
	__device__
	bigram_score_op(bigram_table const* bgt) : bgt_(bgt)
	{}
	__device__
	unsigned operator()(thrust::device_vector<modalpha> const& pt)
	{
		unsigned score = 0;
		auto b = pt.begin();
		auto n = b + 1;
		while (n != pt.end())
		{
			score += bgt_->wt(*b, *n);
			++b;
			++n;
		}
		// normalise to message length here, right down at the source.
		return score / (static_cast<unsigned>(pt.size()) - 1);
	}
};

__device__ struct trigram_score_op
{
	trigram_table const* tgt_;
	__device__
	trigram_score_op(trigram_table const* tgt) : tgt_(tgt)
	{}
	__device__
	unsigned operator()(thrust::device_vector<modalpha> const& pt)
	{
		unsigned score = 0;
		auto b = pt.begin();
		auto m = b + 1;
		auto n = m + 1;
		while (n != pt.end())
		{
			score += tgt_->wt(*b, *m, *n);
			++b;
			++m;
			++n;
		}
		return score / (static_cast<unsigned>(pt.size()) - 2);
	}
};

template<typename F, typename FD, size_t max_stecker = 10 > auto hillclimb_base_fast(thrust::device_vector<modalpha> const& ct, F eval_fn, double iocb, FD& fd, stecker& s_base)
{
	stecker s = s_base;
	stecker s_b;
	auto vo = fd.decode(ct, s);
#if 0
	auto iocs = index_of_coincidence(vo);
	if (iocs * .95 < iocb)
		return 0U;
#endif
	// establish the baseline
	auto scr = eval_fn(vo);
	bool improved = true;
	while (improved)
	{
		improved = false;
		modalpha mx = 0;
		modalpha my = 0;
		for (int fi = 0; fi < alpha_max; ++fi)
		{
			modalpha f{ fi };
			for (int ti = fi; ti < alpha_max; ++ti)
			{
				modalpha t{ ti };
				s_b = s;
				s.Apply(f, t);
				vo = fd.decode(ct, s);
				auto scrn = eval_fn(vo);
				if (scrn > scr && s.Count() < max_stecker + 1)
				{
					mx = f;
					my = t;
					scr = scrn;
					improved = true;
				}
				s = s_b;
			}
		}
		if (improved)
			s.Apply(mx, my);
	}
	s_base = s;

	return scr;
}

__device__ struct hillclimb_bgtg_fast
{
	thrust::device_vector<modalpha> const& ct_;
	arena_decode_t const* ai_;
	bigram_table const* bgt_;
	trigram_table const* tgt_;

	__device__
	hillclimb_bgtg_fast(thrust::device_vector<modalpha> const& ct, arena_decode_t const* ai, bigram_table const* bgt, trigram_table const* tgt) : ct_(ct), ai_(ai), bgt_(bgt), tgt_(tgt)
	{}

	__device__
	void operator()( cudaJob& cj)
	{
		fast_decoder_ref fd(ai_->arena_.data() + cj.off_);
		hillclimb_base_fast(ct_, bigram_score_op(bgt_), 0.0, fd, cj.s_);
		cj.scr_ = hillclimb_base_fast(ct_, trigram_score_op(tgt_), 0.0, fd, cj.s_);
	}
};

void cudaWrap::proc()
{
	thrust::for_each(vjd_.begin(), vjd_.end(), hillclimb_bgtg_fast(ct_, adt_, bgt_, tgt_));
}