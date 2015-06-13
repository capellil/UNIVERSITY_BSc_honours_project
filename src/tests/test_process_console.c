/**
 * @file test_process_console.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/
 
#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_console.h"
#include "../net2/net2_node.h"

/**
 * @brief Creates a console process and connects it to the copy process.
 * @return <ul>
 *             <li>Success : EXIT_SUCCESS
 *             <li>Failure : EXIT_FAILURE
 *         </ul>
 **/
int main(int argc, char* argv[])
{   
    int result = EXIT_SUCCESS;
    
    unsigned short my_port = 3003;
    unsigned int my_vcn = 303;
    
    printf("Did the node initialisation succeed... ");
    if(!net2_node_init(my_port))
    {
        printf("yes.\n");
        struct net2_channel_input_t in;
        
        printf("Did the channel input creation succeed... ");
        if(!net2_channel_input_create(&in, my_vcn))
        {
            printf("yes.\n");
            struct process_console_int_t console;
            process_console_int_create(&console, &in);
            
            console._run(&console);
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
