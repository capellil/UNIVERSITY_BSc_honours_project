#include "../net2/net2_channel.h"

struct process_console_int_t
{
    struct net2_channel_input_t* _in;
    void* (*_run)(void*);
};

void process_console_int_create(struct process_console_int_t* process_console_int, struct net2_channel_input_t* channel_input);
