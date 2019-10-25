
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}


uint32_t probe_rdseed(uint32_t n)
{
    uint32_t contention;

    if(n == 0) return 0;

    asm volatile(
    "loop%=: \n\t"
    "rdseed %%edx\n\t"
    "jnc contention%=\n\t"
    "decl %%eax\n\t"
    "jne loop%=\n\t"
    "movl $0, %1\n\t"
    "jmp done%=\n\t"
    "contention%=:\n\t"
    "movl $1, %1\n\t"
    "done%=:\n\t"
    : "=r"(contention) 
    : "a"(n) 
    : "cc", "edx"); 

    return contention;
}

void do_n_rdseed(uint32_t n)
{

    asm volatile(
    "loop%=: \n\t"
    "lfence\n\t"
    "rdseed %%edx\n\t"
    "lfence\n\t"
    "loop loop%=\n\t"
    : 
    : "c"(n) 
    : "edx"); 

}

CYCLES measure_prime_time()
{
    CYCLES cycles;

    asm volatile(
    "lfence\n\t"
    "rdtsc\n\t"
    "mov %%eax, %%edi\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "rdseed %%edx\n\t"
    "lfence\n\t"
    "rdtsc\n\t"
    "sub %%edi, %%eax\n\t"
    : "=a"(cycles) 
    : 
    : "edx", "edi"); 

    return cycles;
}


void nops(uint32_t n)
{
    asm volatile(
    "loop%=: \n\t"
    "lfence\n\t"
    "loop loop%=\n\t"
    : 
    : "c"(n) 
    : ); 
}

CYCLES nops_time(uint32_t n)
{
    CYCLES cycles;

    asm volatile(
    "lfence\n\t"
    "rdtsc\n\t"
    "mov %%eax, %%edi\n\t" 
    "loop%=: \n\t"
    "lfence\n\t"
    "nop\n\t"
    "lfence\n\t"
    "loop loop%=\n\t"
    "lfence\n\t"
    "rdtsc\n\t"
    "sub %%edi, %%eax\n\t"
    : "=a"(cycles)
    : "c"(n) 
    : "ebx"); 

    return cycles;
}

CYCLES rdtsc_time()
{
    CYCLES cycles;

    asm volatile(
    "lfence\n\t"
    "rdtsc\n\t"
    "mov %%eax, %%edi\n\t" 
    "lfence\n\t"
    "rdtsc\n\t"
    "sub %%edi, %%eax\n\t"
    : "=a"(cycles)
    : 
    : ); 

    return cycles;
}

CYCLES n_rdseed_time(uint32_t n)
{
    CYCLES cycles;
    asm volatile(
    "lfence\n\t"
    "rdtsc\n\t"
    "mov %%eax, %%edi\n\t"
    "loop%=: \n\t"
    "lfence\n\t"
    "rdseed %%edx\n\t"
    "lfence\n\t"
    "loop loop%=\n\t"
    "lfence\n\t"
    "rdtsc\n\t"
    "sub %%edi, %%eax\n\t"
    : "=a"(cycles)
    : "c"(n) 
    : "edx"); 
    
    return cycles;
}

double cac_failure_rate()
{
    int c;
	double cnt=0;
	for(int i=0;i<1000;i++)
	{
		c = probe_rdseed(8);
        if(c==1) cnt++;
	}

    return cnt/1000;
}


