#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>

#include <cuda.h>
#include <cuda_runtime.h>

#include "hillclimb_cuda.h"

cudaWrap::cudaWrap(bigram_table const& bgt, trigram_table const& tgt, std::vector<modalpha> const& ct)
{
	bgt_ = nullptr;
	tgt_ = nullptr;
	adt_ = nullptr;
	ct_ = nullptr;
	jl_ = nullptr;
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

	// reserve and copy ciphertext
	ctl_ = static_cast<unsigned>(ct.size());
	cudaMalloc(reinterpret_cast<void**>(&ct_), ctl_);
	cudaMemcpy(ct_, ct.data(), ctl_, cudaMemcpyHostToDevice);
}

cudaWrap::~cudaWrap()
{
	cudaFree(adt_);
	cudaFree(tgt_);
	cudaFree(ct_);
	cudaFree(jl_);
	cudaDeviceReset();
}

bool cudaWrap::cudaGood() const
{
	if (tgt_ != nullptr && adt_ != nullptr && ct_ != nullptr)
	{
		// wait
		auto err = cudaDeviceSynchronize();
		if (err != cudaSuccess)
		{
			std::cout << "Cuda Launch Error - " << err << ": " << cudaGetErrorString(err) << '\n';
			return false;
		}
		return true;
	}
	return false;
}

void cudaWrap::set_arena(arena_decode_t const& a)
{
	// copy in the arena for this wheelorder/ring setting
	cudaMemcpy(adt_, &a, sizeof(arena_decode_t), cudaMemcpyHostToDevice);
}

void cudaWrap::sync_joblist_to_device(std::vector<cudaJob> const& jl)
{
	if (jl_ == nullptr)
	{
		jls_ = static_cast<unsigned>(jl.size());
		auto err = cudaMalloc(reinterpret_cast<void**>(&jl_), sizeof(cudaJob) * jls_);
		if (err != cudaSuccess)
		{
			std::cout << "cudaMalloc Error - " << err << ": " << cudaGetErrorString(err) << '\n';
		}

	}
	auto err = cudaMemcpy(jl_, jl.data(), jls_ * sizeof(cudaJob), cudaMemcpyHostToDevice);
	if (err != cudaSuccess)
	{
		std::cout << "cudaMemcpy Error - " << err << ": " << cudaGetErrorString(err) << '\n';
	}
}

void cudaWrap::sync_joblist_from_device(std::vector<cudaJob>& jl)
{
	auto err = cudaMemcpy(jl.data(), jl_, jls_ * sizeof(cudaJob), cudaMemcpyDeviceToHost);
	if (err != cudaSuccess)
	{
		std::cout << "cudaMemcpy2 Error - " << err << ": " << cudaGetErrorString(err) << '\n';
	}
}

// localise the hillclimb code(s) to here for now as functors
//
class fast_decoder_ptr
{
private:
	modalpha const*	ai_;
	modalpha		vo_[256];
public:
	fast_decoder_ptr() = delete;
	__device__
	fast_decoder_ptr(modalpha const* ai) : ai_(ai)
	{
	}
	__device__
	~fast_decoder_ptr()
	{
	}
	__device__
	modalpha* decode(modalpha const* ctb, modalpha const* cte, stecker const& s)
	{
		modalpha const* a = ai_;
		auto pt = vo_;
		while (ctb != cte)
		{
			auto o = s.Eval(*ctb);
			o = a[o.Val()];
			o = s.Eval(o);
			*pt = o;
			++pt;
			++ctb;
			a += alpha_max;
		}
		return vo_;
	}
};

__device__ double index_of_coincidence(modalpha const* ctb, unsigned ctl)
{
	unsigned tab[alpha_max];
	for (auto& t : tab)
		t = 0;

	// count
	while(ctb != ctb + ctl)
		++tab[(*ctb).Val()];

	// calculate
	double nn = double(ctl) * double(ctl - 1);
	double rv = 0.0;
	for (auto t : tab)
		rv += double(t * (t - 1)) / nn;
	return rv;
}

struct bigram_score_op
{
	bigram_table const* bgt_;
	__device__
	bigram_score_op(bigram_table const* bgt) : bgt_(bgt)
	{}
	__device__
	unsigned operator()(modalpha const* ptb, modalpha const* pte)
	{
		unsigned score = 0;
		auto b = ptb;
		auto n = ptb + 1;
		while (n != pte)
		{
			score += bgt_->wt(*b, *n);
			++b;
			++n;
		}
		// normalise to message length here, right down at the source.
		return score / (static_cast<unsigned>(pte - ptb) - 1);
	}
};

struct trigram_score_op
{
	trigram_table const* tgt_;
	__device__
	trigram_score_op(trigram_table const* tgt) : tgt_(tgt)
	{}
	__device__
	unsigned operator()(modalpha const* ptb, modalpha const* pte)
	{
		unsigned score = 0;
		auto b = ptb;
		auto m = b + 1;
		auto n = m + 1;
		while (n != pte)
		{
			score += tgt_->wt(*b, *m, *n);
			++b;
			++m;
			++n;
		}
		return score / (static_cast<unsigned>(pte - ptb) - 2);
	}
};


