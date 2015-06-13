/**
 * @file process_copy.h
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 09/02/2015
 **/
#include "../net2/net2_channel.h"

/**
 * @class process_copy_int_t
 * @brief Reads data from its channel input and copies it in parallel to its two channel ouptuts.
 **/
struct process_copy_int_t
{
    struct net2_channel_input_t* _in; ///< Channel where data is read from.
    struct net2_channel_output_t* _out1; ///< First output channel to copy data to.
    struct net2_channel_output_t* _out2; ///< Second output channel to copy data to.
    void* (*_run)(void*); ///< Function to execute the process.
};

/**
 * @brief Creates a copy process.
 * @param process_copy_int A pointer on an allocated memory area to store the created process.
 * @param channel_input The channel input to read data from.
 * @param channel_output1 The first output channel to copy data to.
 * @param channel_output2 The second output channel to copy data to.
 * @pre process_copy_int !- NULL
 **/
void process_copy_int_create(struct process_copy_int_t* process_copy_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output1, struct net2_channel_output_t* channel_output2);
