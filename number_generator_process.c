#include "number_generator_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <unistd.h> // write | close
#include <string.h> //strerror
#include <errno.h> // errno
#include <pthread.h> // trylock

static void* run(void* genericProcess)
{
	struct number_generator_process* number_generator_process = (struct number_generator_process*)genericProcess;
	struct channel_output_end_t* channel_output_end = number_generator_process->channel_output_end;
	const int min = number_generator_process->min;
	const int max = number_generator_process->max;
	const int incrementation = number_generator_process->incrementation;
	int i = min;
	
	while(i <= max)
	{
		write_to(channel_output_end, &i, ((unsigned int)sizeof(int)));
		i += incrementation;
	}
	
	write_to(channel_output_end, NULL, 0);
	
	return NULL;
}

struct number_generator_process* create_number_generator_process(int min, int max, int incrementation, struct channel_output_end_t* channel_output_end)
{
	struct number_generator_process* number_generator_process = (struct number_generator_process*)malloc(sizeof(struct number_generator_process));

	if(number_generator_process != NULL)
	{
		number_generator_process->min = min;
		number_generator_process->max = max;
		number_generator_process->incrementation = incrementation;
		number_generator_process->channel_output_end = channel_output_end;
		number_generator_process->run = &run;
	}
	else
	{
		number_generator_process = NULL;
	}

	return number_generator_process;
}