template<typename F, typename FD, size_t max_stecker = 10 > __device__ unsigned hillclimb_base_fast(modalpha const* ctb, modalpha const* cte, F eval_fn, double iocb, FD& fd, stecker* s_base)
{
	stecker s = *s_base;
	stecker s_b;
	unsigned ctl = cte - ctb;
	auto vo = fd.decode(ctb, cte, s);
	auto iocs = index_of_coincidence(vo, ctl);
	if (iocs * .95 < iocb)
		return 0U;
	// establish the baseline
	auto scr = eval_fn(vo, vo + ctl);
	while (1)
	{
		bool improved = false;
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
				vo = fd.decode(ctb, cte, s);
				auto scrn = eval_fn(vo, vo + ctl);
				if (scrn > scr /*&& s.Count() < max_stecker + 1*/)
				{
					mx = f;
					my = t;
					scr = scrn;
					improved = true;
				}
				s = s_b;
			}
		}
		if (!improved)
			break;
		s.Apply(mx, my);
	}
	*s_base = s;
	return scr;
}

// the simple form of hillclimb, best of bg followed by best of tg
__device__ void hillclimb_bgtg_fast(modalpha const* ctb, modalpha const* cte, arena_decode_t const* ai, bigram_table const* bgt, trigram_table const* tgt, cudaJob& cj)
{
	fast_decoder_ptr fd(ai->arena_ + cj.off_ * alpha_max);
	hillclimb_base_fast(ctb, cte, bigram_score_op(bgt), 0.0, fd, &cj.s_);
	cj.scr_ = hillclimb_base_fast(ctb, cte, trigram_score_op(tgt), 0.0, fd, &cj.s_);
}

__global__ void process_hillclimb(cudaJob* jl, unsigned jls, modalpha* ct, unsigned ctl, arena_decode_t* ai, bigram_table* bgt, trigram_table* tgt)
{
	// figure out which cudaJob refers and call the actual worker fn.
	unsigned j = blockIdx.x * blockDim.x + threadIdx.x;
	if (j >= jls)
		return;
	hillclimb_bgtg_fast(ct, ct + ctl, ai, bgt, tgt, *(jl + j));
}
#if 0
// try partial exhaustion of the combinations
//
// use the fast decoder
__device__ void hillclimb_partial_exhaust2_fast(modalpha const* ctb, modalpha const* cte, arena_decode_t const* ai, trigram_table const* tgt, cudaJob& cj)
{
	const modalpha f1 = alpha::E;
	const modalpha f2 = alpha::N;
	fast_decoder_ptr fd(ai->arena_ + cj.off_ * alpha_max);
	stecker s_b = cj.s_;
	stecker s_best;
	// establish the baseline
	unsigned ctl = cte - ctb;
	auto vo = fd.decode(ctb, cte, s_b);
	auto ef = trigram_score_op(tgt);
	auto scr = ef(vo, vo + ctl);
	auto iocb = index_of_coincidence(vo, ctl);
	for (int ti1 = 0; ti1 < alpha_max; ++ti1)
	{
		modalpha t1{ ti1 };
		if (ti1 == f2)
			continue;
		for (int ti2 = 0; ti2 < alpha_max; ++ti2)
		{
			modalpha t2{ ti2 };
			if (t2 == t1 || t2 == f1)
				continue;
			s_b = cj.s_;
			s_b.Apply(f2, t2);
			s_b.Apply(f1, t1);
			auto scrn = hillclimb_base_fast(ctb, cte, ef, iocb, fd, &s_b);
			if (scrn > scr)
			{
				s_best = s_b;
				scr = scrn;
			}
		}
	}
	cj.s_ = s_best;
	cj.scr_ = scr;
}

__global__ void process_hillclimb_ex(cudaJob* jl, unsigned jls, modalpha* ct, unsigned ctl, arena_decode_t* ai, trigram_table* tgt)
{
	// figure out which cudaJob refers and call the actual worker fn.
	unsigned j = blockIdx.x * blockDim.x + threadIdx.x;
	if (j >= jls)
		return;
	hillclimb_partial_exhaust2_fast(ct, ct + ctl, ai, tgt, *(jl + j));
}
#endif
void cudaWrap::run_gpu_process()
{
	// assume (for now) 32 threads per warp and so threads per block 
	// is cj size / 32.
	unsigned tpb = (jls_ + 31) / 32;
	// start
	dim3 block(tpb);
	dim3 grid(32);
#if 0
	unsigned step = jls_ / 16;
	unsigned off = 0;
	while (off < jls_)
	{
		process_hillclimb << <grid, block >> > (jl_ + off, step, ct_, ctl_, adt_, bgt_, tgt_);
		auto err = cudaDeviceSynchronize();	if (err != cudaSuccess)
		{
			std::cout << "cudaDeviceSynchronize Error - " << err << ": " << cudaGetErrorString(err) << '\n';
			return;
		}
		off += step;
	}
#else
	process_hillclimb <<<grid, block>>> (jl_, jls_, ct_, ctl_, adt_, bgt_, tgt_);
#endif
}

void cudaWrap::run_gpu_process_ex()
{
#if 0
	// assume (for now) 32 threads per warp and so threads per block 
	// is cj size / 32.
	unsigned tpb = (jls_ + 31) / 32;
	// start
	dim3 block(tpb);
	dim3 grid(32);
	process_hillclimb_ex <<<grid, block>>> (jl_, jls_, ct_, ctl_, adt_, tgt_);
#endif
}
