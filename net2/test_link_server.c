#include <stdio.h>

#include "link.h"

int main(int argc, char* argv[])
{
    struct net2_link_t link;

    if(!net2_create_server_link(&link, 3001))
    {
        printf("Connection succeeded.\n");
    }
    else
    {
        perror("Connection");
    }

    return 0;
}
