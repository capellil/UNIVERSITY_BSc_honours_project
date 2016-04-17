/**
 * @file test_link_server_client.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 22/01/2015
 **/
 
#include <stdio.h> // printf
#include <stdlib.h> // atoi
#include <stdbool.h> // bool

#include "../net2/net2_link_server.h"
#include "../net2/net2_link_manager.h"

/**
 * @brief Sets up a link server and connects to the remote link server.
 *
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : IP address of the remote link server.
 *     <li>Argument 2 : port of the remote link server.
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
        
        printf("Did the link manager initialisation succeed... ");
        if(!net2_link_manager_init())
        {
            printf("yes.\n");
            bool found = false;
                
            printf("Did the link manager research succeed...");
            if(!net2_link_manager_check_address_and_port(htonl(ip_address.s_addr), atoi(argv[2]), &found))
            {
                printf("yes.\n");
                printf("Is the node at the given \"%s\" + %d available...", argv[1], atoi(argv[2]));
                if(!found)
                {
                    printf("yes.\n");
                    struct net2_socket_t socket;
                    
                    printf("Did the socket creation & connection succeed...");
                    if(net2_create_and_connect_socket(&socket, htonl(ip_address.s_addr), atoi(argv[2])))
                    {
                        printf("yes.\n");
                        signed char data;
                        unsigned int data_length = sizeof(signed char);
                        
                        printf("Did the socket read succeed...");
                        if(net2_read_from_socket(&socket, (void*)&data, data_length) >= 0)
                        {
                            printf("yes.\n");
                            printf("Has the connection been allowed...");
                            if(!data)
                            {
                                printf("yes.\n");
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
