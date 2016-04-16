/**
 * @file test_process_prefix.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_prefix.h"
#include "../net2/net2_node.h"

/**
 * @brief Creates a prefix process and connects it to the successor process as well as opening a channel for the copy process.
 * 
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : IP address of the successor process.
 * </ul>
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    struct in_addr ip_address;
    
    printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
    if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
    {
        printf("yes.\n");
        char* address_of_successor = argv[1];
        unsigned short port_of_successor = 3001;
        unsigned int vcn_of_successor = 301;
        
        unsigned short my_port = 3000;
        unsigned int my_vcn = 300;
        
        printf("Did the node initialisation succeed... ");
        if(!net2_node_init(my_port))
        {
            printf("yes.\n");
            struct net2_channel_input_t in;

            printf("Did the channel input creation succeed... ");
            if(!net2_channel_input_create(&in, my_vcn))
            {
                printf("yes.\n");
                struct net2_channel_output_t out;

                printf("Did the channel output creation succeed... ");
                if(!net2_channel_output_create(&out, address_of_successor, port_of_successor, vcn_of_successor))
                {
                    printf("yes.\n");
                    struct process_prefix_int_t prefix;
                    process_prefix_int_create(&prefix, 0, &in, &out);
                    
                    prefix._run(&prefix);
                }
                else
                {
                    printf("no.\n");
                    result = EXIT_FAILURE;
                }
            }
            else
            {
                printf("no.\n");
                result = EXIT_FAILURE;
            }
        }
        else
        {
            printf("no.\n");
            result = EXIT_FAILURE;
        }
    }
    else
    {
        printf("no.\n");
        result = EXIT_FAILURE;
    }

    return result;
}
