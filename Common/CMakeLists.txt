cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (Common STATIC "wheelset.h" "stecker.h" "machine.h" "machine.cpp"  "hillclimb.h" "unigram.h" "fast_decoder.h" "utility.h"   "arena_simple.h")
