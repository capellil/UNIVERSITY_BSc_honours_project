#include <stdio.h> // printf

#include "process_console.h"

static void* run(void* generic_process_console_int)
{
    struct process_console_int_t* temp = (struct process_console_int_t*)generic_process_console_int;
    
    int value;
    
    while(!(net2_channel_input_read_integer(temp->_in, &value)))
    {
        printf("Integer received : %d.\n", value);
    }
    
    return NULL;
}

void process_console_int_create(struct process_console_int_t* process_console_int, struct net2_channel_input_t* channel_input)
{   
    process_console_int->_in = channel_input;
    process_console_int->_run = &run;
}
