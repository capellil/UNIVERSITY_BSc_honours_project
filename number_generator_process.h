/**
 * @file number_generator_process.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef NUMBER_GENERATOR
#define NUMBER_GENERATOR

#include "channel.h"

/**
 * @brief Generates number from the given min value to the given max value, the generated numbers will be incremented by the given incrementation and written to the given channel output.
 **/
struct number_generator_process
{
	/// The value to start the generation from
	int min;

	/// The value to stop the generation at
	int max;

	/// The incrementation
	int incrementation;

	/// The channel output
	struct channel_output_end_t* channel_output_end;
	
	/// The function called to execute the process
	void* (*run)(void*);
};

/**
 * @brief Creates the number generator process.
 * @param min[in] - The value to start the generation from
 * @param max[in] - The value to stop the generation at
 * @param incrementation[in] - The value to add each time
 * @param channel_output_end[in] - The channel output to generate the numbers on
 * @return \li A pointer on the created number generator process. 
		   \li NULL if it failed.
 **/
struct number_generator_process* create_number_generator_process(int min, int max, int incrementation, struct channel_output_end_t* channel_output_end);

#endif // NUMBER_GENERATOR INCLUDED
