/**
 * @file delayer_process.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef DELAYER_INCLUDED
#define DELAYER_INCLUDED

#include "channel.h"

/**
 * @brief Delays data transmission from the given channel input to the given channel output.
 **/
struct delayer_process
{
	/// The delay to assign to the data transmission (in microseconds);
	unsigned int delay;

	/// The channel to read data from.
	struct channel_input_end_t * channel_input_end;

	/// The channel to write data to.
	struct channel_output_end_t* channel_output_end;

	/// The function to call to execute the process.
	void* (*run)(void*);
};

/**
 * @brief Creates a delayer process.
 * @param delay[in] - The delay to assign to the data transmission in microseconds.
 * @param channel_input_end[in] - The channel to read data from.
 * @param channel_output_end[in] - The channel to write data to.
 * @return \li A pointer on the created delayer process. 
		   \li NULL if it failed.
 **/
struct delayer_process* create_delayer_process(unsigned int delay, struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end);

#endif // DELAYER_INCLUDED
