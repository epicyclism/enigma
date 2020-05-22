#pragma once

#include <vector>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

#include "modalpha.h"
#include "stecker.h"
#include "gram_common.h"
#include "arena_simple.h"


// stripped back from arena source for now, consider making completely compatible
// to avoid double copy
//
struct cudaJob
{
	// index of this job in teh array (so implicitly available I think)
	unsigned off_;
	// resolved plugs
	stecker s_;
	// final score
	unsigned scr_;
	cudaJob() : off_(0), scr_(0)
	{}
	cudaJob(unsigned off, stecker const& s) : off_(off), s_(s), scr_(0)
	{}
	cudaJob(cudaJob const& other) : off_(other.off_), s_(other.s_), scr_(other.scr_)
	{}
};

struct cudaWrap
{
	// bigram table on device
	bigram_table*  bgt_;
	// trigram table on device
	trigram_table*  tgt_;

	// decode arena on device
	arena_decode_t* adt_;

	// cipher text on device
	thrust::device_vector<modalpha> ct_;

	// joblist on host
	thrust::host_vector<cudaJob> vjh_;

	// joblist on device
	thrust::device_vector<cudaJob> vjd_;

	cudaWrap() = delete;
	cudaWrap(bigram_table const& bgt, trigram_table const& tgt, std::vector<modalpha> const& ct);
	~cudaWrap();

	// test valid
	bool cudaGood() const;

	// set arena for decode
	void set_arena(arena_decode_t const& a);
	// set joblist for decode
	void sync_joblist_to_device(std::vector<cudaJob> const& jl);
	// get joblist from decode
	void sync_joblist_from_device(std::vector<cudaJob>& jl);
	// do search
	void proc();
};