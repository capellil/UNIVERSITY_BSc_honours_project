#include <stdlib.h> // EXIT_SUCCESS
#include <stdio.h> // printf

#include "net2_node.h"
#include "net2_channel.h"

int main(int argc, char* argv[])
{
    int programme_return = EXIT_SUCCESS;
    unsigned short port = 2500;
    unsigned int channel_input_number = 100;

    printf("Did the node initialisation succeed...");
    if(!net2_node_init(port))
    {
        printf("yes.\n");
        struct net2_channel_input_t channel;
        
        printf("Did the channel input creation on channel %d succeed...", channel_input_number);
        if(!net2_channel_input_create(&channel, channel_input_number))
        {
            printf("yes.\n");
            int value = 0;
            printf("Did the integer reading succeed...");
            if(!net2_channel_input_read_integer(&channel, &value))
            {
                printf("yes.\n");
                net2_node_close();
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
