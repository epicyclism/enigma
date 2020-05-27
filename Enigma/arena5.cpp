// arena5.cpp : final cut
//
#include <iostream>
#include <iomanip>
#if !defined (_MSC_VER)
#if defined ( __GNUC__ ) && ( __GNUC__ > 8)
#include <execution>
#else
#define SEQ_PROC
#pragma message("Note parallel STL not supported, sequential execution on the CPU may be intolerable.")
#endif
#endif
#include <algorithm>
#include <numeric>
#include <chrono>
#include "wheelset.h"
#include "machine.h"
#include "position.h"
#include "arena.h"
#include "jobs.h"
#include "utility.h"
#include "hillclimb.h"
#include "hillclimb_cuda.h"

//#include "trigram.h"

constexpr  char version[] = "v0.02";

constexpr unsigned tg_threshold = 16000; // trigram

void Help()
{
	std::cerr << "arena5 " << version << " : Enigma settings hunt, CUDA testbed.\n\n";
	std::cerr << "For example,\n\n";
	std::cerr << "./arena5 BC 12345 [b [e]]\n";
	std::cerr << "Reads ciphertext from stdin then searches all wheel orders formed from reflectors BC and\n";
	std::cerr << "wheels 12345 and attempts to find wheels, ring and plug settings that produce a decrypt.\n";
	std::cerr << "The optional trailing 'b' and 'e' values determine where in the set of wheel orders to\n";
	std::cerr << "start, and optionally, to end so that sessions can be interrupted, restarted or shared across systems.\n\n";
}

struct job_position
{
	machine_settings_t mst_;
	unsigned off_;
	unsigned scr_;

	job_position(machine_settings_t const& mst, unsigned off) : mst_(mst), off_(off), scr_(512)
	{
	}
};

template<typename J, typename A> auto make_job_list_position(machine_settings_t mst, A& a, size_t ctl) -> std::vector<J>
{
	std::vector<J> vjb;

	bool bend = false;
	auto w3 = MakeWheels3(mst);
	unsigned off = 0;
	auto ap = a.arena_.begin();
	do
	{
		mst.pos_ = w3.Position();
		vjb.emplace_back(mst, off);
		w3.Step();
		auto col = w3.Evaluate(alphabet);
		std::copy(col.begin(), col.end(), (*ap).begin());
		++ap;
		++off;
		if (w3.Position() == position())
			bend = true;
	} while (!bend || --ctl);
	return vjb;
}

// a results type for the hillclimb process
//
struct result_out_t
{
	machine_settings_t mst_;
	unsigned scr_;

	explicit result_out_t(machine_settings_t const& mst, unsigned scr) : mst_(mst), scr_(scr)
	{
	}
};

template<typename R> unsigned collate_results_tg(R const& in, std::vector<result_out_t>& out)
{
	unsigned tg_max = 0;
	for (auto& r : in)
	{
		if (r.scr_ > tg_max)
			tg_max = r.scr_;
		if (r.scr_ > tg_threshold)
			out.emplace_back(r.mst_, r.scr_);
	}
	return tg_max;
}

template<typename CI, typename SCR> void report_result(machine_settings_t const& mst, SCR scr, CI ctb, CI cte)
{
	machine3 m3 = MakeMachine3(mst);
	std::vector<modalpha> vo;
	vo.reserve(std::distance(ctb, cte));
	decode(ctb, cte, m3, vo);
	// report
	std::cout << mst << " { " << scr << " } : ";
	report_ciphertext(vo, std::cout);
}

std::vector<cudaJob> to_vjc(std::vector<job_position> const& jl)
{
	std::vector<cudaJob> vjc;
	vjc.resize(jl.size());
	std::transform(jl.begin(), jl.end(), vjc.begin(), [](auto const& jp) { return cudaJob(jp.off_, jp.mst_.stecker_); });
	return vjc;
}

void update_vjb(std::vector<job_position>& vjb, std::vector<cudaJob> const& vjc)
{
	auto ito = vjb.begin();
	std::for_each(vjc.begin(), vjc.end(), [&ito](auto const& cj) { (*ito).mst_.stecker_ = cj.s_; (*ito).scr_ = cj.scr_; ++ito; });
}

