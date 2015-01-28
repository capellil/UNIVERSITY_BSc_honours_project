#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <stdbool.h> // bool

#include "net2_node.h"
#include "net2_link_manager.h"

/**
 * @brief Inits the node on the given port and connects to the node at the given address and port.
 *
 * Argument 1 : The port to use for the local node.
 * Argument 2 : The IP address of the remote node.
 * Argument 3 : The port of the remote node.
 **/
int main(int argc, char* argv[])
{
    if(!net2_node_init(atoi(argv[1])))
    {
        printf("Node init ok.\n");
        struct net2_link_t* my_link = NULL;
    
        struct in_addr ip_address;
        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[2]);
        if(inet_pton(AF_INET, argv[2], &ip_address) > 0)
        {
            printf("yes.\n");                        
            if(!net2_node_connect(&my_link, htonl(ip_address.s_addr), atoi(argv[3])))
            {
                printf("Connection ok.\n");
                bool found = false;
                if(!net2_link_manager_check_address_and_port(htonl(ip_address.s_addr), atoi(argv[3]), &found))
                {
                    if(found)
                        printf("A link to the given node is already established.\n");
                }
            }
            else
            {
                printf("Connection failed.\n");
            }
        }
        else
        {
            printf("no.\n");
        }
    }
    else
    {
        printf("Node init failed.\n");
    }
    
    return EXIT_SUCCESS;
}
