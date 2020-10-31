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

// function.
// Checking the parameters status, on boundary etc
int params_check(int idx, int tag, int associativity){
  int ok_param = OK;
  int assocPower2 = associativity;
  if (idx >= 0 && tag >= 0 && associativity > 0) {
    //we need to checks if associativity is a power of 2, based on the spacial principle
    while(assocPower2%2==0 && assocPower2 > 1){ // across all values power of two.
      assocPower2 /= 2;
    }
    if (associativity%2!=0){
      ok_param = PARAM;
    }
    if (assocPower2==1){
      ok_param = OK;
    }else {
      ok_param = PARAM;
    }
  } else {
    ok_param = PARAM;
  }
  return ok_param;
}
////////////////////////////////////////

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
  /*LRU Policy:
The LRU algorithm (less recently used for its acronym in English), tries to reduce the possibility
of discarding blocks that are going to be used again in the near future. For this, the information
regarding the use of the blocks is stored so that in a replacement the block that has not been used
for a longer time is discarded. LRU is based on the principle of temporary locality, according to which
if a block has been used recently it is likely to be used again in the near future, hence the best
candidate to replace is the block that has not been used for more. weather. The implementation of this
algorithm requires keeping a record of the use of each block so that updating the use of one block
implies updating that of all the others, hence this implementation tends to be very expensive for
highly associativity caches. This algorithm works as a queue-like data structure, hence it is referred
to as the LRU queue. The beginning of the queue (head), and therefore the candidate to leave it first,
is the block that has been referenced least recently (LRU block). Likewise, the end of the queue (queue)
and therefore the candidate to stay longer in it, is the block that has been most recently referenced
(MRU block, from the most recently used English). Since this algorithm is based on the principle of
temporal locality, it performs well against loads with high temporal locality in the data, but in
the case of loads where new references occur in the distant future, LRU performs poorly. As an example of the latter
type of loads there is the case in which the working set is greater than the capacity of the cache or
when a group of data without any type of reuse supposes the replacement of the data of the working set.*/
int lru_replacement_policy (int idx,
                             int tag,
                             int associativity,
                             bool loadstore,
                             entry* cache_blocks,
                             operation_result* result,
                             bool debug)
{
  // Control Flags
  bool cache_full = true;          // for eviction
  bool Hit_flag = false;           // one flag 1 for hit, 0 for miss
  
  // Ints
  int MRU_value = associativity - 1;      // Most recently used value
  int LRU_value = 0;                      // Lest recently used value.
  int Victim_value = -1;                        // Victim  value
  int hit_way = -1;                       // number of the hit way analyzed
  int free_way = -1;                       // number of the free way analyzed
  
  // getting parameters checked
  int param_check = params_check(idx, tag, associativity);      // returns Ok or PARAM
  if (param_check!=OK){
    return PARAM;
  }
  // checking all the ways
  for (int way = 0; way < associativity; way++){

    // LETS CHECK IF Hit
    if (cache_blocks[way].valid && cache_blocks[way].tag == tag){     // There will only be hit if the block in the current way is valid and the tag is the one indicated
      Hit_flag = true;
      hit_way = way;
    }
    //Space check, tracking of free ways and full cache
    if (cache_blocks[way].valid == false) {
      cache_full = false;
      free_way = way;
    }
  }
  // check hit status
  if (Hit_flag){
    //if was a Store
    if (loadstore){
      result->miss_hit = HIT_STORE;  
    //if was a Load
    }else{
      result->miss_hit = HIT_LOAD;
    }
    result->evicted_address = 0;      // not an address
    result->dirty_eviction = false; // not evicted

    // checking the replacement policy value
    for (int i = 0; i < associativity; i++){                // across all ways
      if (cache_blocks[i].rp_value>cache_blocks[hit_way].rp_value){   // limits of desicion
        if (cache_blocks[i].rp_value!=LRU_value){             
          cache_blocks[i].rp_value = cache_blocks[i].rp_value - 1;  // LRU value substraccion, policy.
        }
      }
    }
    cache_blocks[hit_way].rp_value = MRU_value; // now if the hit gets the maximun replacement value
    if(loadstore){     // Under a operation whit hit
      cache_blocks[hit_way].dirty = true;
    }
  }else{ // if we have a Miss
    if (loadstore){    // operation Store
      result->miss_hit = MISS_STORE;   // a store with a miss

    }else{     // If we have a Load
      result->miss_hit = MISS_LOAD;     // a Load with a miss
    }
    if (cache_full == false){     // now case of Free Space, if is not full
      result->dirty_eviction = false;     // not necessary an eviction
      result->evicted_address = 0;        // setting address to zero
      // checking all ways.
      for (int i = 0; i < associativity; i++){
        if (cache_blocks[i].valid){       // the block is valid
          if (cache_blocks[i].rp_value != LRU_value){    // the same logic, to update de Replacement policy value
            cache_blocks[i].rp_value = cache_blocks[i].rp_value - 1;  // substraction on the RP value
            }
        }
      }
      if (loadstore){    // lets check the dirty bit
        cache_blocks[free_way].dirty = true;   // if it was a operation over the block, set the dirty bit
      }else{
        cache_blocks[free_way].dirty = false;     // clean by default
      }
      cache_blocks[free_way].rp_value = MRU_value;  // now if the hit gets the maximun replacement value, the most recently used value.
      cache_blocks[free_way].tag = tag;             // update tag
      cache_blocks[free_way].valid = true;          // update valid
    }else{          // Now we have the last case: if cache is full
      // again checking all ways
      for (int i = 0; i < associativity; i++){
        if (cache_blocks[i].rp_value == LRU_value){   // checking for the victim 
          Victim_value = i;     //now set the Victim value
        }else{
          cache_blocks[i].rp_value = cache_blocks[i].rp_value -1;  // substraction on the RP value
        }
      }
      result->evicted_address = cache_blocks[Victim_value].tag;       // saving to result the address of the victim block with tag
      if (cache_blocks[Victim_value].dirty){
        result->dirty_eviction = true;        // update dirty eviction 
      }else{
        result->dirty_eviction = false;
      }
  
      if (loadstore){     //Now is necessary to check the dirty bits of the victims in order to know if store/load is needed
        cache_blocks[Victim_value].dirty = true;
      }else{
        cache_blocks[Victim_value].dirty = false;
      }
      cache_blocks[Victim_value].rp_value = MRU_value;    // again back and gets the maximun replacement value, the most recently used value.
      cache_blocks[Victim_value].valid = true;
      cache_blocks[Victim_value].tag = tag;


    }
  }
  return OK;        // param type return 
}
  
  
  // ------------- END - ESQUIVEL, BRANDON ----------------
  
