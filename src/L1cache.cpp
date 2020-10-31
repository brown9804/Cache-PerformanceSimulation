/*
 *  Cache simulation project
 *  Class UCR IE-521
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
#include <debug_utilities.h>
#include <L1cache.h>

#define KB 1024
#define ADDRSIZE 32
using namespace std;

int field_size_get(struct cache_params cache_params,
                   struct cache_field_size *field_size)
{
   (*field_size).idx = log2((cache_params.size * KB) / (cache_params.block_size * cache_params.asociativity));
   (*field_size).offset = log2(cache_params.block_size);
   (*field_size).tag = ADDRSIZE - (*field_size).offset - (*field_size).idx;

   if ((*field_size).offset + (*field_size).idx + (*field_size).tag == ADDRSIZE)
   {
      return OK; 
   }
   else // err on field size 
   {
      return ERROR;
   }
}

void address_tag_idx_get(long address,
                         struct cache_field_size field_size,
                         int *idx,
                         int *tag)
{
   long indexMASK = (1 << field_size.idx) - 1;
   long tagMASK = (1 << field_size.tag) - 1;
   *idx = (address >> field_size.offset) & indexMASK;
   *tag = (address >> (field_size.offset + field_size.idx)) & tagMASK;
}
// ------------- BEGIN - BROWN, BELINDA ----------------
// @brown9804 Belinda Brown
// timna.brown@ucr.ac.cr
// November, 2020

// SRRIP Replacement Policy
// Stactic RRIP - Re-reference Interval Prediction
// M-bit Re-reference per cache block
// RRPV = how soon the block will be Rr
// ~ML by access pattern
// 1. RRPV == 0 ... Rr in near immediate future [~now]
// 2. RRPV = 2^M -1 ... RR in a distant future  [distant]
// Then, RRIP add new blocks with a long Rr interval
// from distant future to higher RRPV
// When M == 1 -> NMRU replacement policy
// Avoid corrected date by distant Rr blocks

// HP = Hit Priority (RRPV = 0)
// FP = Frequency Priority (RRPV = RRPV - 1)

// %%%%%%%%%%%%%%%%%%%%%%%%
// t=0 ... 2^2-1 = 3
// find-Victim-RRPIP(): on miss
// access-Block(): on hit  rrpv = 0 .. block(HP)
// insert-Block(): clear if valid and set rrpv as 2^2-2=2

//*** Cache hit:
// 1. RRPV of block to 0

//*** Cache miss:
// 1. Search from 2^M - 1 from left
// 2. if 2^M-1 is forund -> go to step(v)
// 3. Then increment all RRPVs
// 4. Again 1.
// 5. Replace block and put RRPV = 2^M-2
int srrip_replacement_policy (int idx,
                             int tag,
                             int associativity,
                             bool loadstore,
                             entry* cache_blocks,
                             operation_result* result,
                             bool debug)
{
   return ERROR;
}

// ------------- END - BROWN, BELINDA ----------------

int lru_replacement_policy (int idx,
                             int tag,
                             int associativity,
                             bool loadstore,
                             entry* cache_blocks,
                             operation_result* result,
                             bool debug)
{
   return ERROR;
}
int nru_replacement_policy(int idx,
                           int tag,
                           int associativity,
                           bool loadstore,
                           entry* cache_blocks,
                           operation_result* operation_result,
                           bool debug)
{
   return ERROR;
}
