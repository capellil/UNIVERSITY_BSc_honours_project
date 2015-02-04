#include "../net2/net2_channel.h"

struct process_copy_int_t
{
    struct net2_channel_input_t* _in;
    struct net2_channel_output_t* _out1;
    struct net2_channel_output_t* _out2;
    void* (*_run)(void*);
};

void process_copy_int_create(struct process_copy_int_t* process_copy_int, struct net2_channel_input_t* channel_input, struct net2_channel_output_t* channel_output1, struct net2_channel_output_t* channel_output2);
