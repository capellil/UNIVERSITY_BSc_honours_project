#include <stdbool.h> // stdbool

#include "process_copy.h"

struct copy_int_and_parameter_t
{
    struct net2_channel_output_t* _out;
    int* _parameter;
    bool* _return_value;
};

static void* start_a_copy_process(void* generic_copy_int_and_parameter)
{
    struct copy_int_and_parameter_t* temp = (struct copy_int_and_parameter_t*)generic_copy_int_and_parameter;
    
    *(temp->_return_value) = (!net2_channel_output_write_integer(temp->_out, *(temp->_parameter)));
    
    return NULL;
}

static void* run(void* generic_process)
{
    struct process_copy_int_t* process_copy_int = (struct process_copy_int_t*)generic_process;
    bool condition1 = true;
    bool condition2 = true;
    int value;
    
    struct copy_int_and_parameter_t copy_int_and_parameter1;
    copy_int_and_parameter1._out = process_copy_int->_out1;
    copy_int_and_parameter1._parameter = &value;
    copy_int_and_parameter1._return_value = &condition1;
    
    struct copy_int_and_parameter_t copy_int_and_parameter2;
    copy_int_and_parameter2._out = process_copy_int->_out2;
    copy_int_and_parameter2._parameter = &value;
    copy_int_and_parameter2._return_value = &condition2;
    
    pthread_t copy1;
    pthread_t copy2;
    
    while(condition1 && condition2)
    {
        condition1 = (!net2_channel_input_read_integer(process_copy_int->_in, &value));
        if(condition1)
        {
            pthread_create(&copy1, NULL, &start_a_copy_process, &copy_int_and_parameter1); 
            pthread_create(&copy2, NULL, &start_a_copy_process, &copy_int_and_parameter2); 
            pthread_join(copy1, NULL);
            pthread_join(copy2, NULL);
        }
    }
    
    return NULL;
}

void process_copy_int_create(struct process_copy_int_t* process_copy_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output1, struct net2_channel_output_t* channel_output2)
{    
    process_copy_int->_in = channel_input;
    process_copy_int->_out1 = channel_output1;
    process_copy_int->_out2 = channel_output2;
    process_copy_int->_run = &run;
}