int nru_replacement_policy(int idx,
                           int tag,
                           int associativity,
                           bool loadstore,
                           entry* cache_blocks,
                           operation_result* operation_result,
                           bool debug)
{for(int i=0; i < associativity; i++)
	{
		if(cache_blocks[idx*associativity+i].tag == tag) /*HIT*/
		{
			if(loadstore == 0) /*Load*/
			{
				H_L = H_L+1;
			}
			else /*Store*/
			{
				H_S = H_S+1;
			}
			cache_blocks[idx*associativity+i].rp_value = 0;
			cache_blocks[idx*associativity+i].dirty = 0; /* No write */
		}
		else
		{
			if(i == associativity-1) /*Set is done?*/ /*MISS*/
			{
				if(loadstore == 0) /*Load*/
				{
					M_L = M_L+1;
				}
				else /*Store*/
				{
					M_S = M_S+1;
				}

				/*Victim search*/
				bool rep_ok = false;
				while(rep_ok==false)
				{
					for(int j=0; j < associativity; j++)
					{
						if(cache_blocks[idx*associativity+j].rp_value == 1) /*Victim found*/
						{
							if(cache_blocks[idx*associativity+j].dirty = 1)
							{
								operation_result[idx*associativity+j].dirty_eviction = 1;
								operation_result[idx*associativity+j].evicted_address = 1;
							}

							/*Replacement*/	
							cache_blocks[idx*associativity+j].rp_value = 0;
							cache_blocks[idx*associativity+j].tag = tag;
							cache_blocks[idx*associativity+j].valid = 1;
							cache_blocks[idx*associativity+j].dirty = 1;

							rep_ok = true;	
						}
						else
						{
							if(j == associativity-1) /*Set is done?*/ /*There's no 1 found*/
							{
								for(int k=0; k < associativity; k++)
								{
									cache_blocks[idx*associativity+k].rp_value = 1;
								}
							}
						}
					}
				}	
			}
		}
	}
   return ERROR;
}