// an arena for bulk fast decode, shouldn't be slower than calulating per position, should be quicker because each
// position only calculated once!
//
arena_decode_t arena_;

int main(int ac, char** av)
{
	if (ac < 3)
	{
		Help();
		return 0;
	}
	int jobbegin = 0;
	int jobend = -1;
	if (ac > 3)
		jobbegin = ::atoi(av[3]);
	if (ac > 4)
		jobend = ::atoi(av[4]);
	if (jobend != -1 && jobend <= jobbegin)
	{
		std::cout << "Nonsensical values for job begin and/or end\n";
		return -1;
	}
	// this is where we collect the overall results!
	std::vector<result_out_t> vr_oall;
	try
	{
		std::cout << "arena5 version " << version << '\n';
		std::cout << "\nReady to read ciphertext\n";
		// capture the ciphertext
		auto ct = read_ciphertext();
		std::cout << "Ciphertext is - ";
		report_ciphertext(ct, std::cout);
		std::cout << "\nInitialising search\n";

		cudaWrap cw(bg_gen, tg_gen, ct);
		// check GPU
		if (!cw.cudaGood())
		{
			std::cout << "No go with check_cuda!\nCuda is required for this implementation.\n";
			return -1;
		}

		std::vector<job_wheels> vjbw = make_job_list<job_wheels>(av[1], av[2], jobbegin, jobend);
		if (vjbw.empty())
		{
			std::cout << "No matching wheel and reflector arrangements found!\n";
			return -1;
		}
		std::cout << "Searching " << vjbw.size() << " wheel and reflector arrangements.\n";
		auto start = std::chrono::steady_clock::now();

		// work through the wheel orders linearly
		for (auto& j : vjbw)
		{
			auto start_wo = std::chrono::steady_clock::now();

			// search each wheel order in parallel
			do
			{
				std::cout << j.mst_ << " " << vr_oall.size();
				// also fills the arena for this wheel order
				auto vjb = make_job_list_position<job_position>(j.mst_, arena_, ct.size());
				auto vjc = to_vjc(vjb);
				// wait for cuda good, this will overlap the job list creation on the CPU with the processing on the GPU
				if (!cw.cudaGood())
					throw std::exception("Some cuda failure in loop, see console above.");
				// punt to CUDA
				cw.set_arena(arena_);
				cw.sync_joblist_to_device(vjc);

				std::cout << " - considering " << vjb.size() << " possibles.";
#if 1
				// GPU!
				cw.run_gpu_process();
#else
				// CPU
				auto pa = arena_.arena_.begin();
#if defined (SEQ_PROC)
				std::for_each(std::begin(vjb), std::end(vjb), [&pa, &ct](auto& aj)
#else
				std::for_each(std::execution::par, std::begin(vjb), std::end(vjb), [&pa, &ct](auto& aj)
#endif
					{
						aj.scr_ = hillclimb_bgtg_fast(std::begin(ct), std::end(ct), pa + aj.off_, aj.mst_);
					});
#endif
				cw.sync_joblist_from_device(vjc);
				update_vjb(vjb, vjc);

				auto n = vr_oall.size();
				auto mx = collate_results_tg(vjb, vr_oall);
				std::cout << " Max tg score = " << mx << ".\n";
				std::for_each(vr_oall.begin() + n, vr_oall.end(), [&ct](auto& r)
					{
						report_result(r.mst_, r.scr_, std::begin(ct), std::end(ct));
					});
			} while (AdvanceRing(j.mst_));
			auto now = std::chrono::steady_clock::now();
			std::cout << "Wheel order search time: " << std::chrono::duration<double>(now - start_wo).count() << "s\n";
		}
		auto now = std::chrono::steady_clock::now();
		std::cout << "Finished, search time: " << std::chrono::duration<double>(now - start).count() << "s\n";
		// sort in best->worst!
		std::sort(std::begin(vr_oall), std::end(vr_oall), [](auto const& l, auto const& r) { return l.scr_ == r.scr_; });
		// report
		std::for_each(vr_oall.begin(), vr_oall.end(), [&ct](auto& r)
			{
				report_result(r.mst_, r.scr_, std::begin(ct), std::end(ct));
			});
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error configuring machine. >" << ex.what() << "<\n\n";
		return -1;
	}

	return 0;
}
