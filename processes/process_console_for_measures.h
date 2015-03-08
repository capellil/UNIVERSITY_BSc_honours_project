#include "../net2/net2_channel.h"

struct process_console_for_measures_int_t
{
    unsigned int _number_of_iterations;
    unsigned int _number_of_repetitions;
    struct net2_channel_input_t* _in;
    void* (*_run)(void*);
};

void process_console_for_measures_int_create(unsigned int number_of_iterations, unsigned int number_of_repetitions, struct process_console_for_measures_int_t* process_console_for_measures_int, struct net2_channel_input_t* channel_input);
