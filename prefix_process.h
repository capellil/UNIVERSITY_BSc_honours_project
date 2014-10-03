/**
 * @file prefix_process.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 02/10/2014
 **/

#ifndef PREFIX_PROCESS
#define PREFIX_PROCESS

#include "channel.h"

/**
 * @brief Prefixes what it reads by a given value.
 * 
 * Sends the prefix value to the channel output end, then it just transfers what it receives from its channel input end to the channel output end it has.
 **/
struct prefix_process
{
	/// The channel to read data from
	struct channel_input_end_t* channel_input_end;

	/// The channel to write data to
	struct channel_output_end_t* channel_output_end;

	/// The value to be passed first
	int prefix_value;

	/// The method to be called in order to execute the process
	void* (*run)(void*);
};

/**
 * @brief Creates a prefix process and returns a pointer on the created prefix process.
 * @param prefix_Value[in] The char to be prefixed.
 * @param channel_input_end[in] The channel to read data from
 * @param channel_output_end[out] The channel to write data to
 * @return A pointer on the created prefix process. NULL if it failed.
 * @pre \li channel_input_end != NULL
	\li channel_output_end != NULL
 **/
struct prefix_process* create_prefix_process(int prefix_Value, struct channel_input_end_t* channel_input, struct channel_output_end_t* channel_output_end);

#endif // PREFIX_PROCESS INCLUDED
