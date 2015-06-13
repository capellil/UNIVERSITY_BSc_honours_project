/**
 * @file console_process.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef CONSOLE_PROCESS
#define CONSOLE_PROCESS

#include "channel.h"

/**
 * @brief Console process structure.
 * 
 * A console process has a channel input it reads data from, it then displays it on the standard output (e.g : console). User can choose a prefix and a suffix to respectively prepend and append to the displayed message.
 * IMPORTANT : prefix and suffix must have the '\0' terminating character.
 **/
struct console_process
{
	/// Message to prepend.
	char* prefix;

	/// Message to append.
	char* suffix;

	/// Channel to read data from.
	struct channel_input_end_t* channel_input_end;

	/// Method to call to execute the process.
	void* (*run)(void*);
};

/**
 * @brief Creates a console process. 
 * @param prefix[in] - Message to prepend.
 * @param suffix[in] - Message to append.
 * @param channel_input_end[in] - The channel to read the data from.
 * @return \li A pointer on the created console process. 
		   \li NULL if it failed.
 **/
struct console_process* create_console_process(char* prefix, char* suffix, struct channel_input_end_t* channel_input_end);

#endif // CONSOLE_PROCESS INCLUDED
