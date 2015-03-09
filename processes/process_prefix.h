/**
 * @file process_prefix.h
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 09/02/2015
 **/
#include "../net2/net2_channel.h"

/**
 * @class process_prefix_int_t
 * @brief A process sending an initial value, then it simply transmits data its receives from its channel input to its channel output.
 **/
struct process_prefix_int_t
{
    int _value; ///< Value to be initially sent.
    struct net2_channel_input_t* _in; ///< Channel to read data from.
    struct net2_channel_output_t* _out; ///< Channel to send data to.
    void* (*_run)(void*); ///< Function to execute the process.
};

/**
 * @brief Creates a prefix process.
 * @param process_prefix_int A pointer on an allocated memory area to store the created process in.
 * @param value Value to be initially sent.
 * @param channel_input The channel to read data from.
 * @param channel_output The channel to send data to.
 * @pre process_prefix != NULL
 **/
void process_prefix_int_create(struct process_prefix_int_t* process_prefix_int, int value, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output);
