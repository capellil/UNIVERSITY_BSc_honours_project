/**
 * @file copy_process.h
 * @author Capelli Ludovic
 * @version _1.0
 * @date _15/09/_20_14
 **/

#ifndef COPIER_INCLUDED
#define COPIER_INCLUDED

#include "channel.h"

/**
 * @brief Copies what is given in the channel input to the two given channel outputs.
 * 
 * It reads data from the channel input and creates a two threads : each one will write the read data to one of the channel outputs. It will be done in parallel but it before reading some other data, it will wait the two threads to finish.
 **/
struct copy_process
{
	/// The channel to read the data from.
	struct channel_input_end_t* channel_input_end;

	/// One of the channels to copy the data to.
	struct channel_output_end_t* channel_output_end_1;
	
	/// The other channel to copy the data to.
	struct channel_output_end_t* channel_output_end_2;

	/// Method to call to execute the process.
	void* (*run)(void*);
};

/**
 * @brief Creates a copier process with the given channel input and channel outputs.
 * @param channel_input_end[in] - The channel input to read data from.
 * @param channel_output_end_1[in] - The channel output to write the data to.
 * @param channel_output_end_2[in] - The channel output to write the data to.
 * @return \li A pointer on the creater process. 
		   \li NULL if it failed.
 **/
struct copy_process* create_copy_process(struct channel_input_end_t* channel_input_end, struct channel_output_end_t* channel_output_end_1, struct channel_output_end_t* channel_output_end_2);

#endif // COPIER_INCLUDED
