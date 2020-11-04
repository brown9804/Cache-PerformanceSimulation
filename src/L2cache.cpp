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

// ****
///A second level of off-chip (L2) cache
// is added with less access time than main memory.
// ****
// STEPS:
// 1. HITS_L1 - Analyze if hits in L1:
// When a primary cache (L1) failure occurs,
// 2. Depends on load - store do same thing on l2:
// the secondary (L2) cache is accessed to search for data.
// 3. Analize the eviction - L1 & L2 .. Here?
//////////// * /////////////
// - If they are there, the failure penalty is reduced.
// - If they are not, we access the main memory.

int lru_replacement_policy_l1_l2(const entry_info *l1_l2_info,
								 bool loadstore,
								 entry *l1_cache_blocks,
								 entry *l2_cache_blocks,
								 operation_result *l1_result,
								 operation_result *l2_result,
								 bool debug)
{
///////////////////////////////////////
// Variables
///////////////////////////////////////
//// L1
int l1l2_l1_info_tag = l1_l2_info->l1_tag;
int l1l2_l1_info_idx = l1_l2_info->l1_idx;
int l1l2_l1_info_assoc = l1_l2_info->l1_assoc;
/// L2
int l1l2_l2_info_tag = l1_l2_info->l2_tag;
int l1l2_l2_info_idx = l1_l2_info->l2_idx;
int l1l2_l2_info_assoc = l1_l2_info->l2_assoc;
// GENERAL
int i = 0;
///FLAGS 
bool L1_HITS = false;

///////////////////////////////////////
// LOGIC 
///////////////////////////////////////
/////// ANALIZE --- FAILURE OCCURS...
//// DIRTY  --- EVICTION
while (i < l1l2_l1_info_assoc) {
	i = i + 1;
	l1_result->dirty_eviction = 0;
	l1_cache_blocks[i].dirty = 0;
	////  1. HITS_L1 - Analyze if hits in L1
	if (L1_HITS == false && (l1_cache_blocks[i].tag == l1l2_l1_info_tag))
	{
		L1_HITS = true;
	}
}

if (L1_HITS == false) // 3. Analize the eviction - L1 & L2 .. Here?
{
	//////// L1
	lru_replacement_policy(l1l2_l1_info_idx,
						   l1l2_l1_info_tag,
						   l1l2_l1_info_assoc,
						   loadstore,
						   l1_cache_blocks,
						   l1_result,
						   debug);
	//////// L2
	lru_replacement_policy(l1l2_l2_info_idx,
						   l1l2_l2_info_tag,
						   l1l2_l2_info_assoc,
						   loadstore,
						   l2_cache_blocks,
						   l2_result,
						   debug);
}

//// BECAUSE L1 FAILURE OCCUR ---- 
/////// L2 WORKING...
/// Is accessed to search for data
else // 2. Depends on load - store do same thing on l2:
	{
		lru_replacement_policy(l1l2_l2_info_idx,
							   l1l2_l2_info_tag,
							   l1l2_l2_info_assoc,
							   loadstore,
							   l2_cache_blocks,
							   l2_result,
							   debug);

		//////////////////
		//////// Store // 1 true
		///////////////////
		if (loadstore == 1)
		{
			l2_result->miss_hit = HIT_STORE;
		}

		//////////////////
		//////// Load // 0 false
		///////////////////
		else
		{
			l2_result->miss_hit = HIT_LOAD;
		}

}



return OK;
}
