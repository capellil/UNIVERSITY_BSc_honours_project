#include <stdlib.h> // EXIT_SUCCESS
#include <stdio.h> // printf

#include "net2_node.h"
#include "net2_channel.h"

int main(int argc, char* argv[])
{
    int programme_return = EXIT_SUCCESS;
    char ip_address[] = "127.0.0.1";
    unsigned short port = 2600;
    unsigned short remote_port = 2500;
    unsigned int remote_channel_input_number = 100;

    printf("Did the node initialisation succeed...");
    if(!net2_node_init(port))
    {
        printf("yes.\n");
        struct net2_channel_output_t channel;
        
        printf("Did the channel output creation succeed on remote channel %d...", remote_channel_input_number);
        if(!net2_channel_output_create(&channel, ip_address, remote_port, remote_channel_input_number))
        {
            printf("yes.\n");
            int value = 10;
            printf("Did the integer writing succeed...");
            if(!net2_channel_output_write_integer(&channel, value))
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
