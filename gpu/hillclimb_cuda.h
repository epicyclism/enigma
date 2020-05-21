#pragma once

#include <vector>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

#include "modalpha.h"
#include "gram_common.h"
#include "arena_simple.h"

#if 0
bool init_cuda(trigram_table const& tgt);
void set_arena(arena_decode_t const& arena);
void uninit_cuda();
#endif

struct cudaWrap
{
	// trigram table on device
	trigram_table*  tgt_;

	// decode arena on device
	arena_decode_t* adt_;

	// cipher text on device
	thrust::device_vector<modalpha> ct_;

	// joblist on host
	
	// joblist on device

	cudaWrap() = delete;
	cudaWrap(trigram_table const& tgt, std::vector<modalpha> const& ct);
	~cudaWrap();

	// test valid
	bool cudaGood() const;

	// set arena for decode
	void set_arena(arena_decode_t const& a);

	// set joblist for decode

	// get joblist from decode
};