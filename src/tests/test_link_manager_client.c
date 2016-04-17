/**
 * @file test_link_manager_client.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 22/01/2015
 **/
 
#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../net2/net2_link_manager.h"

/**
 * @brief Creates a link to the server. Then it get the instance of the link manager and performs verifications to ensure the link manager functions are operational.
 *
 * <ul>
 *     <li>Argument 0 : the name of the application. (hidden)
 *     <li>Argument 1 : the server IP address.
 *     <li>Argument 2 : the server port.
 * </ul>
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{
    int programme_return = EXIT_SUCCESS;
    
    printf("Checks the number of parameters. Is there the right number of parameters...");
    if(argc == 3)
    {
        printf("yes.\n");
        struct sockaddr_in ip_address;
        
        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
            
            printf("Checking the port number : is %d a valid port number...", atoi(argv[2]));
            if(atoi(argv[2]) >= 0 && atoi(argv[2]) < 65536)
            {
                printf("yes.\n");
                
                printf("Did the link manager initialisation succeed...");
                if(!net2_link_manager_init())
                {
                    printf("yes.\n");
                    bool found = false;
                    
                    printf("Calls the link manager method to check if an address and port are already used by a link. Did the function succeed...");
                    if(!net2_link_manager_check_address_and_port(ip_address.sin_addr.s_addr, atoi(argv[2]), &found))
                    {
                        printf("yes.\n");                
                        struct net2_socket_t socket;
                        
                        printf("Checks if an occurence has been found. Is this address and port available so far...");
                        if(!found)
                        {
                            printf("yes.\n");
                            
                            printf("Checks the socket creation & connection. Did the socket creation & connection succeed...");
                            if(net2_create_and_connect_socket(&socket, ip_address.sin_addr.s_addr, atoi(argv[2])))
                            {
                                printf("yes.\n");
                                struct net2_link_t link;
                                net2_link_create(&link, &socket);
                                
                                printf("Calls the link manager method to check if an address and port are already used by a link. Did the function succeed...");
                                if(!net2_link_manager_check_address_and_port(ip_address.sin_addr.s_addr, atoi(argv[2]), &found))
                                {
                                    printf("yes.\n");      
                                              
                                    printf("Checks if an occurence has been found. Is this address and port busy...");
                                    if(!found)
                                    {
                                        printf("yes.\n");
                                    }
                                    else
                                    {
                                        printf("no.\n");
                                        programme_return = EXIT_FAILURE;
                                    }
                                }
                                else
                                {
                                    printf("no.\n");
                                    programme_return = EXIT_FAILURE;
                                }
                            }
                            else
                            {
                                printf("no.\n");
                                programme_return = EXIT_FAILURE;
                            }
                        }
                        else
                        {
                            printf("no.\n");
                            programme_return = EXIT_FAILURE;
                        }
                    }
                    else
                    {
                        printf("no.\n");
                        programme_return = EXIT_FAILURE;
                    }
                }
                else
                {
                    printf("no.\n");
                    programme_return = EXIT_FAILURE;
                }
            }
            else
            {
                printf("no.\n");
                programme_return = EXIT_FAILURE;
            }
        }
        else
        {
            printf("no.\n");
            programme_return = EXIT_FAILURE;
        }
    }
    else
    {
        printf("no.\n");
        programme_return = EXIT_FAILURE;
    }

    return programme_return;
}
