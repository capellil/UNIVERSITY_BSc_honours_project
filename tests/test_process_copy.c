/**
 * @file test_process_copy.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_copy.h"
#include "../net2/net2_node.h"

/**
 * @brief Creates a copy process and connects it to the console, successor and prefix processes.
 * 
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : IP address of the console process.
 *     <li>Argument 2 : IP address of the prefix process.
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
    if(argc == 3)
    {
        printf("yes.\n");
	    struct sockaddr_in ip_address;

        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
            
            printf("Checking the second parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
            if(inet_pton(AF_INET, argv[2], &ip_address) > 0)
            {
                printf("yes.\n");
                char* address_of_console = argv[1];
                unsigned short port_of_console = 3003;
                unsigned int vcn_of_console = 303;
                
                char* address_of_prefix = argv[2];
                unsigned short port_of_prefix = 3000;
                unsigned int vcn_of_prefix = 300;
                
                unsigned short my_port = 3002;
                unsigned int my_vcn = 302;
    
                printf("Did the node initialisation succeed... ");
                if(!net2_node_init(my_port))
                {
                    printf("yes.\n");
                    struct net2_channel_output_t out1;
                    
                    printf("Did the first channel output creation succeed... ");
                    if(!net2_channel_output_create(&out1, address_of_console, port_of_console, vcn_of_console))
                    {
                        printf("yes.\n");
                        struct net2_channel_output_t out2;
                        
                        printf("Did the second channel output creation succeed... ");
                        if(!net2_channel_output_create(&out2, address_of_prefix, port_of_prefix, vcn_of_prefix))
                        {
                            printf("yes.\n");
                            struct net2_channel_input_t in;
                            
                            printf("Did the channel input creation succeed... ");
                            if(!net2_channel_input_create(&in, my_vcn))
                            {
                                printf("yes.\n");
                                struct process_copy_int_t copy;
                                process_copy_int_create(&copy, &in, &out1, &out2);
                                
                                copy._run(&copy);
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
