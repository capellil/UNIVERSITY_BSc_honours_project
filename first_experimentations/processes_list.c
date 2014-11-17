#include "processes_list.h"

#include <pthread.h> // pthread_t

struct processes_list_t* create_processes_list(size_t initial_size)
{
	struct processes_list_t* processes_list = (struct processes_list_t*)malloc(sizeof(struct processes_list_t));
	if(processes_list != NULL)
	{
		processes_list->processes = (void**)malloc(initial_size * sizeof(void*));
		if(processes_list->processes != NULL)
		{
			processes_list->runs = (void* (**)(void*))(malloc(initial_size * 	sizeof(void* (*)(void*))));
			
			if(processes_list->runs != NULL)
			{
				processes_list->used = 0;
				processes_list->size = initial_size;
			}
			else
			{
				free(processes_list->processes);
				processes_list->processes = NULL;
				free(processes_list);
				processes_list = NULL;
			}
		}
		else
		{
			free(processes_list);
			processes_list = NULL;
		}
	}
	else
	{
	}
	
	return processes_list;
}

void insert_processes_list(struct processes_list_t* processes_list, void* process, void* (*run)(void*)) 
{
	if (processes_list->used == processes_list->size) 
	{
		size_t initial_size = processes_list->size;
		processes_list->size *= 2;
		processes_list->processes = (void**)(realloc(processes_list->processes, processes_list->size * sizeof(void*)));
		processes_list->runs = (void* (**)(void*))(realloc(processes_list->runs, initial_size * sizeof(void* (*)(void*))));
	}

	processes_list->processes[processes_list->used] = process;
	processes_list->runs[processes_list->used++] = run;
}

void free_processes_list(struct processes_list_t* processes_list) 
{
	for(unsigned int i = 0; i < processes_list->size; ++i)
	{
		free(processes_list->processes[i]);
	}
	
	free(processes_list);
	
	processes_list->processes = NULL;
	processes_list->used = 0;
	processes_list->size = 0;
}

void run_in_parallel(struct processes_list_t* processes_list)
{
	pthread_t* allThreads = malloc(sizeof(pthread_t) * processes_list->used);
	
	if(allThreads != NULL)
	{
		for(unsigned int i = 0; i < processes_list->used; ++i)
		{
			pthread_create(&allThreads[i], NULL, processes_list->runs[i], processes_list->processes[i]);
		}
		for(unsigned int i = 0; i < processes_list->used; ++i)
		{
			pthread_join(allThreads[i], NULL);
			free(processes_list->processes[i]);
		}
	}
	else
	{
		
	}
}
