#include <iostream>
#include <array>

#include <cuda.h>

#include "hillclimb_cuda.h"
#include <thrust/copy.h>
#include <thrust/for_each.h>

cudaWrap::cudaWrap(trigram_table const& tgt, std::vector<modalpha> const& ct)
{
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

	// copy in the trigram table of the day
	cudaMalloc(&tgt_, sizeof(trigram_table));
	cudaMemcpy(tgt_, &tgt, sizeof(trigram_table), cudaMemcpyHostToDevice);

	// reserve space for the arena
	cudaMalloc(&adt_, sizeof(arena_decode_t));

	// copy in the ciphertext
	thrust::host_vector<modalpha> cth(ct.size());
	thrust::copy(ct.begin(), ct.end(), cth.begin());
	ct_ = cth;
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

#if 0
trigram_table*  tgt_;
arena_decode_t* arena_;

bool init_cuda(trigram_table const& tgt)
{
	tgt_ = nullptr;
	arena_ = nullptr;
	int deviceCount = 0;
	cudaError_t err_id = cudaGetDeviceCount(&deviceCount);
	if (err_id != cudaSuccess)
	{
		std::cout << "cudaGetDeviceCount returned " << err_id << "(" << cudaGetErrorString(err_id) << '\n';
		return false;
	}
	if (deviceCount < 1)
	{
		std::cout << "No Cuda devices found\n";
		return false;
	}
	std::cout << "CUDA deviceCount = " << deviceCount << '\n';

	// copy in the trigram table of the day
	cudaMalloc(&tgt_, sizeof(trigram_table));
	cudaMemcpy(tgt_, &tgt, sizeof(trigram_table), cudaMemcpyHostToDevice);

	// reserve space for the arena
	cudaMalloc(&arena_, sizeof(arena_decode_t));

	return false;
}

void set_arena(arena_decode_t const& arena)
{
	// copy in the arena for this wheelorder/ring setting
	cudaMemcpy(arena_, &arena, sizeof(arena), cudaMemcpyHostToDevice);
}

void uninit_cuda()
{
	cudaFree(arena_);
	cudaFree(tgt_);
	cudaDeviceReset();
}
#endif