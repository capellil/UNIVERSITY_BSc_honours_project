/**
 * @file successor_process.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 02/10/2014
 **/

#ifndef SUCCESSOR_PROCESS
#define SUCCESSOR_PROCESS

#include "channel.h"

/**
 * @brief Adds 1 to what it reads and sends it to the channel_output_end.
 **/
struct successor_process
{
	/// The channel to read data from
	struct channel_input_end_t* channel_input_end;

	/// The channel to write data to
	struct channel_output_end_t* channel_output_end;

	/// The method to be called in order to execute the process
	void* (*run)(void*);
};

/**
 * @brief Creates a successor process and returns a pointer on the created prefix process.
 * @param channel_input_end[in] The channel to read data from
 * @param channel_output_end[out] The channel to write data to
 * @return A pointer on the created successor process. NULL if it failed.
 * @pre \li channel_input_end != NULL
	\li channel_output_end != NULL
 **/
struct successor_process* create_successor_process(struct channel_input_end_t* channel_input, struct channel_output_end_t* channel_output_end);

#endif // SUCCESSOR_PROCESS INCLUDED
