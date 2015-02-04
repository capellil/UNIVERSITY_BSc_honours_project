#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_successor.h"
#include "../net2/net2_node.h"

int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    
    char* address_of_copy = "127.0.0.1";
    unsigned short port_of_copy = 3002;
    unsigned int vcn_of_copy = 302;
    
    unsigned short my_port = 3001;
    unsigned int my_vcn = 301;
    
    if(!net2_node_init(my_port))
    {
        struct net2_channel_input_t in;
        if(!net2_channel_input_create(&in, my_vcn))
        {
            struct net2_channel_output_t out;
            if(!net2_channel_output_create(&out, address_of_copy, port_of_copy, vcn_of_copy))
            {
                struct process_successor_int_t successor;
                process_successor_int_create(&successor, &in, &out);
                
                successor._run(&successor);
            }
            else
            {
            }
        }
    }
    else
    {
    }
    

    return result;
}
