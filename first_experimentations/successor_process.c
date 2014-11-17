#include "successor_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // free | malloc

static void* run_successor_process(void* generic_process)
{
	struct successor_process* successor= (struct successor_process*) generic_process;
	
	void* buffer = NULL;
	unsigned int buffer_length;

	read_from(successor->channel_input_end, &buffer, &buffer_length);
	while(buffer_length)
	{
		(*((int*)buffer))++;
		write_to(successor->channel_output_end, buffer, buffer_length);
		free(buffer);
		read_from(successor->channel_input_end, &buffer, &buffer_length);
	}

	return NULL;
}

struct successor_process* create_successor_process(struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end)
{
	struct successor_process* successor = (struct successor_process*)malloc(sizeof(struct successor_process));

	if(successor)// Equivalent to if(successor != NULL)
	{
		successor->channel_input_end = channel_input_end;
		successor->channel_output_end = channel_output_end;
		successor->run = run_successor_process;
	}
	else
	{
	}

	return successor;
}
