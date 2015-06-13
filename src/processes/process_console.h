/**
 * @file process_console.h
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 09/02/2015
 **/
#include "../net2/net2_channel.h"

/**
 * @class process_console_int_t
 * @brief This process is in charge of consuming received messages and display them on the standard output.
 **/
struct process_console_int_t
{
    struct net2_channel_input_t* _in; ///< The channel to read from
    void* (*_run)(void*); ///< Function to execute the process
};

/**
 * @brief Creates a console process.
 * @param process_console_int A pointer on an allocated memory area to store the created process.
 * @param channel_input The channel to read from.
 * @pre process_console_int != NULL
 **/
void process_console_int_create(struct process_console_int_t* process_console_int, struct net2_channel_input_t* channel_input);
