#include "multi_console_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <string.h> // memset
#include <unistd.h> // sleep | read
#include <errno.h>
#include <pthread.h>

static void* run(void* generic_process)
{
	struct multi_console_process* multi_console_process = (struct multi_console_process*)generic_process;
	struct alternative_t* alternative = multi_console_process->alternative;
	unsigned int buffer_length;
	void* buffer = NULL;

	size_t index;
	while(1)
	{
		index = pri_select(alternative);
	
		struct channel_end_generic_t* channel_end_generic = alternative->channel_end_generic[index];
		if(channel_end_generic->type == CHANNEL_INPUT_TYPE)
		{
			struct channel_input_end_t* channel_input_end = (struct channel_input_end_t*)(channel_end_generic->channel_end);
		
			read_from(channel_input_end, &buffer, &buffer_length);
			if(buffer_length != 0)
			{
				printf("%s%d%s\n", multi_console_process->prefix, *((int*)buffer), multi_console_process->suffix); fflush(stdout);
				free(buffer);
			}
			else
			{
			}
		}
	}

	return NULL;
}

struct multi_console_process* create_multi_console_process(char* prefix, char* suffix, size_t initial_alternative_size)
{
	struct multi_console_process* multi_console_process = (struct multi_console_process*)malloc(sizeof(struct multi_console_process));
	if(multi_console_process != NULL)
	{
		multi_console_process->alternative = create_alternative(initial_alternative_size);
		if(multi_console_process->alternative != NULL)
		{	
			multi_console_process->prefix = (char*)malloc(strlen(prefix));
			strcpy(multi_console_process->prefix, prefix);
			multi_console_process->suffix = (char*)malloc(strlen(suffix));
			strcpy(multi_console_process->suffix, suffix);
			multi_console_process->run = &run;
		}
		else
		{
			free(multi_console_process);
			multi_console_process = NULL;
		}
	}

	return multi_console_process;
}
