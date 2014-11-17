#include "delay_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <string.h> // memset
#define __USE_BSD // usleep
#include <unistd.h> // usleep only if __USE_BSD defined | write | read | close

static void* run(void* genericProcess)
{
	struct delay_process* delay_process = (struct delay_process*)genericProcess;
	struct channel_input_end_t* channel_input_end = delay_process->channel_input_end;
	struct channel_output_end_t* channel_output_end = delay_process->channel_output_end;

	unsigned int delay = delay_process->delay;

	unsigned int buffer_length;	
	void* buffer = NULL;

	read_from(channel_input_end, &buffer, &buffer_length);
	usleep(delay);
	write_to(channel_output_end, buffer, buffer_length);
	while(buffer_length != 0)
	{
		free(buffer);
		read_from(channel_input_end, &buffer, &buffer_length);
		usleep(delay);
		write_to(channel_output_end, buffer, buffer_length);
	}	

	return NULL;
}

struct delay_process* create_delay_process(unsigned int delay, struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end)
{
	struct delay_process* delay_process = (struct delay_process*)malloc(sizeof(struct delay_process));

	if(delay_process != NULL)
	{
		delay_process->delay = delay;
		delay_process->channel_input_end = channel_input_end;
		delay_process->channel_output_end = channel_output_end;
		delay_process->run = &run;
	}
	else
	{
		delay_process = NULL;
	}

	return delay_process;
}
