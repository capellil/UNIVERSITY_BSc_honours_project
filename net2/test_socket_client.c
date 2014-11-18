#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int programme_return = EXIT_SUCCESS;
	
	int address = LOCALHOST_IPV4 + 1;
	unsigned short port = 3001;
	struct net2_socket_t client;
	int result = create_net2_socket(address, port, &client); 
	
	if(!result)
	{
		printf("CLIENT CREATED\n");
		print_net2_socket(&client);
		printf("TRIES TO CONNECT TO THE SERVER\n");
		if(!connect_to_socket(&client, LOCALHOST_IPV4, 3000))
		{
			printf("HAS CONNECTED\n");
		}
		else
		{
			perror("CONNECTION");
		}
	}
	else
	{
		perror("CREATING SERVER");
	}
	
	return programme_return;
}
