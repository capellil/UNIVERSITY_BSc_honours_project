/**
 * @file alternative.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 26/09/2014
 **/

#include "channel.h"

#include <stdlib.h> // malloc | realloc | free
#include <pthread.h> // pthread_mutex_lock | pthread_mutex_unlock

/**
 * @brief A structure standing for an alternative.
 **/
struct alternative_t
{
	/// An array of channel ends generic
	struct channel_end_generic_t** channel_end_generic;
	
	/// The maximum size of the alternative's channel ends generic array
	size_t size;
	
	/// The number of channel ends generic
	size_t used;	
};

/**
 * @brief Creates an alternative with its channel ends generic array length set to initial_size.
 * @param intial_size[in] The size to be assigned to the channel ends generic array.
 * @return The created alternative structure. NULL if it failed.
 * @pre \li intial_size > 0
 **/
struct alternative_t* create_alternative(size_t initial_size);

/**
 * @brief Adds the given channel end generic at the end of the given alternative.
 * @param channel_end_generic[in] The channel end to be added.
 * @param alternative[out] The alternative the channel end has to be added to.
 * @return The index of the added channel_end_generic_t.
 * @pre \li channel_end_generic != NULL
 		   \li alternative != NULL
 **/
size_t add_to_alternative(struct channel_end_generic_t* channel_end_generic, struct alternative_t* alternative);

/**
 * @brief Select the first channel end to be ready following the given order.
 * @param alternative[in] The alternative to be parsed.
 * @return The index of the selected channel end.
 * @pre \li alternative != NULL
 * @post \li The channel end at the returned index is ready.
 **/
size_t pri_select(struct alternative_t* alternative);
