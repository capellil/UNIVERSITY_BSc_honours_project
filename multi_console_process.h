/**
 * @file multi_console_process.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 26/09/2014
 **/

#include "alternative.h"

/**
 * @brief Displays on the console what it receives from several channel input end;
 **/
struct multi_console_process
{
	/// Message to prepend.
	char* prefix;

	/// Message to append.
	char* suffix;
	
	/// The alternative on the channel input ends.
	struct alternative_t* alternative;

	/// Method that will execute the process
	void* (*run)(void*);
};

/**
 * @brief Creates a multi console process containing an alternative with initial_size channel ends.
 * @param prefix[in] - Message to prepend.
 * @param suffix[in] - Message to append.
 * @param initial_size[in] The length to be assigned to the alternative channel ends array.
 * @return The created multi console process. NULL if it failed.
 **/
struct multi_console_process* create_multi_console_process(char* prefix, char* suffix, size_t initial_alternative_size);
