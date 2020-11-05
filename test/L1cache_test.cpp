/*
 *  Cache simulation project
 *  Class UCR IE-521
 *  Semester: I-2019
*/

#include <gtest/gtest.h>
#include <time.h>
#include <stdlib.h>
#include <debug_utilities.h>
#include <L1cache.h>

class L1cache : public ::testing::Test{
    protected:
	int debug_on;
	virtual void SetUp()
	{
           /* Parse for debug env variable */
	   get_env_var("TEST_DEBUG", &debug_on);
	};
};

/*
 * TEST1: Verifies miss and hit scenarios for srrip policy
 * 1. Choose a random associativity
 * 2. Fill a cache entry
 * 3. Force a miss load
 * 4. Check  miss_hit_status == MISS_LOAD
 * 5. Force a miss store
 * 6. Check miss_hit_status == MISS_STORE
 * 7. Force a hit read
 * 8. Check miss_hit_status == HIT_READ
 * 9. Force a hit store
 * 10. miss_hit_status == HIT_STORE
 */
TEST_F(L1cache, hit_miss_srrip){
  int status;
  int i;
  int idx;
  int tag;
  int associativity;
  enum miss_hit_status expected_miss_hit;
  bool loadstore = 1;
  bool debug = 0;
  operation_result result = {};

  /* Fill a random cache entry */
  idx = rand()%1024;
  tag = rand()%4096;
  associativity = 1 << (rand()%4);
  if (debug_on) {
    printf("Entry Info\n idx: %d\n tag: %d\n associativity: %d\n",
          idx,
          tag,
          associativity);
  }
 
  entry cache_line[associativity];
  /* Check for a miss */
  DEBUG(debug_on,Checking miss operation);
  for (i = 0 ; i < 2; i++){
    /* Fill cache line */
    for ( i =  0; i < associativity; i++) {
      cache_line[i].valid = true;
      cache_line[i].tag = rand()%4096;
      cache_line[i].dirty = 0;
      cache_line[i].rp_value = (associativity <= 2)? rand()%associativity: 3;
      while (cache_line[i].tag == tag) {
        cache_line[i].tag = rand()%4096;
      }
    }
    /* Load operation for i = 0, store for i =1 */
    loadstore = (bool)i;
    status = srrip_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     bool(debug_on));
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? MISS_STORE: MISS_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }
  /*
   * Check for hit: block was replaced in last iteration, if we used the same 
   * tag now we will get a hit
   */
  DEBUG(debug_on,hecking hit operation);
  for (i = 0 ; i < 2; i++){
    loadstore = (bool)i;
    status = srrip_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     (bool)debug_on);
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? HIT_STORE: HIT_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }

}

/*
 * TEST2: Verifies miss and hit scenarios for lru policy
 * 1. Choose a random associativity
 * 2. Fill a cache entry
 * 3. Force a miss load
 * 4. Check  miss_hit_status == MISS_LOAD
 * 5. Force a miss store
 * 6. Check miss_hit_status == MISS_STORE
 * 7. Force a hit read
 * 8. Check miss_hit_status == HIT_READ
 * 9. Force a hit store
 * 10. miss_hit_status == HIT_STORE
 */
TEST_F(L1cache, hit_miss_lru) {
  int status;
  int i;
  int idx;
  int tag;
  int associativity;
  enum miss_hit_status expected_miss_hit;
  bool loadstore = 1;
  bool debug = 0;
  operation_result result = {};

  /* Fill a random cache entry */
  idx = rand()%1024;
  tag = rand()%4096;
  associativity = 1 << (rand()%4);
  if (debug_on) {
    printf("Entry Info\n idx: %d\n tag: %d\n associativity: %d\n",
          idx,
          tag,
          associativity);
  }
 
  entry cache_line[associativity];
  /* Check for a miss */
  DEBUG(debug_on,Checking miss operation);
  for (i = 0 ; i < 2; i++){
    /* Fill cache line */
    for ( i =  0; i < associativity; i++) {
      cache_line[i].valid = true;
      cache_line[i].tag = rand()%4096;
      cache_line[i].dirty = 0;
      cache_line[i].rp_value = (associativity <= 2)? rand()%associativity: 3;
      while (cache_line[i].tag == tag) {
        cache_line[i].tag = rand()%4096;
      }
    }
    /* Load operation for i = 0, store for i =1 */
    loadstore = (bool)i;
    status = lru_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     bool(debug_on));
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? MISS_STORE: MISS_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }
  /*
   * Check for hit: block was replaced in last iteration, if we used the same 
   * tag now we will get a hit
   */
  DEBUG(debug_on,hecking hit operation);
  for (i = 0 ; i < 2; i++){
    loadstore = (bool)i;
    status = lru_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     (bool)debug_on);
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? HIT_STORE: HIT_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }
}

