/**
 * @file test_socket_client_bidirectional.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <string.h> // memcpy

/**
 * @brief Here is the configuration to set : @image html client_server_bidirectional.png <br>
          Creates the client reception socket with :
 		  <ul>
 	          <li>IPv4 address (IP_CLIENT)
 	          <li>port (THD_DEFAULT_PORT + 1)<br>
		  </ul>
		  Creates the client transmission socket with :
 		  <ul>
 	          <li>IPv4 address (IP_CLIENT)
 	          <li>port (THD_DEFAULT_PORT + 2)<br>
		  </ul>
		  The client transmission socket is then connected to the server at :<br>
		  <ul>
		      <li>IPv4 address IP_SERVER
		      <li>port THD_DEFAULT_PORT
	      </ul>
	      Next, the client reception socket location is sent to the server reception socket.
	      Finally, the client reception socket listens and accept the server's transmission socket connection.
 * @return <ul>
 		       <li>SUCCESS : EXIT_SUCCESS
 	           <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	// We create the client reception socket (THD_DEFAULT_PORT + 1)
	unsigned short client_reception_port = THD_DEFAULT_PORT + 1;
	struct net2_socket_t client_reception_socket;
	int result = create_and_bind_net2_socket(client_reception_port, &client_reception_socket); 
	
	if(!result)
	{
		printf("CLIENT RECEPTION SOCKET CREATED\n");
		print_net2_socket("Client reception socket", &client_reception_socket);
			
		// We create the client transmission socket
		struct net2_socket_t client_transmission_socket;
		result = create_net2_socket(&client_transmission_socket); 
	
		if(!result)
		{
			printf("CLIENT TRANSMISSION SOCKET CREATED\n");
			print_net2_socket("Client transmission socket", &client_transmission_socket);
			
			printf("TRIES TO CONNECT TO THE SERVER RECEPTION SOCKET\n");
			result = connect_to_socket(&client_transmission_socket, 1844854179, THD_DEFAULT_PORT);
			
			if(!result)
			{
				printf("HAS CONNECTED TO THE SERVER RECEPTION SOCKET\n");
			
				// The length of the data : address (= int) + port (= short)
				const int data_length = sizeof(short);
				char data[data_length];
				memcpy(data, &client_reception_port, sizeof(short));
				
				// The client's reception socket location is sent to the server's reception socket
				write_to_socket(client_transmission_socket._socket, data, data_length);
				
				// The client now listens to accept the server's transmission socket connection
				if(!listen_on_socket(client_reception_socket._socket))
				{
					printf("IS LISTENING ON CLIENT RECEPTION SOCKET\n");
					
					struct net2_socket_t server_transmission_socket;
					result = accept_from_socket(&client_reception_socket, &server_transmission_socket);
					
					if(result != -1)
					{
						printf("SERVER TRANSMISSION SOCKET ACCEPTED ON CLIENT RECEPTION SOCKET\n");
					}
					else
					{
						perror("ACCEPTING SERVER TRANSMISSION SOCKET ON CLIENT RECEPTION SOCKET");
						programme_return = EXIT_FAILURE;
					}
				}
				else
				{
					perror("LISTENING ON CLIENT RECEPTION SOCKET");
					programme_return = EXIT_FAILURE;
				}
			}
			else
			{
				perror("CONNECTION TO SERVER RECEPTION SOCKET");
				programme_return = EXIT_FAILURE;
			}
		}
		else
		{
			perror("CREATING CLIENT TRANSMISSION SOCKET");
			programme_return = EXIT_FAILURE;
		}
	}
	else
	{
		perror("CREATING CLIENT RECEPTION SOCKET");
		programme_return = EXIT_FAILURE;
	}
	
	close_socket(client_reception_socket._socket);
	
	return programme_return;
}
