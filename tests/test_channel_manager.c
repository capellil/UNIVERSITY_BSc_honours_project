#include <stdio.h> // pritnf
#include <stdlib.h> // EXIT_SUCCESS

#include "../net2/net2_channel_manager.h"

int main(int argc, char* argv[])
{
    int programme_return = EXIT_SUCCESS;
    
    printf("Is there the right number of parameter...");
    if(argc == 2)
    {
        printf("yes.\n");
        printf("Did the channel manager initialisation succeed...");
        if(!net2_channel_manager_init())
        {
            printf("yes.\n");
            struct net2_channel_output_t channel_output;
            
            printf("Did the channel registration succeed...");
            if(!net2_channel_manager_register_channel_output(&channel_output))
            {
                printf("yes.\n");
                printf("Tries to register the same channel again...");
                if(net2_channel_manager_register_channel_output(&channel_output))
                {
                    printf("failed AS EXPECTED.\n");
                    struct net2_channel_input_t channel_input;
                    printf("Tries to register a INPUT channel with the same channel number...");
                    if(net2_channel_manager_register_channel_input(&channel_input, atoi(argv[1])))
                    {
                        printf("failed AS EXPECTED.\n");
                        printf("Tries to register a INPUT channel with a NEW channel number...");
                        if(!net2_channel_manager_register_channel_input(&channel_input, atoi(argv[1]) + 1))
                        {
                            printf("succeeded.\n");
                        }
                        else
                        {
                            printf("failed.\n");
                            programme_return = EXIT_FAILURE;
                        }
                    }
                    else
                    {
                        printf("succeed. SHOULD HAVE FAILED.\n");
                        programme_return = EXIT_FAILURE;
                    }
                }
                else
                {
                    printf("succeed. SHOULD HAVE FAILED.\n");
                    programme_return = EXIT_FAILURE;
                }
            }
            else
            {
                printf("no.\n");
                programme_return = EXIT_FAILURE;
            }
        }
        else
        {
            printf("no.\n");
            programme_return = EXIT_FAILURE;
        }
    }
    else
    {
        printf("no.\n");
        programme_return = EXIT_FAILURE;
    }

    return programme_return;
}
