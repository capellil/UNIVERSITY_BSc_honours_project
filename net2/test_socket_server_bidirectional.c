/**
 * @file test_socket_server_bidirectional.c
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
          Creates the server reception socket with :
 		  <ul>
 	          <li>IPv4 address (IP_SERVER)
 	          <li>port (THD_DEFAULT_PORT)<br>
		  </ul>
		  Next, the server reception socket listens and accept the client's transmission socket connection.
		  Then, the server reads the message containing the client's reception socket location.
		  Creates the server transmission socket with :
 		  <ul>
 	          <li>IPv4 address (IP_SERVER)
 	          <li>port (THD_DEFAULT_PORT + 3)<br>
		  </ul>
		  The server transmission socket is then connected to the client's reception socket from what has been read. In theory :<br>
		  <ul>
		      <li>IPv4 address IP_CLIENT
		      <li>port THD_DEFAULT_PORT + 1
	      </ul>
 * @return <ul>
 		       <li>SUCCESS : EXIT_SUCCESS
 	           <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	// We create the server reception socket (THD_DEFAULT_PORT)
	unsigned short server_reception_port = THD_DEFAULT_PORT;
	struct net2_socket_t server_reception_socket;
	int result = create_and_bind_net2_socket(server_reception_port, &server_reception_socket); 
	
	if(!result)
	{
		printf("SERVER RECEPTION SOCKET CREATED\n");
		print_net2_socket("Server reception socket", &server_reception_socket);
		
		// We listen on the server reception socket
		if(!listen_on_socket(server_reception_socket._socket))
		{
			printf("IS LISTENING ON CLIENT RECEPTION SOCKET\n");
			
			struct net2_socket_t client_transmission_socket;
			result = accept_from_socket(&server_reception_socket, &client_transmission_socket);
			
			if(result != -1)
			{
				printf("CLIENT TRANSMISSION SOCKET ACCEPTED ON SERVER RECEPTION SOCKET\n");
				print_net2_socket("Client transmission socket", &client_transmission_socket);
				
				const unsigned int buffer_length = sizeof(short);
				char buffer[buffer_length];
				if(read_from_socket(client_transmission_socket._socket, buffer, buffer_length) > 0)
				{
					unsigned short client_reception_port;
					memcpy(&client_reception_port, buffer, sizeof(short));
					
					struct net2_socket_t server_transmission_socket;
					result = create_net2_socket(&server_transmission_socket); 
	
					if(!result)
					{
						printf("SERVER TRANSMISSION SOCKET CREATED\n");
						printf("TRIES TO CONNECT TO THE CLIENT RECEPTION SOCKET\n");
						
						result = connect_to_socket(&server_transmission_socket, get_ip_of_socket(&client_transmission_socket), client_reception_port);
						
						if(!result)
						{
							printf("SERVER TRANSMISSION SOCKET CONNECTED TO CLIENT RECEPTION SOCKET\n");
						}
						else
						{
							perror("CONNECTION TO CLIENT RECEPTION SOCKET");
							programme_return = EXIT_FAILURE;
						}
					}
					else
					{
						perror("CREATING THE SERVER TRANSMISSION SOCKET");
						programme_return = EXIT_FAILURE;
					}
				}
				else
				{
					perror("READING FROM SERVER RECEPTION SOCKET");
					programme_return = EXIT_FAILURE;
				}				
			}
			else
			{
				perror("ACCEPTING CLIENT TRANSMISSION SOCKET ON SERVER RECEPTION SOCKET");
				programme_return = EXIT_FAILURE;
			}
		}
		else
		{
			perror("LISTENING ON THE SERVER RECEPTION SOCKET");
			programme_return = EXIT_FAILURE;
		}
	}
	else
	{
		perror("CREATING SERVER RECEPTION SOCKET");
		programme_return = EXIT_FAILURE;
	}
	
	close_socket(server_reception_socket._socket);
	
	return programme_return;
}
