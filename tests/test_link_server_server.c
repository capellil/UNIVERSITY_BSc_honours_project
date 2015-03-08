/**
 * @file test_link_server_server.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 22/01/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // atoi | NULL | EXIT_FAILURE | EXIT_SUCCESS
#include <pthread.h> // pthread_join

#include "../net2/net2_link_server.h"
#include "../net2/net2_link_manager.h"

/**
 * @brief Runs a link server on the given port and accepts every incoming connections.
 *
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : port to start the link server on.
 * </ul>
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{
    int result = EXIT_SUCCESS;
    
    printf("Did the link manager initialisation succeeded... ");
    if(!net2_link_manager_init())
    {
        printf("yes.\n");
        
        printf("Has the link server been correctly instanced... ");
        if(!net2_link_server_init(atoi(argv[1])))
        {
            printf("yes.\n");
            pthread_join(*((*net2_link_server_get_instance())->_server_thread), NULL);
        }
        else if(result == -1)
        {
            printf("no.\n");
            result = EXIT_FAILURE;
        }
        else
        {
            printf("no because already instanced.\n");
            result = EXIT_FAILURE;
        }    
    }
    else if(result == -1)
    {
        printf("no.\n");
        result = EXIT_FAILURE;
    }
    else
    {
        printf("no because already instanced.\n");
        result = EXIT_FAILURE;
    } 
    
    return result;
}
