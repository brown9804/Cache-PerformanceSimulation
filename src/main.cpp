#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
#include <L1cache.h>
#include <debug_utilities.h>


char buffer[256];

/* Helper funtions */
void print_cache (int cache_size, int assoc, int block_size)
{
	std::cout<< "Cache Size (KB): "<< cache_size <<'\n';	
	std::cout<< "Cache Associativity: "<< assoc <<'\n';
	std::cout<< "Cache Block Size (bytes): "<< block_size <<'\n';
}

void print_usage (int LOAD_MISS_p, int STORE_MISS_p, int LOAD_HIT_p, int STORE_HIT_p, int DIRTY_EVICTION_p, int TOTAL_ADDRESS)
{
	
	float MISS_RATE_p = (LOAD_MISS_p+STORE_MISS_p)*pow(TOTAL_ADDRESS,-1);
	float READ_MISS_RATE_p = LOAD_MISS_p*pow(LOAD_MISS_p+STORE_MISS_p,-1);
	
	std::cout<< "Overall miss rate: "<< MISS_RATE_p <<'\n';
	std::cout<< "Read miss rate: "<< READ_MISS_RATE_p <<'\n';
	std::cout<< "Dirty evictions: "<< DIRTY_EVICTION_p <<'\n';
	std::cout<< "Load misses: "<< LOAD_MISS_p <<'\n';
	std::cout<< "Store misses: "<< STORE_MISS_p <<'\n';
	std::cout<< "Total_misses: "<< LOAD_MISS_p+STORE_MISS_p <<'\n';
	std::cout<< "Load hits: "<< LOAD_HIT_p <<'\n';
	std::cout<< "Store hits: "<< STORE_HIT_p <<'\n';
	std::cout<< "Total hits: "<< LOAD_HIT_p + STORE_HIT_p <<'\n';
	exit (0);
}
int main(int argc, char * argv []) {
  // printf("Do something :), don't forget to keep track of execution time \n");
  /* Parse argruments */
  
  // Time Implementation - Brown, Belinda 
  time_t current_time;
  time_t final_time;

  time(&current_time); // capture time
  
/*-------------------------Jonathan Ramirez------------------------------------------*/

	float load_h = 0;
	float load_m = 0;
	float store_h = 0;
	float store_m = 0;
	float dirty_eviction = 0;	    	

	int policy;
	int TOTAL_ADDRESS = 0;
	
	int size_p=atoi(argv[2]);
	int assoc_p=atoi(argv[4]);
	int block_p=atoi(argv[6]);

	const char *LRU_policy = "LRU";
	const char *NRU_policy = "NRU";
	const char *RRIP_policy = "SRRIP";
	const char *random = "RANDOM";
	
	if(*argv[8]==*LRU_policy)
	{
		policy = LRU;
	}	
	
	else if(*argv[8]==*NRU_policy)
	{
		policy = NRU;
	}
	
	else if(*argv[8]==*RRIP_policy)
	{
		policy = RRIP;
	}
	
	else if(*argv[8]==*random)
	{
		policy = RANDOM;
	}	
	struct cache_pararms argv_prm;
	
	argv_prm.size = size_p;
	argv_prm.asociativity = assoc_p;
	argv_prm.block_size = block_p;

	int tags = (size_p*1024)/block_p;    /* REVISAR */ 
	int sets = (size_p*1024)/(block_p*assoc_p);

	struct entry *cache_tags[sets*assoc_p], cache_out[sets*assoc_p];
	for(int i = 0; i < sets*assoc_p; i++)	/* mem allocation */
	{
		cache_tags[i] = &cache_out[i];
	}
	
	for(int i = 0; i < sets*assoc_p; i++)
	{
		cache_tags[i]->valid = 0;
		cache_tags[i]->dirty = 0;
		cache_tags[i]->tag = 0;
		cache_tags[i]->rp_value = 0;
		cache_tags[i]->obl_tag = 0;
	}
	
	/*for(int i = 0; i < sets*assoc_p; i++)
	{
		op_out[i] = &result_out[i];
	}	

	for(int i = 0; i < sets*assoc_p; i++)
	{
		op_out[i]->dirty_eviction = 0;
		op_out[i]->evicted_address = 0;
	}*/
		
	struct cache_field_size *argv_out, field_out;
	argv_out = &field_out; /* mem allocation*/	
		
	//idx, offset, tag bits from parameters.
	field_size_get(argv_prm, argv_out);
	
 	struct operation_result *op_out, result_out;
	op_out = &result_out;	
	
	/* Parse arguments */
	/* Get trace's lines and start your simulation */
	
	/*Index, tag from direction.*/
	while(fgets(buffer, sizeof(buffer), stdin) != NULL)
  	{
		//std::cout<< TOTAL_ADDRESS <<'\n';
  		TOTAL_ADDRESS = TOTAL_ADDRESS+1;
  	
  		/* Buffer tokens */
  		const char *delim = " ";
  		const char *null = strtok(buffer, delim);
		const char *inst_type_d = strtok(NULL, delim);
		const char *direction_d = strtok(NULL, delim);
		const char *IC_d = strtok(NULL, delim);
		
		int inst_type = atoi(inst_type_d);
		long int direction = strtol(direction_d, NULL, 16);
		int IC = atoi(IC_d);
	
		int *etq, mem_etq;
		etq = &mem_etq;
		int *index, mem_index;
		index = &mem_index;
		bool type_op;
		bool debug = false;
		
		if(inst_type == 0)
		{
			type_op = false;
		}
		else
		{
			type_op = true;
		}		
		
		address_tag_idx_get(direction, *argv_out, index, etq);
		
		/*---------------------------------------------Simulation------------------------------------------*/
		POLICY:
			if(policy == 0) /* LRU */
			{
				lru_replacement_policy(
					*index,
		                  	*etq,
		                 	assoc_p, 
		                   	type_op, 
		                   	*cache_tags,
		                   	op_out,
		                   	debug);
				/*Usar contadores que están definidos en el header */
			}		
			
			
			if(policy == 1) /* NRU */
			{
				nru_replacement_policy(
					*index,
		                  	*etq,
		                 	assoc_p, 
		                   	type_op, 
		                   	*cache_tags,
		                   	op_out,
		                   	debug);
			}
			
			if(policy == 2) /* RRIP */
			{
				srrip_replacement_policy(
					*index,
		                  	*etq,
		                 	assoc_p, 
		                   	type_op, 
		                   	*cache_tags,
		                   	op_out,
		                   	debug);
				
				/*Usar contadores que están definidos en el header */
				
			}
			
			if(policy == 3)
			{
				int num;
				srand(time(NULL));
				num = rand()%3;
				policy = num;
				goto POLICY;
			}
		/*-----------------------------------------------------------------------------------------------*/	
		
		/* Metrics */
		if (op_out->miss_hit == HIT_LOAD){
		load_h++;
		} 
		else if (op_out->miss_hit == MISS_LOAD){
		load_m++;
		}
		else if (op_out->miss_hit == HIT_STORE){
		store_h++;
		}
		else if (op_out->miss_hit == MISS_STORE){
		store_m++;
		}
		if (op_out->dirty_eviction == true){
		dirty_eviction++;
		}			
  	}
  	fclose(stdin);
	// Time Implementation - Brown, Belinda
	time(&final_time); // capture time
        int ex_time = difftime(final_time,current_time);
	std::cout << "Time running: " << ex_time << " seconds" << std::endl;

	/* Print cache configuration */
	print_cache(size_p, assoc_p, block_p);
	/* Print Statistics */
	print_usage(load_m, store_m, load_h, store_h, dirty_eviction, TOTAL_ADDRESS);
	return 0;
}
