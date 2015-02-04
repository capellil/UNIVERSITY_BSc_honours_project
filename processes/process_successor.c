#include <stdbool.h> // stdbool

#include "process_successor.h"

static void* run(void* generic_process)
{
    struct process_successor_int_t* process_successor_int = (struct process_successor_int_t*)generic_process;
    bool condition = true;
    
    while(condition)
    {
        condition = (!net2_channel_input_read_integer(process_successor_int->_in, &(process_successor_int->_value)));
        if(condition)
        {
            process_successor_int->_value++;
            condition = (!net2_channel_output_write_integer(process_successor_int->_out, process_successor_int->_value));
        }
    }
    
    return NULL;
}

void process_successor_int_create(struct process_successor_int_t* process_successor_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output)
{    
    process_successor_int->_in = channel_input;
    process_successor_int->_out = channel_output;
    process_successor_int->_run = &run;
}
