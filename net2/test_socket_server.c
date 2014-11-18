#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	int address = LOCALHOST_IPV4;
	unsigned short port = 3000;
	struct net2_socket_t server, client;
	int result = create_net2_socket(address, port, &server); 
	
	if(!result)
	{
		printf("SERVER CREATED AND STARTS TO LISTEN\n");
		listen_on_socket(server._socket);
		printf("SERVER IS LISTENING, WAITING A CONNECTION TO ACCEPT IT\n");
		accept_from_socket(&server, &client);
		printf("HAS ACCEPTED A CONNECTION\n");
	}
	else
	{
		perror("CREATING SERVER");
	}
	
	return programme_return;
}
