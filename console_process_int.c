#include "console_process_int.h"

#include <stdio.h> // NULL
#include <stdlib.h> // malloc | free
#include <string.h> // memset
#include <unistd.h> // sleep | read

static void* run(void* generic_process)
{
	struct console_process_int* console_process_int = (struct console_process_int*)generic_process;
	struct channel_input_end_t* channel_input_end = console_process_int->channel_input_end;
	unsigned int buffer_length;
	void* buffer = NULL;

	read_from(channel_input_end, &buffer, &buffer_length);
	while(buffer_length != 0)
	{	
		printf("%s%d%s\n", console_process_int->prefix, *((int*)buffer), console_process_int->suffix); fflush(stdout);
		free(buffer);
		read_from(channel_input_end, &buffer, &buffer_length);
	}

	return NULL;
}

struct console_process_int* create_console_process_int(char* prefix, char* suffix, struct channel_input_end_t* channel_input_end)
{
	struct console_process_int* console_process_int = (struct console_process_int*)malloc(sizeof(struct console_process_int));
	if(console_process_int != NULL)
	{
		console_process_int->prefix = (char*)malloc(strlen(prefix));
		strcpy(console_process_int->prefix, prefix);
		console_process_int->suffix = (char*)malloc(strlen(suffix));
		strcpy(console_process_int->suffix, suffix);
		console_process_int->channel_input_end = channel_input_end;
		console_process_int->run = &run;
	}

	return console_process_int;
}
