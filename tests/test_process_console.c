#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_console.h"
#include "../net2/net2_node.h"

int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    
    unsigned short my_port = 3003;
    unsigned int my_vcn = 303;
    
    if(!net2_node_init(my_port))
    {
        struct net2_channel_input_t in;
        if(!net2_channel_input_create(&in, my_vcn))
        {
            struct process_console_int_t console;
            process_console_int_create(&console, &in);
            
            console._run(&console);
        }
        else
        {
        }
    }
    else
    {
    }
    

    return result;
}
