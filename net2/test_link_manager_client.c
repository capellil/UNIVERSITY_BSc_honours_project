#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "net2_link_manager.h"

int main(int argc, char* argv[])
{
    struct net2_link_manager_t* link_manager = NULL;
    
    // TEST : Did the link manager get instance succeed ?
    if(!net2_link_manager_get_instance(&link_manager))
    {
        // Yes, the link manager get instance succeed.
        printf("Link manager ok.\n");
        
        struct net2_link_t link;
        
        // TEST : Did the server link creation succeed ?
        if(net2_create_client_link(&link, 3000, 1844854094, 3001) >= 0)
        {
            // Yes, the server link has been correctly created.
            printf("The connection has been created.");
        }
        else
        {
            // No, the server link creation failed.
            printf("The connection has failed.");
        }
    }
    else
    {
        // No, the link manager get instance failed.
        printf("Link manager get instance failed.\n");
    }

    return EXIT_SUCCESS;
}