/*
 * TEST3: Verifies miss and hit scenarios for nru policy
 * 1. Choose a random associativity
 * 2. Fill a cache entry
 * 3. Force a miss load
 * 4. Check  miss_hit_status == MISS_LOAD
 * 5. Force a miss store
 * 6. Check miss_hit_status == MISS_STORE
 * 7. Force a hit read
 * 8. Check miss_hit_status == HIT_READ
 * 9. Force a hit store
 * 10. miss_hit_status == HIT_STORE
 */
TEST_F(L1cache, hit_miss_nru) {
  int status;
  int i;
  int idx;
  int tag;
  int associativity;
  enum miss_hit_status expected_miss_hit;
  bool loadstore = 1;
  bool debug = 0;
  operation_result result = {};

  /* Fill a random cache entry */
  idx = rand()%1024;
  tag = rand()%4096;
  associativity = 1 << (rand()%4);
  if (debug_on) {
    printf("Entry Info\n idx: %d\n tag: %d\n associativity: %d\n",
          idx,
          tag,
          associativity);
  }
 
  entry cache_line[associativity];
  /* Check for a miss */
  DEBUG(debug_on,Checking miss operation);
  for (i = 0 ; i < 2; i++){
    /* Fill cache line */
    for ( i =  0; i < associativity; i++) {
      cache_line[i].valid = true;
      cache_line[i].tag = rand()%4096;
      cache_line[i].dirty = 0;
      cache_line[i].rp_value = (associativity <= 2)? rand()%associativity: 3;
      while (cache_line[i].tag == tag) {
        cache_line[i].tag = rand()%4096;
      }
    }
    /* Load operation for i = 0, store for i =1 */
    loadstore = (bool)i;
    status = nru_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     bool(debug_on));
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? MISS_STORE: MISS_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }
  /*
   * Check for hit: block was replaced in last iteration, if we used the same 
   * tag now we will get a hit
   */
  DEBUG(debug_on,hecking hit operation);
  for (i = 0 ; i < 2; i++){
    loadstore = (bool)i;
    status = nru_replacement_policy(idx, 
                                     tag, 
                                     associativity,
                                     loadstore,
                                     cache_line,
                                     &result,
                                     (bool)debug_on);
    EXPECT_EQ(status, 0);
    EXPECT_EQ(result.dirty_eviction, 0);
    expected_miss_hit = loadstore ? HIT_STORE: HIT_LOAD;
    EXPECT_EQ(result.miss_hit, expected_miss_hit);
  }
}
/*
 * TEST4: Verifies replacement policy promotion and eviction
 * 1. Choose a random policy 
 * 2. Choose a random associativity
 * 3. Fill a cache entry
 * 4. Insert a new block A
 * 5. Force a hit on A
 * 6. Check rp_value of block A
 * 7. Keep inserting new blocks until A is evicted
 * 8. Check eviction of block A happen after N new blocks were inserted
 * (where N depends of the number of ways)
 */
