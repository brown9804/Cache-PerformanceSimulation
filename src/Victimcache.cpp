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
#include <Victimcache.h>

// ------------- BEGIN - BROWN, BELINDA ----------------
// @brown9804 Belinda Brown
// timna.brown@ucr.ac.cr
// November, 2020

// The victim cache will be of 16 fully
// associative entries and with a FIFO
// replacement policy.

//This cache works for:
// decrease conflict errors and 
// improve hit latency for directly mapped caches.
// This fault caching occurs by placing a fully associative 
// cache between the cache and its fill path (Victim cache). 
// Cache misses that hit the VC have a penalty of one cycle, a 
// difference from a penalty for failing many cycles
// without the VC.

/// LOGIC 
// 1. Cache Hit: No action taken.
// 2. Cache Miss or Victim Hit: The block is in the victim 's 
// cache and the one in the cache replaces each other. 
// Then the new entry in the victim' s cache becomes 
// the most recently used block.
// 3. Cache Miss or Victim Miss: The block itself is moved to 
// the cache from the next level.The block that comes out 
// of the cache is stored in the victim cache.

#define KB 1024
#define ADDRSIZE 32
using namespace std;

// In utilities is vc size and other functions
int lru_replacement_policy_l1_vc(
	const entry_info *l1_vc_info,
    bool loadstore,
   	entry* l1_cache_blocks,
	entry* vc_cache_blocks,
   	operation_result* l1_result,
    operation_result* vc_result,
    bool debug)
{
	///////////////////////////////////////
	// Variables
	///////////////////////////////////////
	// LOCAL
	// l1_assoc in utilities
	int l1vc_l1_related_idx = l1_vc_info->l1_idx;
	int l1vc_l1_related_tag = l1_vc_info->l1_tag;
	int l1vc_l1_related_assoc = l1_vc_info->l1_assoc;
	//// vc_assoc in utilities
	int l1vc_vc_related_assoc = l1_vc_info->vc_assoc;
	////
	int l1_evic_addr = l1_result->evicted_address;
	bool vc_dirty_evic = vc_result->dirty_eviction;
	int vc_evic_addr = vc_result->evicted_address;
	//////
	int a = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int f = 0;
	int p =0;
	int amount_valids = 0;
	int amount_valids_active = 0;
	///// FLAGS
	bool hit_found_YorN_L1 = false;
	bool enable_VC = false;
	bool hit_found_YorN_VC = false;
	bool l1_dirty_eviction = l1_result->dirty_eviction;

	///////////////////////////////////////
	// Validate L1 space? -> ENABLE victim cache
	///////////////////////////////////////
	while (a < l1vc_l1_related_assoc)
	{
		if (l1_cache_blocks[a].valid) // if true
		{
			amount_valids_active = amount_valids_active + 1;
		}
		a = a + 1;
	}

	if (amount_valids_active == l1vc_l1_related_assoc)
	{
		enable_VC = true;
	}
	///////////////////////////////////////
	// Analyze if Cache Hit ->  L1 
	///////////////////////////////////////
	while (i < l1vc_l1_related_assoc)
	{
		if ((hit_found_YorN_L1 == false) && ((l1vc_l1_related_tag) == l1_cache_blocks[i].tag))
		{
			hit_found_YorN_L1 = true;
		}
		i = i +1;
	}
	///////////////////////////////////////
	// Analyze if Cache Hit ->  CV
	///////////////////////////////////////
	while (j < l1vc_l1_related_assoc)
	{
		if ((hit_found_YorN_VC != true) && ((l1vc_l1_related_tag) == vc_cache_blocks[j].tag))
		{
			hit_found_YorN_VC = true;
		}
		j = j + 1;
	}

	if (hit_found_YorN_L1 == true) // if hits ... L1
	{
		lru_replacement_policy(l1vc_l1_related_idx,
							   l1vc_l1_related_tag,
							   l1vc_l1_related_assoc,
							   loadstore,
							   l1_cache_blocks,
							   l1_result,
							   debug);
	}

	///////////////////////////////////////
	// Cache Miss or Victim Hit
	///////////////////////////////////////
	if ((hit_found_YorN_L1 != true) && (hit_found_YorN_VC == true))
	{
		lru_replacement_policy(l1vc_l1_related_idx,
							   l1vc_l1_related_tag,
							   l1vc_l1_related_assoc,
							   loadstore,
							   l1_cache_blocks,
							   l1_result,
							   debug
		);
		/////////////////////
		/// // The block is in the victim 's
		// // cache and the one in the cache
		/// replaces each other.
		/////////////////////
		while (k < l1vc_vc_related_assoc)
		{
			if (l1vc_l1_related_tag == vc_cache_blocks[k].tag)
			{
				vc_cache_blocks[k].tag = l1_evic_addr;
			}
			k = k + 1;
		}

		//////////////////
		//////// Store // 1 true
		///////////////////
		if (loadstore == 1)
		{
			(vc_result->miss_hit) = HIT_STORE;
		}

		//////////////////
		//////// Load // 0 false 
		///////////////////
		else
		{
			(vc_result->miss_hit) = HIT_LOAD;
		}

	}
	/////////////////////
	// Cache Miss or Victim Miss
	/////////////////////
	if ((hit_found_YorN_VC != true) && (hit_found_YorN_L1 != true))
	{

		lru_replacement_policy(l1vc_l1_related_idx,
							   l1vc_l1_related_tag,
							   l1vc_l1_related_assoc,
							   loadstore,
							   l1_cache_blocks,
							   l1_result,
							   debug);
		/////////////
		///// IF DIRTY BIT is high dirty eviction happens
		///////
		if (vc_cache_blocks[l1vc_vc_related_assoc - 1].dirty == true)
		{
			vc_dirty_evic = true;
		}
		else
		{
			vc_dirty_evic = false;
		}
		// Update the address on VC for the eviction
		vc_evic_addr = vc_cache_blocks[l1vc_vc_related_assoc - 1].tag;

		//////////////////
		//////// Store // 1 true
		///////////////////
		if (loadstore == 1)
		{
			vc_result->miss_hit = MISS_STORE;
			vc_cache_blocks[l1vc_vc_related_assoc - 1].dirty = true;
		}

		//////////////////
		//////// Load // 0 false
		///////////////////
		else
		{
			vc_result->miss_hit = MISS_LOAD;
			vc_cache_blocks[l1vc_vc_related_assoc - 1].dirty = false;
		}
		//////////////////
		//////// To replace VC's blocks
		if (enable_VC == true)
		{

		//// The structure is a FIFO but the VC works 
		//// associated with an L1 where the cache has 
		//// a replacement policy lru.It encompasses 
		//// the L1 system plus the VC
			int mem = vc_cache_blocks[0].tag;
			bool mem_valid= vc_cache_blocks[0].valid;
			bool mem_dirty = vc_cache_blocks[0].dirty;
			while (f < l1vc_vc_related_assoc - 1)
			{ // Needed flags
				vc_cache_blocks[f + 1].valid = mem_valid;
				vc_cache_blocks[f + 1].tag = mem;
				vc_cache_blocks[f + 1].dirty = mem_dirty;
				mem = vc_cache_blocks[f + 1].tag;
				mem_valid = vc_cache_blocks[f + 1].valid;
				mem_dirty = vc_cache_blocks[f + 1].dirty;
				f = f + 1;
			}
			/////////////////////////////////////////
			/// Asssing values -> victim cache blocks
			/////////////////////////////////////////
			vc_cache_blocks[0].valid = mem_valid;
			vc_cache_blocks[0].dirty = mem_dirty;
			vc_cache_blocks[0].tag = mem;
			vc_cache_blocks[0].tag = l1_evic_addr;
			/////////////////////////////
			//////// UPDATE INFORMATION ,,,,
			////////  DIRTY BIT & DIRTY BIT
			//////// THEN TAGS
			////////////////////////////////
			while (p < l1vc_l1_related_assoc)
			{
				if (l1_cache_blocks[p].valid == true)
				{
					amount_valids = amount_valids + 1;
				}
				p = p + 1;
			}

			////////////////////
			////// AMOUNT OF VALID
			////////////////////
			if (amount_valids != l1vc_l1_related_assoc)
			{
				vc_cache_blocks[0].valid = false;
			}

			else // if equals valids to assoc
			{
				vc_cache_blocks[0].valid = true;
			}

			////////////////
			///// EVICTION
			///////////////
			if (l1_dirty_eviction == false)
			{
				vc_cache_blocks[0].dirty = false;
			}
			else
			{
				vc_cache_blocks[0].dirty = true;
			}

		}
	}

	return OK;
}
//// REFERENCES
// [1] // The Arrow(->) operator exists to access the members of the structure
// or the unions using pointers.
//https: //www.geeksforgeeks.org/arrow-operator-in-c-c-with-examples/
// [2] // DIAGRAM - construction
// https://www2.seas.gwu.edu/~mlancast/cs211al/reference/D-VictimCache.pdf
// [3] // Bool - works 
// http://www.functionx.com/cpp/keywords/bool.htm
// [4]
// ------------- END - BROWN, BELINDA ----------------
