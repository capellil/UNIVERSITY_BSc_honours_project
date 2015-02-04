#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_prefix.h"
#include "../net2/net2_node.h"

int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    
    char* address_of_successor = "127.0.0.1";
    unsigned short port_of_successor = 3001;
    unsigned int vcn_of_successor = 301;
    
    unsigned short my_port = 3000;
    unsigned int my_vcn = 300;
    
    if(!net2_node_init(my_port))
    {
        struct net2_channel_input_t in;
        if(!net2_channel_input_create(&in, my_vcn))
        {
            struct net2_channel_output_t out;
            if(!net2_channel_output_create(&out, address_of_successor, port_of_successor, vcn_of_successor))
            {
                struct process_prefix_int_t prefix;
                process_prefix_int_create(&prefix, 0, &in, &out);
                
                prefix._run(&prefix);
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
