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
	int l1_l1_related_idx = l1_vc_info->l1_idx;
	int l1_l1_related_tag = l1_vc_info->l1_tag;
	int l1_l1_related_assoc = l1_vc_info->l1_assoc;
	//// vc_assoc in utilities
	int l1_vc_related_assoc = l1_vc_info->vc_assoc;
	////
	int l1_evic_addr = l1_result->evicted_address;
	bool vc_dirty_evic vc_result->dirty_eviction;
	int vc_evic_addr = vc_result->evicted_address;
	//////
	int amount_valids = 0;
	///// FLAGS
	bool hit_found_YorN_L1 = false;
	bool enable_VC = false;
	bool hit_found_YorN_VC = false;
	bool l1_dirty_eviction = l1_result->dirty_eviction;

	///////////////////////////////////////
	// Validate L1 space? -> ENABLE victim cache
	///////////////////////////////////////
	for (int i = 0; i < (l1_l1_related_assoc); i= i + 1)
	{
		if (l1_cache_blocks[i].valid) // if true 
		{
			count_valids_active = count_valids_active + 1;
		}
	}
	if (count_valids_active == l1_l1_related_assoc)
	{
		enable_VC = true;
	}
	///////////////////////////////////////
	// Analyze if Cache Hit ->  L1 
	///////////////////////////////////////
	for (int i = 0; i < (l1_l1_related_assoc); i = i + 1)
	{
		if (((l1_l1_related_tag) == l1_cache_blocks[i].tag) && (hit_found_YorN_L1 == false))
		{
			hit_found_YorN_L1 = true;
		}
	}

	if (hit_found_YorN_L1 == true) // if hits ...
	{
		lru_replacement_policy(l1_l1_related_idx,
							   l1_l1_related_tag,
							   l1_l1_related_assoc,
							   loadstore,
							   l1_cache_blocks,
							   l1_result,
							   debug);
	}

	///////////////////////////////////////
	// Analyze if Cache Hit ->  CV
	///////////////////////////////////////
	for (int i = 0; i < (l1_vc_related_assoc); i = i + 1)
	{
		if (((l1_l1_related_tag) == vc_cache_blocks[i].tag) && (hit_found_YorN_VC == false))
		{
			hit_found_YorN_VC = true;
		}
	}

	///////////////////////////////////////
	// Cache Miss or Victim Hit
	///////////////////////////////////////
	if ((hit_found_YorN_L1 == false) && (hit_found_YorN_VC == true))
	{
		lru_replacement_policy(l1_l1_related_idx,
							   l1_l1_related_tag,
							   l1_l1_related_assoc,
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
		for (int i = 0; i < (l1_vc_related_assoc); i = i + 1)
		{
			if (l1_l1_related_tag == vc_cache_blocks[i].tag)
			{
				vc_cache_blocks[i].tag = (l1_evic_addr);
			}
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
	if ((hit_found_YorN_L1 == false) && (hit_found_YorN_VC == false))
	{

		lru_replacement_policy(l1_l1_related_idx,
							   l1_l1_related_tag,
							   l1_l1_related_assoc,
							   loadstore,
							   l1_cache_blocks,
							   l1_result,
							   debug);
	/////////////
	///// IF DIRTY BIT is high dirty eviction happens
	///////
		if (vc_cache_blocks[l1_vc_related_assoc - 1].dirty == true)
		{
			(vc_dirty_evic) = true;
		}
		else
		{
			(vc_dirty_evic) = false;
		}
		// Update the address on VC for the eviction
		vc_evic_addr = vc_cache_blocks[l1_vc_related_assoc - 1].tag;

		//////////////////
		//////// Store // 1 true
		///////////////////
		if (loadstore == 1)
		{
			vc_cache_blocks[l1_vc_related_assoc - 1].dirty = true;
			vc_result->miss_hit = MISS_STORE;
		}

		//////////////////
		//////// Load // 0 false
		///////////////////
		else
		{
			vc_cache_blocks[l1_vc_related_assoc - 1].dirty = false;
			vc_result->miss_hit = MISS_LOAD;
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
			for (int i = 0; i < l1_vc_related_assoc - 1; i = i + 1)
			{
				bool aux_valid = vc_cache_blocks[i + 1].valid;
				int aux = vc_cache_blocks[i + 1].tag;
				bool aux_dirty = vc_cache_blocks[i + 1].dirty;
				vc_cache_blocks[i + 1].valid = temp_valid;
				vc_cache_blocks[i + 1].tag = mem;
				vc_cache_blocks[i + 1].dirty = mem_dirty;
				mem = aux;
				mem_valid= aux_valid;
				mem_dirty = aux_dirty;
			}

			vc_cache_blocks[0].valid = temp_valid;
			vc_cache_blocks[0].tag = l1_evic_addr;
			vc_cache_blocks[0].dirty = mem_dirty;
/////////////////////////////
//////// UPDATE INFORMATION ,,,,
//////// VALID AND DIRTY BIT 
//////// THEN TAGS
////////////////////////////////			
			for (int i = 0; i < l1_l1_related_assoc; i = i + 1)
			{
				if (l1_cache_blocks[i].valid == true)
				{
					amount_valids = amount_valids + 1;
				}
			}
			////////////////////
			////// AMOUNT OF VALID
			////////////////////
			if (amount_valids == l1_l1_related_assoc)
			{
				vc_cache_blocks[0].valid = true;
			}
			else
			{
				vc_cache_blocks[0].valid = false;
			}
			////////////////
			///// EVICTION
			///////////////
			if (l1_dirty_eviction)
			{
				vc_cache_blocks[0].dirty = true;
			}
			else
			{
				vc_cache_blocks[0].dirty = false;
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
