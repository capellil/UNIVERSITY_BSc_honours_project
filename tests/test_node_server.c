#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <pthread.h> // pthread_join

#include "../net2/net2_node.h"
#include "../net2/net2_link_server.h"

/**
 * @brief Inits the node on the given port.
 * 
 * Argument 1 : the port to use.
 **/
int main(int argc, char* argv[])
{
    if(!net2_node_init(atoi(argv[1])))
    {
        printf("Node init ok.\n");
        pthread_join(*((*net2_link_server_get_instance())->_server_thread), NULL);
    }
    else
    {
        printf("Node init failed.\n");
    }
    
    return EXIT_SUCCESS;
}
