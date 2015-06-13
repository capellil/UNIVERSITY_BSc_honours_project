/**
 * @file test_process_successor.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_successor.h"
#include "../net2/net2_node.h"

/**
 * @brief Creates a successor process and connects it to a prefix and a copy proccesses.
 * 
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : copy process IP address.
 * </ul>
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS 
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;

    printf("Is there the right number of parameters... ");
    if(argc == 2)
    {
        printf("yes.\n");
        struct sockaddr_in ip_address;

        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
        
            char* address_of_copy = argv[1];
            unsigned short port_of_copy = 3002;
            unsigned int vcn_of_copy = 302;
            
            unsigned short my_port = 3001;
            unsigned int my_vcn = 301;
            
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
                    if(!net2_channel_output_create(&out, address_of_copy, port_of_copy, vcn_of_copy))
                    {
                        printf("yes.\n");
                        struct process_successor_int_t successor;
                        process_successor_int_create(&successor, &in, &out);
                        
                        successor._run(&successor);
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
    }
    else
    {
        printf("no.\n");
        result = EXIT_FAILURE;
    }

    return result;
}
