#include "alternative.h"

#define __USE_BSD // usleep
#include <unistd.h> // usleep only if __USE_BSD defined | write | read | close
#include <stdio.h> // NULL

struct alternative_t* create_alternative(size_t initial_size)
{
	struct alternative_t* alternative = (struct alternative_t*)malloc(sizeof(struct alternative_t));

	if(alternative != NULL)
	{
		alternative->size = initial_size;
		alternative->used = 0;

		alternative->channel_end_generic = (struct channel_end_generic_t**)malloc(sizeof(struct channel_end_generic_t*) * initial_size);
	}
	else
	{
	}

	return alternative;
}

size_t add_to_alternative(struct channel_end_generic_t* channel_end_generic, struct alternative_t* alternative)
{
	if(alternative->used == alternative->size)
	{
		alternative->size *= 2;
		alternative->channel_end_generic = (struct channel_end_generic_t**)realloc(alternative->channel_end_generic, sizeof(struct channel_end_generic_t*) * alternative->size);
	}

	alternative->channel_end_generic[alternative->used] = channel_end_generic;
	
	return alternative->used++;
}

size_t pri_select(struct alternative_t* alternative)
{	
	size_t i;
	unsigned char selected = 0;
	
	while(!selected)
	{
		i = 0;

		while(i < alternative->used && !selected)
		{
			if(alternative->channel_end_generic[i]->type == CHANNEL_INPUT_TYPE)
			{
				struct channel_input_end_t* channel_input = ((struct channel_input_end_t*)alternative->channel_end_generic[i]->channel_end);
				pthread_mutex_lock(channel_input->channel->mutex);
				selected = (channel_input->channel->am_I_first == WRITE_IS_FIRST);

				pthread_mutex_unlock(channel_input->channel->mutex);
			}

			i++;
		}
		
		i--;
		
		if(selected)
		{
			return i;
		}
		
		usleep(100000);
	}
	
	return i;
}
