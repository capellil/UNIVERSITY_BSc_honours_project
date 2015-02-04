#include "process_prefix.h"

static void* run(void* generic_process)
{
    struct process_prefix_int_t* process_prefix_int = (struct process_prefix_int_t*)generic_process;
    
    if(!net2_channel_output_write_integer(process_prefix_int->_out, process_prefix_int->_value))
    {
        while(!net2_channel_input_read_integer(process_prefix_int->_in, &(process_prefix_int->_value)) && !net2_channel_output_write_integer(process_prefix_int->_out, process_prefix_int->_value))
        {
        }
    }
    
    return NULL;
}

void process_prefix_int_create(struct process_prefix_int_t* process_prefix_int, int value, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output)
{    
    process_prefix_int->_value = value;
    process_prefix_int->_in = channel_input;
    process_prefix_int->_out = channel_output;
    process_prefix_int->_run = &run;
}
