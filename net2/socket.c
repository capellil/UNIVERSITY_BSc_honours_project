#include "socket.h"

#include <stdio.h> // printf
#include <stdlib.h> // NULL
#include <unistd.h> // close

void print_net2_socket(char* heading, struct net2_socket_t* net2_socket)
{
	printf("%s\n", heading);
	printf("\t- File descriptor : %d\n", net2_socket->_socket);
	
	int address = ntohl(net2_socket->_address.sin_addr.s_addr);
	switch(address)
	{
		case INADDR_ANY :
			printf("\t- Address : INADDR_ANY\n");
			break;
		default :
			printf("\t- Address : %d.%d.%d.%d\n", address >> 24 & 0xFF, 
												  address >> 16 & 0xFF, 
												  address >> 8  & 0xFF, 
												  address       & 0xFF);
			break;
	}
										  
	unsigned short port = ntohs(net2_socket->_address.sin_port);
	printf("\t- Port : %d\n", port);
	
	char* family;
	switch(net2_socket->_address.sin_family)
	{
		case AF_INET :
			family = "AF_INET";
			break;
		case AF_UNIX :
			family = "AF_UNIX";
			break;
		default :
			family = "UNKNOWN";
			break;
	}
	printf("\t- Family : %s\n", family);
}

int create_socket()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

int create_net2_socket(struct net2_socket_t* net2_socket)
{
	int result = 0;
	int socket = create_socket();
	if(socket != -1)
	{
		net2_socket->_socket = socket;
	}
	else
	{
		net2_socket = NULL;
		result = -1;
	}
	
	return result;
}

int create_and_bind_net2_socket(unsigned short port, struct net2_socket_t* net2_socket)
{
	int result = 0;
	int socket = create_socket();
	if(socket != -1)
	{
		struct sockaddr_in socket_address;
		socket_address.sin_family = AF_INET; // IPv4 Internet Protocol
		socket_address.sin_addr.s_addr = INADDR_ANY; // All interfaces
		socket_address.sin_port = htons(port);
		
		if(!bind(socket, (struct sockaddr*)&socket_address, sizeof(socket_address)))
		{
			net2_socket->_socket = socket;
			net2_socket->_address = socket_address;
		}
		else
		{
			net2_socket = NULL;
			result = -2;
		}		
	}
	else
	{
		net2_socket = NULL;
		result = -1;
	}
	
	return result;
}

int listen_on_socket(int socket)
{
	return listen(socket, 1);
}

int accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client)
{
	struct sockaddr_in address;
	socklen_t address_length = sizeof(struct sockaddr_in);
	int socket = accept(server->_socket, (struct sockaddr*)&address, &address_length);
	client->_socket = socket;
	client->_address = address;
	
	return socket;
}

int connect_to_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET; // IPv4 Internet Protocol
	server.sin_addr.s_addr = htonl(address);
	server.sin_port = htons(port);
	socklen_t server_length = sizeof(struct sockaddr_in);
	
	return connect(net2_socket->_socket, (struct sockaddr*)&server, server_length);
}

int write_to_socket(int socket, char* data, unsigned int data_length)
{
	return send(socket, data, data_length, 0);
}

int read_from_socket(int socket, char* data, unsigned int data_length)
{
	return recv(socket, data, data_length, 0);
}

int close_socket(int socket)
{
	return close(socket);
}
