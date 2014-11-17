#include "prefix_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // free | malloc

static void* run_prefix_process(void* generic_process)
{
	struct prefix_process* prefix = (struct prefix_process*)generic_process;
	
	// We write the prefix
	write_to(prefix->channel_output_end, &prefix->prefix_value, sizeof(int));
	
	void* buffer = NULL;
	unsigned int buffer_length;

	// Then we just keep copying what we read to the channel output end
	read_from(prefix->channel_input_end, &buffer, &buffer_length);
	while(buffer_length)
	{
		write_to(prefix->channel_output_end, buffer, buffer_length);
		free(buffer);
		read_from(prefix->channel_input_end, &buffer, &buffer_length);
	}

	return NULL;
}

struct prefix_process* create_prefix_process(int prefix_value, struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end)
{
	struct prefix_process* prefix = (struct prefix_process*)malloc(sizeof(struct prefix_process));

	if(prefix) // Equivalent to if(prefix != NULL)
	{
		prefix->prefix_value = prefix_value;
		prefix->channel_input_end = channel_input_end;
		prefix->channel_output_end = channel_output_end;
		prefix->run = &run_prefix_process;
	}
	else
	{
	}

	return prefix;
}
