/**
 * @file process_successor.h
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 09/02/2015
 **/
#include "../net2/net2_channel.h"

/**
 * @class process_successor_int_t
 * @brief It reads integers from its channel input, increments them and send them on its channel output.
 **/
struct process_successor_int_t
{
    int _value; ///< Variable to temporarily store the value to increment.
    struct net2_channel_input_t* _in; ///< The channel to read data from.
    struct net2_channel_output_t* _out; ///< The channel to send data to.
    void* (*_run)(void*); ///< Function to execute the process.
};

/**
 * @brief Creates a successor process.
 * @param process_successor_int A pointer on an allocated memory area to store the created process in.
 * @param channel_input The channel to read data from.
 * @param channel_output The channel to send data to.
 * @pre process_successor != NULL
 **/
void process_successor_int_create(struct process_successor_int_t* process_successor_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output);