TEST_F(L1cache, promotion){
	int status;
  	int i;
 	int idx;
  	int tag;
  	int policy;
  	int associativity;
	enum miss_hit_status expected_miss_hit;
 	bool loadstore = 0;
	bool debug = 0;
	struct operation_result result = {};
	
	/* Random policy */
	policy = rand()%3;
	/* Random associativity */
	associativity =  1 << rand()%4;
	struct entry cache_set[associativity];
	/* Fill a cache entry */
	/* Random tag */
	tag = rand()%4096;
	idx = rand()%1024;
	for(int i=0;i<associativity;i++)
	{
		cache_set[i].valid = true;
		cache_set[i].dirty = 0; 
		cache_set[i].tag = rand()%4096; 
		if(cache_set[i].tag == tag)
		{
			 cache_set[i].tag = rand()%4096;
		}
		
		/* Policy rp value*/
		if(policy == 0)	//LRU
		{
			cache_set[i].rp_value = 0;
		}
		else if(policy == 1)	//NRU
		{
			cache_set[i].rp_value = 1;
		}
		else if(policy == 2)	//SRRIP
		{
			if(associativity <= 2)
			{
				cache_set[i].rp_value = 1;
			}
			else
			{
				cache_set[i].rp_value = 3;
			}
		}	
	}
	/* Insert a new block A */
	struct entry A[1];
 	A[0].valid = true;
 	A[0].tag = 256;
  	A[0].dirty = 0;
	/* Policy rp value*/
	if(policy == 0)	//LRU
	{
		A[0].rp_value = 0;
	}
	else if(policy == 1)	//NRU
	{
		A[0].rp_value = 1;
	}
	else if(policy == 2)	//SRRIP
	{
		if(associativity <= 2)
		{
			A[0].rp_value = 1;
		}
		else
		{
			A[0].rp_value = 3;
		}
	}		
	/* Force a hit on A */
	int tag_f = A[0].tag;
	
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag_f,
		                        associativity,
		                        loadstore,
		                        cache_set,
		                        &result,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag_f,
		                        associativity,
		                        loadstore,
		                        cache_set,
		                        &result,
		                        bool(debug_on));
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag_f,
		                        associativity,
		                        loadstore,
		                        cache_set,
		                        &result,
		                        bool(debug_on));
	}
	
	/* Check rp_value of block A */
	int rp_value_set = cache_set[0].rp_value;
	int rp_check;
	
	if(policy == 0)	//LRU
	{
		rp_check = associativity - 1;
		EXPECT_EQ(rp_value_set, rp_check);
	}
	else if(policy == 1)	//NRU
	{
		rp_check = 0;
		EXPECT_EQ(rp_value_set, rp_check);		
	}
	else if(policy == 2)	//SRRIP
	{
		rp_check = 0;
		EXPECT_EQ(rp_value_set, rp_check);		
	}
		
	/* Keep inserting new blocks until A is evicted */
	int tag_random = rand()%4096;
	
	while(result.evicted_address != A[0].tag)
	{
		if(policy == 0)	//LRU
		{
			lru_replacement_policy (idx,
				    tag_random,
				    associativity,
				    loadstore,
				    cache_set,
				    &result,
				    bool(debug_on));
		}
		else if(policy == 1)	//NRU
		{
			nru_replacement_policy (idx,
				    tag_random,
				    associativity,
				    loadstore,
				    cache_set,
				    &result,
				    bool(debug_on));		
		}
		else if(policy == 2)	//SRRIP
		{
			srrip_replacement_policy (idx,
			    tag_random,
			    associativity,
			    loadstore,
			    cache_set,
			    &result,
			    bool(debug_on));		
		}	
	}
	/* Check eviction of block A happen after N new blocks were inserted */
	EXPECT_EQ(result.evicted_address, A[0].tag);
}


/*
 * TEST5: Verifies evicted lines have the dirty bit set accordantly to the operations
 * performed.
 * 1. Choose a random policy
 * 2. Choose a random associativity
 * 3. Fill a cache entry with only read operations
 * 4. Force a write hit for a random block A
 * 5. Force a read hit for a random block B
 * 6. Force read hit for random block A
 * 7. Insert lines until B is evicted
 * 8. Check dirty_bit for block B is false
 * 9. Insert lines until A is evicted
 * 10. Check dirty bit for block A is true
 */
