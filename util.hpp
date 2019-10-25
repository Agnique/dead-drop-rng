// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

void clflush(ADDR_PTR addr);

uint32_t probe_rdseed(uint32_t n);
CYCLES measure_prime_time();
void prime();
void nops(uint32_t n);
CYCLES nops_time(uint32_t n);
double cac_failure_rate();
CYCLES rdtsc_time();
void do_n_rdseed(uint32_t n);
CYCLES n_rdseed_time(uint32_t n);
#endif
