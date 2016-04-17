/**
 * @file test_socket_client.c
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 18/01/2015
 **/

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <string.h> // strlen

#include "../net2/net2_socket.h"

/**
 * @brief Creates a socket and connects it to the given address and port.
 *
 * <ul>
 *     <li>The argument 0 : the name of the programme. (hidden from the user input)
 *     <li>The argument 1 : the IPv4 address to connect on.
 *     <li>The argument 2 : the port to connect on.
 * </ul>
 * @return <ul>
 		       <li>SUCCESS : EXIT_SUCCESS
 	           <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
    struct net2_socket_t my_socket;

    printf("Tries to create a client socket. Has the client socket creation succeeded...");
    if(net2_create_client_socket(argv[1], atoi(argv[2]), &my_socket) == 0)
	{
		printf("yes.\n");
		char data[13] = "Hello server\0";
        unsigned int data_length = 13;
        
        printf("Test the connection by sending \"%s\" to the server. Has the message been correctly sent...", data);
        if(net2_write_to_socket(&my_socket, (void*)data, data_length) != -1)
        {
            printf("yes.\n");
            printf("Waits for the feedback from the server. Has the feedback been well received...");
            if(net2_read_from_socket(&my_socket, (void*)data, data_length) >= 0)
            {
                printf("yes.\n");
                printf("The server says : \"%s\".\n", data);
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
