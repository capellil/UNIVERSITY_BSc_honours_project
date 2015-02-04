#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_copy.h"
#include "../net2/net2_node.h"

int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    
    char* address_of_console = "127.0.0.1";
    unsigned short port_of_console = 3003;
    unsigned int vcn_of_console = 303;
    
    char* address_of_prefix = "127.0.0.1";
    unsigned short port_of_prefix = 3000;
    unsigned int vcn_of_prefix = 300;
    
    unsigned short my_port = 3002;
    unsigned int my_vcn = 302;
    
    if(!net2_node_init(my_port))
    {
        struct net2_channel_output_t out1;
        if(!net2_channel_output_create(&out1, address_of_console, port_of_console, vcn_of_console))
        {
            printf("Channel creation 1 ok\n");
            struct net2_channel_output_t out2;
            if(!net2_channel_output_create(&out2, address_of_prefix, port_of_prefix, vcn_of_prefix))
            {
                printf("Channel creation 2 ok\n");
                struct net2_channel_input_t in;
                if(!net2_channel_input_create(&in, my_vcn))
                {
                    struct process_copy_int_t copy;
                    process_copy_int_create(&copy, &in, &out1, &out2);
                    
                    copy._run(&copy);
                }
                else
                {
                }
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
