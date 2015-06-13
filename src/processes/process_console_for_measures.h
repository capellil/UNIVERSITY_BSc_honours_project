/**
 * @file process_console_for_measures.h
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 09/02/2015
 **/

#include "../net2/net2_channel.h"

/**
 * @class process_console_for_measures_int_t
 * @brief This process is in charge of acting as a regular console but with some measurement functionality included.
 * 
 * It reads a given number of times and displays the elpased time, it is repeated a number of time.
 **/
struct process_console_for_measures_int_t
{
    unsigned int _number_of_iterations; ///< The number of integers read per cycle
    unsigned int _number_of_repetitions; ///< The number of cycles
    struct net2_channel_input_t* _in; ///< The channel to read from
    void* (*_run)(void*); ///< Function to execute the process
};

/**
 * @brief Creates a console process.
 * @param number_of_iterations The number of iterations.
 * @param number_of_repetitions The number of repetitions.
 * @param process_console_for_measures_int A pointer on an allocated memory area to store the created process.
 * @param channel_input The channel to read from.
 * @pre process_console_for_measures_int != NULL
 **/
void process_console_for_measures_int_create(unsigned int number_of_iterations, unsigned int number_of_repetitions, struct process_console_for_measures_int_t* process_console_for_measures_int, struct net2_channel_input_t* channel_input);
