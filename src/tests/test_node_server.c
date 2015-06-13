/**
 * @file test_node_server.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <pthread.h> // pthread_join

#include "../net2/net2_node.h"
#include "../net2/net2_link_server.h"

/**
 * @brief Inits the node on the given port.
 * 
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : the port to use.
 * </ul>
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{
    int result = EXIT_SUCCESS;
    
    printf("Is there the right number of paramters... ");
    if(argc == 2)
    {
        printf("Did the node initialisation succeed... ");
        if(!net2_node_init(atoi(argv[1])))
        {
            printf("yes.\n");
            pthread_join(*((*net2_link_server_get_instance())->_server_thread), NULL);
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