TEST_F(L1cache, writeback){
	int status;
	int i;
	int idx;
	int tag;
	int policy;
	int associativity;
	enum miss_hit_status expected_miss_hit;
	bool loadstore = 0;
	bool debug = 0;
	struct operation_result resultA = {};
	struct operation_result resultB = {};
	
  	/* Choose a random policy */
  	policy = rand()%3;

 	/* Choose a random associativity */
  	associativity = 1 << (rand()%4);

	/* Fill a random cache entry with read-only operations*/
	struct entry cache_set_A[associativity];
	
	/* Random tag */
	tag = rand()%4096;
	idx = rand()%1024;
	for(int i=0;i<associativity;i++)
	{
		cache_set_A[i].valid = true;
		cache_set_A[i].dirty = 0; 
		cache_set_A[i].tag = rand()%4096; 
		if(cache_set_A[i].tag == tag)
		{
			 cache_set_A[i].tag = rand()%4096;
		}
		
		/* Policy rp value*/
		if(policy == 0)	//LRU
		{
			cache_set_A[i].rp_value = 0;
		}
		else if(policy == 1)	//NRU
		{
			cache_set_A[i].rp_value = 1;
		}
		else if(policy == 2)	//SRRIP
		{
			if(associativity <= 2)
			{
				cache_set_A[i].rp_value = 1;
			}
			else
			{
				cache_set_A[i].rp_value = 3;
			}
		}	
	}
	/* Fill the block */
	int tag_check_A = tag;
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}	
	
	/* Force a write hit for a random block A */
	loadstore = 1;
	//int tag_force = tag;
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}

	/* Force a read hit on A */
	loadstore = 0;
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_A,
		                        &resultA,
		                        bool(debug_on));		
	}
		

	/* New block B */
	struct entry cache_set_B[associativity];
	
	/* Random tag */
	tag = rand()%4096;
	idx = rand()%1024;
	for(int i=0;i<associativity;i++)
	{
		cache_set_B[i].valid = true;
		cache_set_B[i].dirty = 0; 
		cache_set_B[i].tag = rand()%4096; 
		if(cache_set_B[i].tag == tag)
		{
			 cache_set_B[i].tag = rand()%4096;
		}
		
		/* Policy rp value*/
		if(policy == 0)	//LRU
		{
			cache_set_B[i].rp_value = 0;
		}
		else if(policy == 1)	//NRU
		{
			cache_set_B[i].rp_value = 1;
		}
		else if(policy == 2)	//SRRIP
		{
			if(associativity <= 2)
			{
				cache_set_B[i].rp_value = 1;
			}
			else
			{
				cache_set_B[i].rp_value = 3;
			}
		}	
	}
	
	/* Fill the block */
	int tag_check_B = tag;
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));		
	}  	
    	
    	/* Force a read hit on B */
	loadstore = 0;
	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
		                        tag,
		                        associativity,
		                        loadstore,
		                        cache_set_B,
		                        &resultB,
		                        bool(debug_on));		
	}    	
    	
	/* Keep inserting new blocks until B is evicted*/
	while(resultB.evicted_address != tag_check_B)
	{
		tag = rand()%4096;
		if(policy == 0)	//LRU
		{
			lru_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_B,
					        &resultB,
					        bool(debug_on));
		}
		else if(policy == 1)	//NRU
		{
			nru_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_B,
					        &resultB,
					        bool(debug_on));		
		}
		else if(policy == 2)	//SRRIP
		{
			srrip_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_B,
					        &resultB,
					        bool(debug_on));		
		} 
	}

	/* Check dirty_bit for block B is false */
	DEBUG(debug_on,Checking dirty bit);
	EXPECT_FALSE(resultB.dirty_eviction);
	EXPECT_EQ(resultB.evicted_address, tag_check_B);


	/* Keep inserting new blocks until A is evicted*/
	while(resultA.evicted_address != tag_check_A)
	{
		tag = rand()%4096;
		if(policy == 0)	//LRU
		{
			lru_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_A,
					        &resultA,
					        bool(debug_on));
		}
		else if(policy == 1)	//NRU
		{
			nru_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_A,
					        &resultA,
					        bool(debug_on));		
		}
		else if(policy == 2)	//SRRIP
		{
			srrip_replacement_policy (idx,
					        tag,
					        associativity,
					        loadstore,
					        cache_set_A,
					        &resultA,
					        bool(debug_on));		
		} 
	}

	/* Check dirty_bit for block A is false */
	DEBUG(debug_on,Checking dirty bit);
	EXPECT_FALSE(resultA.dirty_eviction);
	EXPECT_EQ(resultA.evicted_address, tag_check_A);
}

/*
 * TEST6: Verifies an error is return when invalid parameters are pass
 * performed.
 * 1. Choose a random policy 
 * 2. Choose invalid parameters for idx, tag and asociativy
 * 3. Check function returns a PARAM error condition
 */
TEST_F(L1cache, boundaries){
	int idx;
	int tag;
	int associativity = 1;//just necesary for cache line definition
	int policy;
	bool loadstore;
	bool debug;
	int expected_param_result = PARAM;
	int parameter_result;
	struct entry cache_set[associativity];
	struct operation_result result = {};

	/*Choose a random policy*/
	policy = rand()%3;//Check

	/*Choose invalid parameters for idx, tag and asociativy*/
	idx = -12;
	tag = -9;
	associativity = 12;

	if(policy == 0)	//LRU
	{
		lru_replacement_policy (idx,
				        tag,
				        associativity,
				        loadstore,
				        cache_set,
				        &result,
				        bool(debug_on));
		EXPECT_EQ(expected_param_result, parameter_result);
	}
	else if(policy == 1)	//NRU
	{
		nru_replacement_policy (idx,
				        tag,
				        associativity,
				        loadstore,
				        cache_set,
				        &result,
				        bool(debug_on));
		EXPECT_EQ(expected_param_result, parameter_result);		
	}
	else if(policy == 2)	//SRRIP
	{
		srrip_replacement_policy (idx,
				        tag,
				        associativity,
				        loadstore,
				        cache_set,
				        &result,
				        bool(debug_on));
		EXPECT_EQ(expected_param_result, parameter_result);		
	} 
}
