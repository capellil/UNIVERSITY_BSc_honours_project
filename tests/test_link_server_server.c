/**
 * @file test_link_server_server.c
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // atoi | NULL
#include <pthread.h> // pthread_join

#include "../net2/net2_link_server.h"
#include "../net2/net2_link_manager.h"

/**
 * @brief Runs a link server on the given port and accepts every incoming connections.
 **/
int main(int argc, char* argv[])
{
    int result = 0;
    
    // Init the link manager.
    result = net2_link_manager_init();
    
    printf("Checking if the link manager initialisation succeeded...");
    if(!result)
    {
        printf("yes.\n");
    
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
