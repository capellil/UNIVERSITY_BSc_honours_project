#include "../net2/net2_channel.h"

struct process_successor_int_t
{
    int _value;
    struct net2_channel_input_t* _in;
    struct net2_channel_output_t* _out;
    void* (*_run)(void*);
};

void process_successor_int_create(struct process_successor_int_t* process_successor_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output);
