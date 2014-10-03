#include "console_process.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <string.h> // memset
#include <unistd.h> // sleep | read
#include <errno.h>
#include <pthread.h>

static void* run(void* generic_process)
{
	struct console_process* console_process = (struct console_process*)generic_process;
	struct channel_input_end_t* channel_input_end = console_process->channel_input_end;
	unsigned int buffer_length;
	void* buffer = NULL;

	read_from(channel_input_end, &buffer, &buffer_length);
	while(buffer_length != 0)
	{	
		printf("%s%s%s\n", console_process->prefix, *(char*)buffer), console_process->suffix); fflush(stdout);
		free(buffer);
		read_from(channel_input_end, &buffer, &buffer_length);
	}

	return NULL;
}

struct console_process* create_console_process(char* prefix, char* suffix, struct channel_input_end_t* channel_input_end)
{
	struct console_process* console_process = (struct console_process*)malloc(sizeof(struct console_process));
	if(console_process != NULL)
	{
		console_process->prefix = (char*)malloc(strlen(prefix));
		strcpy(console_process->prefix, prefix);
		console_process->suffix = (char*)malloc(strlen(suffix));
		strcpy(console_process->suffix, suffix);
		console_process->channel_input_end = channel_input_end;
		console_process->run = &run;
	}

	return console_process;
}
