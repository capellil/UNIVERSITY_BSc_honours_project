#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP

#include "net2_link.h"

int main(int argc, char* argv[])
{
    unsigned int result = EXIT_FAILURE;
    
    if(argc == 2)
    {
        struct sockaddr_in ip_address;
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            struct net2_link_t link;
    
            if(net2_create_client_link(&link, 3002, ip_address.sin_addr.s_addr, 3001) >= 0)
            {
                printf("Connection succeeded.\n");
                result = EXIT_SUCCESS;
            }
            else
            {
                perror("Connection");
            }
        }
        else
        {
            perror("IP address conversion from string to number");
        }
    }
    else
    {
        printf("HOW TO USE THIS PROGRAM : \"./%s a.b.c.d\" with \"a.b.c.d\" the IPv4 address to use.\n", argv[0]);
    }

    return result;
}
