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
#include <L2cache.h>

#define KB 1024
#define ADDRSIZE 32
using namespace std;

//// ***************************************
///////// MULTILEVEL CACHE 
//// ***************************************
// Two cache levels are simulated :
//  1. Level(L1).
//  2. Level(L2).
//////
///Considerations: 
// *.For L2 : 
// - The size of the second level cache should 
// be four times the size of L1.
// - Associativity will be defined as 
// twice that of the first level.

// *.Levels L1 and L2 use:
// - An LRU replacement policy.
// - They are inclusive.
// - write-throuth policy.

// *.Between L2 and memory it is used:
// - writeback policy.

int lru_replacement_policy_l1_l2(const entry_info *l1_l2_info,
								 bool loadstore,
								 entry *l1_cache_blocks,
								 entry *l2_cache_blocks,
								 operation_result *l1_result,
								 operation_result *l2_result,
								 bool debug)
{
	return OK;
}
