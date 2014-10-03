#include "delayer_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <string.h> // memset
#define __USE_BSD // usleep
#include <unistd.h> // usleep only if __USE_BSD defined | write | read | close

static void* run(void* genericProcess)
{
	struct delayer_process* delayer_process = (struct delayer_process*)genericProcess;
	struct channel_input_end_t* channel_input_end = delayer_process->channel_input_end;
	struct channel_output_end_t* channel_output_end = delayer_process->channel_output_end;

	unsigned int delay = delayer_process->delay;

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

struct delayer_process* create_delayer_process(unsigned int delay, struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end)
{
	struct delayer_process* delayer_process = (struct delayer_process*)malloc(sizeof(struct delayer_process));

	if(delayer_process != NULL)
	{
		delayer_process->delay = delay;
		delayer_process->channel_input_end = channel_input_end;
		delayer_process->channel_output_end = channel_output_end;
		delayer_process->run = &run;
	}
	else
	{
		delayer_process = NULL;
	}

	return delayer_process;
}
