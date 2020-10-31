#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
#include <L1cache.h>
#include <debug_utilities.h>


/* Helper funtions */
void print_usage ()
{
  printf ("Print print_usage\n");
  exit (0);
}

int main(int argc, char * argv []) {
  printf("Do something :), don't forget to keep track of execution time");
  /* Parse argruments */
  
  
  
/*-------------------------Jonathan Ramirez------------------------------------------*/
int size_p=atoi(argv[2]);
	int assoc_p=atoi(argv[4]);
	int block_p=atoi(argv[6]);
		
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
	/*Index, tag from direction.*/
	while(fgets(buffer, sizeof(buffer), stdin) != NULL)
  	{
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
		
		if(*argv[8] == '1') /* NRU */
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
  	}
  	fclose(stdin);
  /*-------------------------------------------------------------------------------*/
  /* Get trace's lines and start your simulation */

  /* Print cache configuration */

  /* Print Statistics */
return 0;
}
