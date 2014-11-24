/**
 * @file test_socket_server.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE

/**
 * @brief Creates a server socket with :
 	      <ul> 
 	          <li>IPv4 address (THD_LOCALHOST_IPV4 + 1)
 	          <li>port 3001
      	  </ul> 
		  The socket then listens and accepts a connection
 * @return <ul>
 		      <li>SUCCESS : EXIT_SUCCESS
 	          <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	int address = THD_LOCALHOST_IPV4;
	unsigned short port = 3000;
	struct net2_socket_t server, client;
	int result = create_net2_socket(address, port, &server); 
	
	if(!result)
	{
		printf("SERVER CREATED AND STARTS TO LISTEN\n");
		if(listen_on_socket(server._socket) == -1)
		{
			printf("SERVER IS LISTENING, WAITING A CONNECTION TO ACCEPT IT\n");
			if(accept_from_socket(&server, &client) == -1)
			{
				printf("HAS ACCEPTED A CONNECTION\n");
			}
			else
			{
				perror("ACCEPTING A CONNECTION");
				programme_return = EXIT_FAILURE;
			}
		}
		else
		{
			perror("LISTENING");
			programme_return = EXIT_FAILURE;
		}
	}
	else
	{
		perror("CREATING SERVER");
		programme_return = EXIT_FAILURE;
	}
	
	return programme_return;
}
