#include <stdio.h> // printf
#include <stdlib.h> // NULL
#include <unistd.h> // close

#include "net2_socket.h"

/**
 * @brief Creates a socket used for communication with a remote machine.
 *
 * This function creates a socket relying on TCP/IPv4. If the function fails, the given socket_descriptor is left untouched.
 * @param socket_descriptor A pointer to use to store the result
 * @return <ul>
 *             <li>SUCCESS : 0.
 *             <li>FAILED : -1, errno is set appropriately.
 *         </ul>
 * @pre <ul>
 *          <li>parameter socket_descriptor != NULL</li>
 *      </ul>
 * @post <ul>
 *           <li>SUCCESS: parameter socket = file descriptor on the new socket.</li>
 *           <li>FAILURE: left untouched.</li>
 *       </ul>
 **/
static int create_tcp_ipv4_socket(int* socket_descriptor)
{
    int family = AF_INET; // IPv4
    int type = SOCK_STREAM; // Stream, suitable for TCP.
    int protocol = IPPROTO_TCP; // TCP.
    int result;    

    result = socket(family, type, protocol);

    if(result == -1)
    {
        // The socket creation failed.
        return -1;
    }

    *socket_descriptor = result;
    return 0;
}

int net2_create_socket(struct net2_socket_t* net2_socket)
{
    int socket_descriptor;
    
    if(create_tcp_ipv4_socket(&socket_descriptor) == -1)
    {       
        net2_debug_failure("Socket creation failed");
        return -1;
    }

    net2_socket->_socket = socket_descriptor;
    net2_debug_success();
    return 0;
}

int net2_create_and_bind_socket(struct net2_socket_t* net2_socket, unsigned short int port)
{
    int socket_descriptor;
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET; // IPv4 Internet Protocol
    socket_address.sin_addr.s_addr = INADDR_ANY; // All interfaces
    socket_address.sin_port = htons(port);
    socklen_t socket_length = sizeof(struct sockaddr_in);
    int option_value = 1;
    
    if(create_tcp_ipv4_socket(&socket_descriptor) == -1)
    {
        net2_debug_failure("Socket creation failed.");
        return -1;
    }    
    
    if(setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) == -1)
    {   
        net2_debug_failure("Socket SOL_REUSEADDR option set failed.");    
        return -2;
    }

    if(bind(socket_descriptor, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_in)) == -1)
    {
        char message[NET2_DEBUG_MAX_MESSAGE_LENGTH]; 
        sprintf(message, "Socket binding failed when trying with port \"%d\", net2_socket pointer is %p.", port, net2_socket);
        net2_debug_failure(message);
        return -3;
    }
    
    if(getsockname(socket_descriptor, (struct sockaddr*)&socket_address, &socket_length) == -1)
    {
        net2_debug_failure("The socket information extraction failed.");
        return -4;
    }    

    net2_socket->_socket = socket_descriptor;
    net2_socket->_address = socket_address;
    net2_debug_success();
    return 0;
}

int net2_listen_from_socket(struct net2_socket_t* net2_socket)
{    
    if(listen(net2_socket->_socket, 1) == -1)
    {
        net2_debug_failure("The socket listening failed.");
        return -1;
    }

    net2_debug_success();
    return 0;
}

int net2_accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client)
{
    struct sockaddr_in address;
    socklen_t address_length = sizeof(struct sockaddr_in);
    int socket_descriptor;

    socket_descriptor = accept(server->_socket, (struct sockaddr*)&address, &address_length);
    
    if(socket_descriptor == -1)
    {
        net2_debug_failure("The accept on socket failed.");
        return -1;
    }

    client->_socket = socket_descriptor;
    client->_address = address;
    net2_debug_success();
    return 0;
}

int net2_connect_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short int port)
{
    struct sockaddr_in server;
    server.sin_family = AF_INET; // IPv4
    server.sin_addr.s_addr = htonl(address);
    server.sin_port = htons(port);
    socklen_t server_length = sizeof(struct sockaddr_in);
    
    if(connect(net2_socket->_socket, (struct sockaddr*)&server, server_length) == -1)
    {
        net2_debug_failure("The socket connection failed.");
        return -1;
    }

    net2_debug_success();    
    return 0;
}

int net2_write_to_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
    int result;
    
    result = send(net2_socket->_socket, data, data_length, 0);

    if(result == 0)
    {
        net2_debug_failure("Nothing has been written.");
        return -1;
    }

    if(result < 0)
    {
        net2_debug_failure("The socket send failed.");
        return -2;
    }
    
    net2_debug_success();
    return 0;
}

int net2_read_from_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
    int result;
    
    result = recv(net2_socket->_socket, data, data_length, 0);
    
    if(result == 0)
    {
        net2_debug_failure("The remote peer has performed an orderly shutdown.");
        return -1;
    }

    if(result == -1)
    {
        net2_debug_failure("The socket read failed.");
        return -2;
    }

    net2_debug_success();    
    return 0;
}

int net2_close_socket(struct net2_socket_t* net2_socket)
{    
    if(close(net2_socket->_socket) == -1)
    {
        net2_debug_failure("The socket close failed.");
        return -1;
    }

    net2_debug_success();
    return 0;
}

unsigned int net2_get_ip_of_socket(struct net2_socket_t* net2_socket)
{
    net2_debug_success();
    return ntohl(net2_socket->_address.sin_addr.s_addr);
}

unsigned short int net2_get_port_of_socket(struct net2_socket_t* net2_socket)
{
    net2_debug_success();
    return ntohs(net2_socket->_address.sin_port);
}

int net2_create_client_socket(char* ip_address, unsigned short int port, struct net2_socket_t* client_socket)
{
    const int family = AF_INET;
    struct in_addr in_address;
    int result;

    result = inet_pton(family, ip_address, &in_address);

    if(result == 0)
    {
        net2_debug_failure("The given ip address is not a valid IPv4 address.");
        return -1;
    }

    if(result == -1)
    {
        net2_debug_failure("The family variable does not contain a valid internet family.");
        return -2;
    }

    if(net2_create_socket(client_socket) == -1)
    {
        net2_debug_failure("The client socket creation failed.");
        return -3;
    }

    if(net2_connect_socket(client_socket, ntohl(in_address.s_addr), port) == -1)
    {
        net2_debug_failure("The socket connection failed.");
        return -4;
    }

    net2_debug_success();
    return 0;
}

int net2_create_server_socket(unsigned short int port, struct net2_socket_t* server_socket)
{
    if(net2_create_and_bind_socket(server_socket, port) < 0)
    {
        net2_debug_failure("Something went wrong in the creation & binding process.");
        return -1;
    }

    if(net2_listen_from_socket(server_socket) == -1)
    {
        net2_debug_failure("Something went wrong in the listening process.");
        return -2;
    }

    net2_debug_success();
    return 0;
}

void net2_print_socket(char* heading, struct net2_socket_t* net2_socket)
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