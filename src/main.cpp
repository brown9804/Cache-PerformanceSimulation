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

void print_usage (int LOAD_MISS, int STORE_MISS, int LOAD_HIT, int STORE_HIT, int DIRTY_EVICTION, int TOTAL_ADDRESS)
{
	
	float MISS_RATE = (LOAD_MISS+STORE_MISS)*pow(TOTAL_ADDRESS,-1);
	float READ_MISS_RATE = LOAD_MISS*pow(LOAD_MISS+STORE_MISS,-1);
	
	std::cout<< "Overall miss rate: "<< MISS_RATE <<'\n';
	std::cout<< "Read miss rate: "<< READ_MISS_RATE <<'\n';
	std::cout<< "Dirty evictions: "<< DIRTY_EVICTION <<'\n';
	std::cout<< "Load misses: "<< LOAD_MISS <<'\n';
	std::cout<< "Store misses: "<< STORE_MISS <<'\n';
	std::cout<< "Total_misses: "<< LOAD_MISS+STORE_MISS <<'\n';
	std::cout<< "Load hits: "<< LOAD_HIT <<'\n';
	std::cout<< "Store hits: "<< STORE_HIT <<'\n';
	std::cout<< "Total hits: "<< LOAD_HIT + STORE_HIT <<'\n';
	exit (0);
}

int main(int argc, char * argv []) {
  printf("Do something :), don't forget to keep track of execution time");
  /* Parse argruments */
  
  
  
/*-------------------------Jonathan Ramirez------------------------------------------*/
int size_p=atoi(argv[2]);
	int policy;
	int TOTAL_ADDRESS = 0;
	
	int size_p=atoi(argv[2]);
	int assoc_p=atoi(argv[4]);
	int block_p=atoi(argv[6]);
	
	const char *LRU_policy = "LRU";
	const char *NRU_policy = "NRU";
	const char *RRIP_policy = "RRIP";
	const char *random = "RANDOM";
	
	if(*argv[8]==*LRU_policy)
	{
		policy = LRU;
	}	
	
	if(*argv[8]==*NRU_policy)
	{
		policy = NRU;
	}
	
	if(*argv[8]==*RRIP_policy)
	{
		policy = RRIP;
	}
	
	if(*argv[8]==*random)
	{
		policy = RANDOM;
	}	
	
	struct cache_params argv_prm;
	
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
	
	struct operation_result *op_out[sets*assoc_p], result_out[sets*assoc_p];
	for(int i = 0; i < sets*assoc_p; i++)	/* mem allocation */
	{
		op_out[i] = &result_out[i];
	}	

	for(int i = 0; i < sets*assoc_p; i++)
	{
		op_out[i]->dirty_eviction = 0;
		op_out[i]->evicted_address = 0;
	}
		
	struct cache_field_size *argv_out, field_out;
	argv_out = &field_out; /* mem allocation*/	
		
	//idx, offset, tag bits from parameters.
	field_size_get(argv_prm, argv_out);
	
	/* Parse arguments */
	/* Get trace's lines and start your simulation */
	
	/*Index, tag from direction.*/
	while(fgets(buffer, sizeof(buffer), stdin) != NULL)
  	{
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
		                   	*op_out,
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
		                   	*op_out,
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
		                   	*op_out,
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
  	}
  	fclose(stdin);
  /*-------------------------------------------------------------------------------*/
	/* Print cache configuration */
	
	print_cache(size_p, assoc_p, block_p);
  
	/* Print Statistics */
	print_usage(LOAD_MISS, STORE_MISS, LOAD_HIT, STORE_HIT, DIRTY_EVICTION, TOTAL_ADDRESS);
	std::cout<< TOTAL_ADDRESS<<'\n';
return 0;
}
