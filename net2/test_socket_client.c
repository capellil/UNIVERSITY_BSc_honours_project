/**
 * @file test_socket_client.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE

/**
 * @brief Creates a client socket with :
 		  <ul>
 	          <li>IPv4 address (THD_LOCALHOST_IPV4 + 1)
 	          <li>port (THD_DEFAULT_PORT + 1)<br>
		  </ul>
		  The socket is then connected to the server at :<br>
		  <ul>
		      <li>IPv4 address THD_LOCALHOST_IPV4
		      <li>port THD_DEFAULT_PORT
	      </ul>
 * @return <ul>
 		       <li>SUCCESS : EXIT_SUCCESS
 	           <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	struct net2_socket_t client;
	int result = create_net2_socket(&client); 
	
	if(!result)
	{
		printf("CLIENT CREATED\n");
		print_net2_socket("Client socket", &client);
		
		printf("TRIES TO CONNECT TO THE SERVER\n");
		if(!connect_to_socket(&client, 1844854179, THD_DEFAULT_PORT))
		{
			printf("HAS CONNECTED\n");
		}
		else
		{
			perror("CONNECTION");
			programme_return = EXIT_FAILURE;
		}
	}
	else
	{
		perror("CREATING CLIENT");
		programme_return = EXIT_FAILURE;
	}
	
	return programme_return;
}
