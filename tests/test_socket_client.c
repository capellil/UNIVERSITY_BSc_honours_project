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
	
	printf("Checking the number of parameters...");
	if(argc == 3)
    {
        printf("right number.\n");
        
        struct sockaddr_in ip_address;
        printf("Checking the first parameter given : is \"%s\" a valid IPv4 address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
            
            printf("Checking the port number : is %d a valid port number...", atoi(argv[2]));
            if(atoi(argv[2]) >= 0 && atoi(argv[2]) < 65536)
            {
                printf("yes.\n");

	            struct net2_socket_t client_socket;
	            int result = net2_socket_create(&client_socket); 
	
	            printf("Checking the socket creation : did it succeed...");
	            if(result >= 0)
	            {
		            printf("yes.\n");
		            printf("The client socket is now created.\n");
		
		            printf("Tries to connect to the server. Did the connection succeed...");
		            if(!net2_socket_connect(&client_socket, ip_address.sin_addr.s_addr, atoi(argv[2])))
		            {
			            printf("yes.\n");
			            printf("The client is now connected to the server.\n");
			            
			            char data[13] = "Hello server\0";
			            unsigned int data_length = 13;
			            
			            printf("Test the connection by sending \"%s\" to the server. Has the message been correctly sent...", data);
			            if(net2_socket_write(&client_socket, (void*)data, data_length) != -1)
			            {
			                printf("yes.\n");
			                printf("Waits for the feedback from the server. Has the feedback been well received...");
			                if(net2_socket_read(&client_socket, (void*)data, data_length) >= 0)
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
        printf("%d parameters have been received instead of %d.\n", argc - 1, 2);
        programme_return = EXIT_FAILURE;
    }
	
	return programme_return;
}
