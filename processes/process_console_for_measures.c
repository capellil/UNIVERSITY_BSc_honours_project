#include <stdio.h> // printf
#include <sys/time.h>

#include "process_console_for_measures.h"

static void* run(void* generic_process_console_for_measures_int)
{
    struct process_console_for_measures_int_t* temp = (struct process_console_for_measures_int_t*)generic_process_console_for_measures_int;
    
    struct timeval tval_before, tval_after;

    unsigned int i = 0, j = 0;
    int value;
 
    net2_channel_input_read_integer(temp->_in, &value);

    while(j < temp->_number_of_repetitions)
    {
        gettimeofday(&tval_before, NULL);
        i = 0;

        while(i < temp->_number_of_iterations && !(net2_channel_input_read_integer(temp->_in, &value)))
        {
            i++;
        }

        gettimeofday(&tval_after, NULL);

        long int tval_result = tval_after.tv_sec * 1000000 + tval_after.tv_usec - tval_before.tv_sec * 1000000 - tval_before.tv_usec;

        printf("Time elapsed: %ld\n", (long int)tval_result);
        j++;
    }
    
    return NULL;
}

void process_console_for_measures_int_create(unsigned int number_of_iterations, unsigned int number_of_repetitions, struct process_console_for_measures_int_t* process_console_for_measures_int, struct net2_channel_input_t* channel_input)
{   
    process_console_for_measures_int->_number_of_iterations = number_of_iterations;
    process_console_for_measures_int->_number_of_repetitions = number_of_repetitions;
    process_console_for_measures_int->_in = channel_input;
    process_console_for_measures_int->_run = &run;
}
