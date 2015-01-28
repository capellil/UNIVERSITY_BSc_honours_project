#include <stdio.h> // printf
#include <stdlib.h> // NULL
#include <unistd.h> // close

#include "net2_socket.h"
#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

/**
 * @brief Creates a socket used for communication with a remote machine, initialised to streaming data (suitable for TCP protocol).
 * @return <ul>
    	       <li>SUCCESS : Non negative number that is the socket descriptor
 		       <li>FAILED : -1, errno is set appropriately
	       </ul>
 **/
static int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
 
void net2_socket_print(char* heading, struct net2_socket_t* net2_socket)
{
	if(heading)
	{
	    printf("%s\n", heading);   
    }
    
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
	switch(port)
	{
		case 0 :
			printf("\t- Port : ANY_PORT\n");
			break;
		default:
			printf("\t- Port : %d\n", port);
			break;
	}
	
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

int net2_socket_create(struct net2_socket_t* net2_socket)
{
	int socket = create_socket();
	
	// TEST : Is the socket successfully created ?
	if(socket != -1)
	{	    
	    // Socket successfully created.
		net2_socket->_socket = socket;
		#ifdef NET2_DEBUG
		    net2_debug_success("net2_socket_create");
		#endif
	}
	else
	{
	    // Socket creation failed.
	    #ifdef NET2_DEBUG
		    net2_debug_failure("net2_socket_create", "Socket creation failed");
		#endif
	}
	
	return socket;
}

int net2_socket_create_and_bind(struct net2_socket_t* net2_socket, unsigned short port)
{
	int result = 0;
	int socket = create_socket();
	
	// TEST : Is the socket successfully created ?
	if(socket != -1)
	{
	    // Yes, Socket successfully created.
		struct sockaddr_in socket_address;
		socket_address.sin_family = AF_INET; // IPv4 Internet Protocol
		socket_address.sin_addr.s_addr = INADDR_ANY; // All interfaces
		socket_address.sin_port = htons(port);
		
		int optval = 1;
		
		// TEST : Is socket option set
	    if(!setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)))
	    {		
	        // Yes, the socket REUSEADDR option set succeeded.
		    // TEST : Is socket binding successful ?
		    if(!bind(socket, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_in)))
		    {
		        // Yes, the socket binding succeeded.
		        // Socket binding successful.
			    net2_socket->_socket = socket;
			    net2_socket->_address = socket_address;
			    #ifdef NET2_DEBUG
		             net2_debug_success("net2_socket_create_and_bind");
		        #endif
		    }
		    else
		    {
		        // No, the socket binding failed.
			    result = -3;
			    #ifdef NET2_DEBUG
			        char message[128]; 
			        sprintf(message, "Socket binding failed when trying with port \"%d\", net2_socket pointer is %p", port, net2_socket);
		            net2_debug_failure("net2_socket_create_and_bind", message);
		        #endif
		    }
	    }
	    else
	    {   
	        // No, the socket REUSEADDR option set failed.
	        result = -2;
		    #ifdef NET2_DEBUG
		         net2_debug_failure("net2_socket_create_and_bind", "Socket SOL_REUSEADDR option set failed.");
		    #endif
	    }		
	}
	else
	{
	    // No, the socket creation failed.
		result = -1;
		#ifdef NET2_DEBUG
		     net2_debug_failure("net2_socket_create_and_bind", "Socket creation failed");
		#endif
	}
	
	return result;
}

int net2_socket_listen(struct net2_socket_t* net2_socket)
{
	return listen(net2_socket->_socket, 1);
}

int net2_socket_accept(struct net2_socket_t* server, struct net2_socket_t* client)
{
	struct sockaddr_in address;
	socklen_t address_length = sizeof(struct sockaddr_in);
	int socket = accept(server->_socket, (struct sockaddr*)&address, &address_length);
	client->_socket = socket;
	client->_address = address;
	
	return socket;
}

int net2_socket_connect(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET; // IPv4 Internet Protocol
	server.sin_addr.s_addr = htonl(address);
	server.sin_port = htons(port);
	socklen_t server_length = sizeof(struct sockaddr_in);
	
	return connect(net2_socket->_socket, (struct sockaddr*)&server, server_length);
}

int net2_socket_write(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
	return send(net2_socket->_socket, data, data_length, 0);
}

int net2_socket_read(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
	return recv(net2_socket->_socket, data, data_length, 0);
}

int net2_socket_close(struct net2_socket_t* net2_socket)
{
	return close(net2_socket->_socket);
}

unsigned int net2_socket_get_ip(struct net2_socket_t* net2_socket)
{
	return ntohl(net2_socket->_address.sin_addr.s_addr);
}

unsigned short net2_socket_get_port(struct net2_socket_t* net2_socket)
{
    return ntohs(net2_socket->_address.sin_port);
}
