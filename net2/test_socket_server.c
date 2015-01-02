/**
 * @file test_socket_server.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "net2_socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE

/**
 * @brief Creates a server socket with :
 	      <ul> 
 	          <li>IPv4 address (THD_LOCALHOST_IPV4 + 1)
 	          <li>port 3000
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
	
	unsigned short port = 3000;
	struct net2_socket_t server, client;
	int result = net2_create_and_bind_socket(port, &server); 
	
	if(!result)
	{
		printf("SERVER CREATED AND STARTS TO LISTEN\n");
		net2_print_socket("Server socket", &server);
		
		if(net2_listen_on_socket(&server) != -1)
		{
			printf("SERVER IS LISTENING, WAITING A CONNECTION TO ACCEPT IT\n");
			result = net2_accept_from_socket(&server, &client);
			
			if(result != -1)
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
	
	net2_close_socket(&server);
	
	return programme_return;
}
