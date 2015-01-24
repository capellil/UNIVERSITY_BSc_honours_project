#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "net2_node.h"

int main(int argc, char* argv[])
{
    if(!net2_node_init(2501))
    {
        printf("Node init ok.\n");
        struct net2_link_t my_link;
    
        struct sockaddr_in ip_address;
        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");                        
            if(!net2_node_connect(&my_link, ip_address.sin_addr.s_addr, atoi(argv[2])))
            {
                printf("Connection ok.\n");
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
