/**
 * @file test_socket_client_bidirectional.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "net2_socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <string.h> // memcpy | strlen

/**
 * @brief Here is the configuration to set : @image html client_server_bidirectional.png <br>
          Creates the client reception socket with :
 		  <ul>
 	          <li>IPv4 address (IP_CLIENT)
 	          <li>port 3001
		  </ul>
		  Creates the client transmission socket with :
 		  <ul>
 	          <li>IPv4 address (IP_CLIENT)
 	          <li>port 3002
		  </ul>
		  The client transmission socket is then connected to the server at :<br>
		  <ul>
		      <li>IPv4 address IP_SERVER
		      <li>port 3000
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

	if(argc == 5)
	{
		unsigned int server_reception_address = 0;
		for(unsigned int i = 1; i < 5; i++)	
		{
			server_reception_address <<= 8;
			server_reception_address += atoi(argv[i]);
		} 
	
		// We create the client reception socket (3001)
		unsigned short client_reception_port = 3001;
		struct net2_socket_t client_reception_socket;
		int result = net2_create_and_bind_socket(client_reception_port, &client_reception_socket); 
		
		if(!result)
		{
			printf("CLIENT RECEPTION SOCKET CREATED\n");
			net2_print_socket("Client reception socket", &client_reception_socket);
				
			// We create the client transmission socket
			struct net2_socket_t client_transmission_socket;
			result = net2_create_and_store_socket(&client_transmission_socket); 
		
			if(result >= 0)
			{
				printf("CLIENT TRANSMISSION SOCKET CREATED\n");
				net2_print_socket("Client transmission socket", &client_transmission_socket);
				
				printf("TRIES TO CONNECT TO THE SERVER RECEPTION SOCKET\n");
				result = net2_connect_to_socket(&client_transmission_socket, server_reception_address, 3000);
				
				if(!result)
				{
					printf("HAS CONNECTED TO THE SERVER RECEPTION SOCKET\n");
				
					// The client now listens to accept the server's transmission socket connection
					if(!net2_listen_on_socket(&client_reception_socket))
					{
						printf("IS LISTENING ON CLIENT RECEPTION SOCKET\n");
						
						// The length of the data : port (= short)
						const int data_length = sizeof(short);
						char data[data_length];
						memcpy(data, &client_reception_port, sizeof(short));
					
						// The client's reception socket location is sent to the server's reception socket
						net2_write_to_socket(&client_transmission_socket, data, data_length);
					
						struct net2_socket_t server_transmission_socket;
						result = net2_accept_from_socket(&client_reception_socket, &server_transmission_socket);
						
						if(result != -1)
						{
							printf("SERVER TRANSMISSION SOCKET ACCEPTED ON CLIENT RECEPTION SOCKET\n");
							
							char message_buffer;
							
							for(int i = 48; i < 58; i++)
							{
							    message_buffer = i;
							    printf("SENDS MESSAGE AS A NUMBER \"%c\"... ", message_buffer);
							    net2_write_to_socket(&client_transmission_socket, &message_buffer, sizeof(char));
							    printf("DONE.\n");
							
							    printf("READS THE FIRST NUMBER FROM CLIENT... ");
							    net2_read_from_socket(&server_transmission_socket, &message_buffer, sizeof(char));
						        printf("DONE => \"%c\".\n", message_buffer);
						    }
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

		net2_close_socket(&client_reception_socket);
	}
	else
	{
		printf("INCORRECT NUMBER OF PARAMETERS : Expected 5 got %d.\n", argc);
		programme_return = EXIT_FAILURE;
	}
		
		
	return programme_return;
}
