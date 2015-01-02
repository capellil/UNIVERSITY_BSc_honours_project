/**
 * @file test_socket_client.c
 * @author Capelli Ludovic
 * @version 1.0
 * @date 24/11/2014
 **/

#include "net2_socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE

/**
 * @brief Creates a client socket with :
 		  <ul>
 	          <li>port 3001<br>
		  </ul>
		  The socket is then connected to the server at :<br>
		  <ul>
		      <li>The given IPv4 address
		      <li>port 3000
	      </ul>
 * @return <ul>
 		       <li>SUCCESS : EXIT_SUCCESS
 	           <li>FAILURE : EXIT_FAILURE
           </ul>
**/
int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	if(argc == 2)
    {
        struct sockaddr_in ip_address;
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
	        struct net2_socket_t client;
	        int result = net2_create_and_store_socket(&client); 
	
	        if(result >= 0)
	        {
		        printf("CLIENT CREATED\n");
		        net2_print_socket("Client socket", &client);
		
		        printf("TRIES TO CONNECT TO THE SERVER\n");
		        if(!net2_connect_to_socket(&client, ip_address.sin_addr.s_addr, 3000)) // 1844854179 = "109.246.61.103", it is my ip address. Hard-coded for the moment
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
        }
        else
        {
            perror("IP address conversion from string to number");
            programme_return = EXIT_FAILURE;
        }
    }
    else
    {
        printf("HOW TO USE THIS PROGRAM : \"./%s a.b.c.d\" with \"a.b.c.d\" the IPv4 address to use.\n", argv[0]);
        programme_return = EXIT_FAILURE;
    }
	
	return programme_return;
}
