/**
 * @file test_link_manager_server.c
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <stdbool.h> // bool

#include "net2_link_manager.h"

/**
 * @brief This application tests the link manager.
 *
 * It first consists in instanciating the link manager singleton through net2_link_manager_get_instance function.<br>
 * Then it creates a link, connected with the test_link_manager_client programme one. This application will check the functions net2_link_manager_check_socket and net2_link_manager_register_link before and after the link gets registered.
 * Argument 0 : the application name. (hidden)
 * Argument 1 : the port to use.
 **/
int main(int argc, char* argv[])
{
    int programme_return = EXIT_SUCCESS;
    printf("Checks the number of parameters. Is there the right number of parameters...");
    
    if(argc == 2)
    {
        printf("yes.\n");
        struct sockaddr_in ip_address;        
        printf("Checking the port number : is %d a valid port number...", atoi(argv[1]));
        
        if(atoi(argv[1]) >= 0 && atoi(argv[1]) < 65536)
        {
            printf("yes.\n");
            printf("Did the link manager initialisation succeed...");
            if(!net2_link_manager_init())
            {
                printf("yes.\n");
                bool found = false;
                printf("Calls the link manager method to check if an address and port are already used by a link. Did the function succeed...");
                
                if(!net2_link_manager_check_address_and_port(ip_address.sin_addr.s_addr, atoi(argv[1]), &found))
                {
                    printf("yes.\n");                
                    struct net2_socket_t socket;
                    printf("Checks if an occurence has been found. Is this address and port available so far...");
                    
                    if(!found)
                    {
                        printf("yes.\n");
                        printf("Checks the socket creation. Did the socket creation succeed...");
                        
                        if(!net2_socket_create_and_bind(&socket, atoi(argv[1])))
                        {
                            printf("yes.\n");
                            printf("Checks the socket listening. Did the socket listening succeed...");
                            
                            if(!net2_socket_listen(&socket))
                            {
                                printf("yes.\n");
                                printf("Tries to accept the client connection. Did the client connection acceptation succeed...");
                                struct net2_socket_t client_socket;
                                if(net2_socket_accept(&socket, &client_socket) >= 0)
                                {
                                    printf("yes.\n");
                                    struct net2_link_t link;
                                    net2_link_create(&link, &client_socket);
                                    printf("Calls the link manager method to check if an address and port are already used by a link. Did the function succeed...");
                                    
                                    if(!net2_link_manager_check_address_and_port(ip_address.sin_addr.s_addr, atoi(argv[1]), &found))
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
    }
    else
    {
        printf("no.\n");
        programme_return = EXIT_FAILURE;
    }

    return programme_return;
}
