#include "copy_process.h"

#include <stdlib.h> // malloc | free
#include <pthread.h>

static void* run(void* generic_process)
{
	struct copy_process* copier = (struct copy_process*)generic_process;

	unsigned int buffer_length;
	void* buffer = NULL;

	struct write_destination_t destination_1;
	destination_1.channel_output_end = copier->channel_output_end_1;

	struct write_destination_t destination_2;
	destination_2.channel_output_end = copier->channel_output_end_2;

	pthread_t copier_thread_1,
			  copier_thread_2;

	read_from(copier->channel_input_end, &buffer, &buffer_length);

	while(buffer_length != 0)
	{
		destination_1.buffer = buffer;
		destination_1.buffer_length = buffer_length;
		
		destination_2.buffer = buffer;
		destination_2.buffer_length = buffer_length;
		
		pthread_create(&copier_thread_1, NULL, write_to_destination, &destination_1);
		pthread_create(&copier_thread_2, NULL, write_to_destination, &destination_2);

		pthread_join(copier_thread_1, NULL);
		pthread_join(copier_thread_2, NULL);

		free(buffer);
		read_from(copier->channel_input_end, &buffer, &buffer_length);
	}
	
	destination_1.buffer = NULL;
	destination_1.buffer_length = 0;
		
	destination_2.buffer = NULL;
	destination_2.buffer_length = 0;
		
	pthread_create(&copier_thread_1, NULL, write_to_destination, &destination_1);
	pthread_create(&copier_thread_2, NULL, write_to_destination, &destination_2);

	pthread_join(copier_thread_1, NULL);
	pthread_join(copier_thread_2, NULL);

	return NULL;
}

struct copy_process* create_copy_process(struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end_1, struct channel_output_end_t* channel_output_end_2)
{
	struct copy_process* copy_process = (struct copy_process*)malloc(sizeof(struct copy_process));

	if(copy_process != NULL)
	{
		copy_process->channel_input_end = channel_input_end;
		copy_process->channel_output_end_1 = channel_output_end_1;
		copy_process->channel_output_end_2 = channel_output_end_2;
		copy_process->run = &run;
	}

	return copy_process;
}
