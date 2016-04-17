/**
 * @file test_socket_server.c
 * @author Capelli Ludovic
 * @version 2.0
 * @date 19/01/2015
 **/

#include "../net2/net2_socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <string.h> // strcpy

/**
 * @brief Creates a socket bound to the given port. Listens on it and accept a connection to communicate with.
 *
 * <ul>
 *     <li>The argument 0 : the name of the programme. (hidden from the user input)
 *     <li>The argument 1 : the port to bind on.
 * </ul>
 * @return <ul>
 		      <li>SUCCESS : EXIT_SUCCESS
 	          <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	printf("Checking the number of parameters...");
	if(argc == 2)
    {
        printf("right number.\n");
        
        printf("Checking the first parameter given : is \"%s\" a valid port...", argv[1]);
        if(atoi(argv[1]) >= 0 && atoi(argv[1]) < 65536)
        {
            printf("yes.\n");

            struct net2_socket_t server_socket;
            int result = net2_create_server_socket(atoi(argv[1]), &server_socket);

            printf("Tries to establish a server socket on port %d. Has the server establishment succeed...", atoi(argv[1]));
			if(result == 0)
            {
	            printf("yes.\n");
	            
	            struct net2_socket_t client_socket;
	            printf("Tries to accept a connection. Has the connection acceptation succeed...");
	            if(net2_accept_from_socket(&server_socket, &client_socket) >= 0)
	            {
	                printf("yes.\n");
	                printf("The server is now connected to the client.\n");
	            
	                char data[13];
	                unsigned int data_length = 13;
	                
	                printf("Waits for client to send a message. Has the message been well received...");
                    if(net2_read_from_socket(&client_socket, (void*)data, data_length) >= 0)
                    {
                        printf("yes.\n");
                        printf("The client said : \"%s\".\n", data);
                        
                        strcpy(data, "Hello client\0");
                        
                        printf("Reply by sending \"%s\". Has the reply been sent...", data);
	                    if(net2_write_to_socket(&client_socket, (void*)data, data_length) != -1)
	                    {
	                        printf("yes.\n");
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
        printf("wrong number of parameters.\n");
        printf("%d parameters have been received instead of %d.\n", argc - 1, 1);
        programme_return = EXIT_FAILURE;
    }
	
	return programme_return;
}
