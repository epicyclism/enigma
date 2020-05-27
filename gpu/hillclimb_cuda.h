#pragma once

#include <vector>

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
	cudaJob() : off_(0), scr_(256)
	{}
	cudaJob(unsigned off, stecker const& s) : off_(off), s_(s), scr_(512)
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
	modalpha*       ct_;
	unsigned        ctl_;

	// joblist on device
	cudaJob*        jl_;
	unsigned        jls_;

	cudaWrap() = delete;
	// these arguments are constant throughout the search
	cudaWrap(bigram_table const& bgt, trigram_table const& tgt, std::vector<modalpha> const& ct);
	~cudaWrap();
	// test valid
	bool cudaGood() const;

	// the arena changes for each wheel order and ring setting
	void set_arena(arena_decode_t const& a);
	// set joblist for decode
	void sync_joblist_to_device(std::vector<cudaJob> const& jl);
	// get joblist from decode
	void sync_joblist_from_device(std::vector<cudaJob>& jl);
	// do search, blocks till completion
	void run_gpu_process();
	void run_gpu_process_ex();
};