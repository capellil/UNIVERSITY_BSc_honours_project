#include <stdio.h> // printf
#include <stdlib.h> // atoi | NULL
#include <pthread.h> // pthread_join

#include "net2_link_server.h"

/**
 * @brief Runs a link server on the given port and accepts every incoming connections.
 **/
int main(int argc, char* argv[])
{
    int result = 0;
    
    // Init the link server.
    result = net2_link_server_init(atoi(argv[1]));
    
    printf("Checking if the link server has been correctly instanced...");
    if(!result)
    {
        printf("yes.\n");
        pthread_join(*((*net2_link_server_get_instance())->_server_thread), NULL);
    }
    else if(result == -1)
    {
        printf("no.\n");
    }
    else
    {
        printf("no because already instanced.\n");
    }    
    
    return result;
}
