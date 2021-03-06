/**
 * @file test_process_console_for_measures.c
 * @author Capelli, Ludovic
 * @version 1.0
 * @date 08/02/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS

#include "../processes/process_console_for_measures.h"
#include "../net2/net2_node.h"

/**
 * @brief Creates the console used for performance measurement.
 * 
 * <ul>
 *     <li>Argument 0 : the programme name. (hidden)
 *     <li>Argument 1 : number of iterations.
 *     <li>Argument 2 : number of repetitions.
 * </ul>
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
    
    printf("Is there the right number of parameters... ");
    if(argc == 3)
    {
        printf("Did the node initialisation succeed... ");
        if(!net2_node_init(my_port))
        {
            printf("yes.\n");
            struct net2_channel_input_t in;
            
            printf("Did the channel input creation succeed... ");
            if(!net2_channel_input_create(&in, my_vcn))
            {
                printf("yes.\n");
                struct process_console_for_measures_int_t console;
                process_console_for_measures_int_create(atoi(argv[1]), atoi(argv[2]), &console, &in);
                
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
    }
    else
    {
        printf("no.\n");
        result = EXIT_FAILURE;
    }
    
    return result;
}
