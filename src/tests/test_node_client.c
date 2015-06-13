/**
 * @file test_node_client.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <stdbool.h> // bool

#include "../net2/net2_node.h"
#include "../net2/net2_link_manager.h"

/**
 * @brief Inits the node on the given port and connects to the node at the given address and port.
 *
 * <ul>
 *     <li>Argument 1 : The port to use for the local node.
 *     <li>Argument 2 : The IP address of the remote node.
 *     <li>Argument 3 : The port of the remote node.
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
    if(argc == 4)
    {
        printf("yes.\n");
        
        printf("Did the node initialisation succeed... ");
        if(!net2_node_init(atoi(argv[1])))
        {
            printf("yes.\n");
            struct net2_link_t* my_link = NULL;
            struct in_addr ip_address;
            
            printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[2]);
            if(inet_pton(AF_INET, argv[2], &ip_address) > 0)
            {
                printf("yes.\n");   
                 
                printf("Did the node connection succeed... ");                    
                if(!net2_node_connect(&my_link, htonl(ip_address.s_addr), atoi(argv[3])))
                {
                    printf("yes.\n");
                    bool found = false;
                    
                    printf("Did the checking of the address and port in the link manager succeed... ");
                    if(!net2_link_manager_check_address_and_port(htonl(ip_address.s_addr), atoi(argv[3]), &found))
                    {
                        printf("yes.\n");
                        
                        printf("Has the link been found... ");
                        if(found)
                        {
                            printf("yes\n");
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
