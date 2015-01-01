#include <stdio.h>

#include "net2_link.h"

int main(int argc, char* argv[])
{
    struct net2_link_t link;

    if(!net2_create_client_link(&link, 3002, 3232235851, 3001))
    {
        printf("Connection succeeded.\n");
    }
    else
    {
        perror("Connection");
    }

    return 0;
}
