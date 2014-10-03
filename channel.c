#include "channel.h"

#include <stdio.h> // malloc | free
#include <stdlib.h> // NULL
#include <string.h> // memcpy
#include <pthread.h>

//#define __DEBUG__

/*****************
 * CHANNEL INPUT *
 *****************/
struct channel_input_end_t* create_channel_input_end()
{
	struct channel_input_end_t* channel_input = (struct channel_input_end_t*)malloc(sizeof(struct channel_input_end_t));
	
	if(channel_input != NULL)
	{
		channel_input->channel = NULL;
	}
	
	return channel_input;
}

void prepare_read(struct channel_input_end_t* channel_input)
{
	#ifdef __DEBUG__
	printf("READ : WANTS TO LOCK MUTEX\n"); fflush(stdout);
	#endif
	pthread_mutex_lock(channel_input->channel->mutex);
	#ifdef __DEBUG__
	printf("READ : HAS LOCKED MUTEX\n"); fflush(stdout);
	#endif

	// The mutex has been locked
	if(channel_input->channel->am_I_first == I_AM_FIRST) // Can be optimized by !channel->am_I_first when I_AM_FIRST = 0
	{
		#ifdef __DEBUG__
		printf("READ IS FIRST : SETS am_I_first TO READ_IS_FIRST\n"); fflush(stdout);
		#endif
		channel_input->channel->am_I_first = READ_IS_FIRST;
		#ifdef __DEBUG__
		printf("READ IS FIRST : HAS SET am_I_first TO READ_IS_FIRST\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("READ IS FIRST : WAITS ON THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_wait(channel_input->channel->data_available, channel_input->channel->mutex);
		#ifdef __DEBUG__
		printf("READ IS FIRST : HAS WAITED ON THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
	}
}

void finish_read(struct channel_input_end_t* channel_input)
{
	if(channel_input->channel->am_I_first == READ_IS_FIRST)
	{
		#ifdef __DEBUG__
		printf("READ IS FIRST : DELETE DATA\n"); fflush(stdout);
		#endif
		channel_input->channel->am_I_first = I_AM_FIRST;
		channel_input->channel->buffer_length = 0;
		channel_input->channel->buffer = NULL;
		#ifdef __DEBUG__
		printf("READ IS FIRST : DATA DELETED\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("READ IS FIRST : WANTS TO SIGNAL THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_signal(channel_input->channel->process_finished);
		#ifdef __DEBUG__
		printf("READ IS FIRST : HAS TO SIGNALED THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
	}
	else
	{
		#ifdef __DEBUG__
		printf("READ IS SECOND : SIGNALS TO WRITE THAT IT FINISHED\n"); fflush(stdout);
		#endif
		pthread_cond_signal(channel_input->channel->data_available);
		#ifdef __DEBUG__
		printf("READ IS SECOND : HAS SIGNALED TO WRITE THAT IT FINISHED\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("READ IS SECOND : WAITS ON THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_wait(channel_input->channel->process_finished, channel_input->channel->mutex);
		#ifdef __DEBUG__
		printf("READ IS SECOND : HAS WAITED ON THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
	}

	#ifdef __DEBUG__
	printf("READ : WANTS TO UNLOCK THE MUTEX\n"); fflush(stdout);
	#endif
	pthread_mutex_unlock(channel_input->channel->mutex);
	#ifdef __DEBUG__
	printf("READ : HAS UNLOCKED THE MUTEX\n"); fflush(stdout);
	#endif
}

void read_from(struct channel_input_end_t* channel_input, void** buffer, unsigned int* buffer_length)
{
	prepare_read(channel_input);
	*buffer_length = channel_input->channel->buffer_length;
	if(*buffer_length != 0)
	{
		*buffer = (void**)malloc(*buffer_length);
		if(*buffer != NULL)
		{
			memcpy(*buffer, channel_input->channel->buffer, *buffer_length);
		}
	}
	else
	{
		channel_input->channel->buffer = NULL;
	}
	finish_read(channel_input);
}

/******************
 * CHANNEL OUTPUT *
 ******************/
struct channel_output_end_t* create_channel_output_end()
{
	struct channel_output_end_t* channel_output = (struct channel_output_end_t*)malloc(sizeof(struct channel_output_end_t));
	
	if(channel_output != NULL)
	{
		channel_output->channel = NULL;
	}
	
	return channel_output;
}

void prepare_write(struct channel_output_end_t* channel_output)
{
	#ifdef __DEBUG__
	printf("WRITE : WANTS TO LOCK MUTEX\n"); fflush(stdout);
	#endif
	pthread_mutex_lock(channel_output->channel->mutex);
	#ifdef __DEBUG__
	printf("WRITE : HAS LOCKED MUTEX\n"); fflush(stdout);
	#endif

	// The mutex has been locked
	if(channel_output->channel->am_I_first == I_AM_FIRST) // Can be optimized by !channel->am_I_first when I_AM_FIRST = 0
	{
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : SETS am_I_first TO WRITE_IS_FIRST\n"); fflush(stdout);
		#endif
		channel_output->channel->am_I_first = WRITE_IS_FIRST;
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : HAS SET am_I_first TO WRITE_IS_FIRST\n"); fflush(stdout);
		#endif
	}
}

void finish_write(struct channel_output_end_t* channel_output)
{
	if(channel_output->channel->am_I_first == WRITE_IS_FIRST)
	{
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : WAITS ON THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_wait(channel_output->channel->data_available, channel_output->channel->mutex);
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : HAS WAITED ON THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : DELETE DATA\n"); fflush(stdout);
		#endif
		channel_output->channel->am_I_first = I_AM_FIRST;
		channel_output->channel->buffer_length = 0;
		channel_output->channel->buffer = NULL;
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : DATA DELETED\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : WANTS TO SIGNAL THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_signal(channel_output->channel->process_finished);
		#ifdef __DEBUG__
		printf("RWRITEEAD IS FIRST : HAS TO SIGNALED THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : WANTS TO UNLOCK THE MUTEX\n"); fflush(stdout);
		#endif
		pthread_mutex_unlock(channel_output->channel->mutex);
		#ifdef __DEBUG__
		printf("WRITE IS FIRST : HAS UNLOCKED THE MUTEX\n"); fflush(stdout);
		#endif
	}
	else
	{
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : SIGNALS THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_signal(channel_output->channel->data_available);
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : HAS SIGNALED THE CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : WAITS ON THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		pthread_cond_wait(channel_output->channel->process_finished, channel_output->channel->mutex);
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : HAS WAITED ON THE INTEGRITY CONDITION VARIABLE\n"); fflush(stdout);
		#endif
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : WANTS TO UNLOCK THE MUTEX\n"); fflush(stdout);
		#endif
		pthread_mutex_unlock(channel_output->channel->mutex);
		#ifdef __DEBUG__
		printf("WRITE IS SECOND : HAS UNLOCKED THE MUTEX\n"); fflush(stdout);
		#endif
	}
}

void write_to(struct channel_output_end_t* channel_output, void* buffer, unsigned int buffer_length)
{
	prepare_write(channel_output);
	channel_output->channel->buffer_length = buffer_length;
	channel_output->channel->buffer = buffer;
	finish_write(channel_output);
}

void* write_to_destination(void* generic_destination)
{
	struct write_destination_t* write_destination = (struct write_destination_t*)generic_destination;

	write_to(write_destination->channel_output_end, write_destination->buffer, write_destination->buffer_length);

	return NULL;
}

/***********
 * CHANNEL *
 ***********/
struct one_2_one_channel_t* create_one_2_one_channel()
{
	struct one_2_one_channel_t* one_2_one_channel = NULL;

	struct channel_input_end_t* channel_input_end = create_channel_input_end();
	if(channel_input_end != NULL)
	{
		struct channel_output_end_t* channel_output_end = create_channel_output_end();
		if(channel_output_end != NULL)
		{
			if(!connect_channels(channel_input_end, channel_output_end))
			{
				one_2_one_channel = (struct one_2_one_channel_t*)malloc(sizeof(struct one_2_one_channel_t));
				if(one_2_one_channel != NULL)
				{
					one_2_one_channel->channel_output_end = channel_output_end;
					one_2_one_channel->channel_input_end = channel_input_end;
				}
				else
				{
					free(channel_input_end);
					channel_input_end = NULL;
					free(channel_output_end);
					channel_output_end = NULL;
					free(one_2_one_channel);
					one_2_one_channel = NULL;
				}
			}
			else
			{
				free(channel_input_end);
				channel_input_end = NULL;
				free(channel_output_end);
				channel_output_end = NULL;
				free(one_2_one_channel);
				one_2_one_channel = NULL;
			}
		}
		else
		{
			free(channel_input_end);
			channel_input_end = NULL;
			one_2_one_channel = NULL;
		}
	}
	else
	{
		one_2_one_channel = NULL;
	}

	return one_2_one_channel;
}

int connect_channels(struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end)
{
	int result = 0;

	if(channel_input_end != NULL && channel_output_end != NULL)
	{
		struct channel_t* channel = (struct channel_t*)malloc(sizeof(struct channel_t));
		
		if(channel != NULL)
		{
			pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
			pthread_mutexattr_t mutex_attr;
			pthread_mutexattr_init(&mutex_attr);
			pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
			pthread_mutex_init(mutex, &mutex_attr);
			
			if(mutex != NULL)
			{
				pthread_cond_t* data_available = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
				pthread_condattr_t cond_attr;
				pthread_condattr_init(&cond_attr);
				pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
				pthread_cond_init(data_available, &cond_attr);
				
				if(data_available != NULL)
				{
					pthread_cond_t* process_finished = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
					pthread_condattr_t integrity_cond_attr;
					pthread_condattr_init(&integrity_cond_attr);
					pthread_condattr_setpshared(&integrity_cond_attr, PTHREAD_PROCESS_SHARED);
					pthread_cond_init(process_finished, &integrity_cond_attr);

					if(process_finished != NULL)
					{
						channel->mutex = mutex;
						channel->data_available = data_available;
						channel->process_finished = process_finished;
						channel->am_I_first = 0;
					
						channel_input_end->channel = channel;
						channel_output_end->channel = channel;
					}
					else
					{
						free(data_available);
						data_available = NULL;
						free(mutex);
						mutex = NULL;
						free(channel);
						channel = NULL;
					}
				}
				else
				{
					free(mutex);
					mutex = NULL;
					free(channel);
					channel = NULL;
				}
			}
			else
			{
				free(channel);
				channel = NULL;	
			}
		}
	}
	else if(channel_input_end == NULL)
	{
		// Input end null
		result -= 1;
	}
	else
	{
		// Output end null
		result -= 2;
	}

	return result;
}
