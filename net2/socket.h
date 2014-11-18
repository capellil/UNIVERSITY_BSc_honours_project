/**
 * @file socket.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

#define LOCALHOST_IPV4 0x7F000001

#include <sys/types.h> ///< Compulsory for AF_INET, SOCK_STREAM...
#include <sys/socket.h> ///< Obvious...
#include <netinet/in.h> ///< sockaddr_in | IPPROTO_TCP | IPPROTO_UDP
#include <arpa/inet.h> ///< inet_ntoa

/**
 * @class net2_socket_t
 * @brief Structure containing the socket and both its address and port.
 **/
struct net2_socket_t
{
	int _socket; ///< The socket file descriptor (e.g "The socket itself")
	struct sockaddr_in _address;
};

/**
 * @brief Prints out the information on the given net2_socket : its file descriptor, address and port.
 * @param socket - The socket the be printed out.
 * @pre socket != NULL
 **/
void print_net2_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket used for communication with a remote machine. Uses TCP/IP.
 * @return Returns a socket descriptor on the created socket or -1 if it failed.
 **/
int create_socket();

/**
 * @brief Creates a socket at the given address with the given port.
 * @param address - The address to be used.
 * @param port - The port to be used.
 * @param A pointer to store the created net2 socket in.
 * @param \li 0 if it succeeded.
          \li Negative if it failed.
 **/
int create_net2_socket(int address, unsigned short port, struct net2_socket_t* net2_socket);

/**
 * @brief Listens from the given socket.
 * @param socket - The socket to listen on.
 * @return \li 0 if succeeded
		   \li Otherwise if it failed.
 **/
int listen_on_socket(int socket);

/**
 * @brief Accept a connection from a client to the given server.
 * @param server - The server to listen on and accepting a connection onto.
 * @param client - A pointer to store data in.
 * @pre server != NULL && client != NULL
 **/
void accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client);

int connect_to_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port);

/**
 * @brief Writes the given data to the given socket.
 * @param socket - The socket to write to.
 * @param data - The data to be transmitted.
 * @param dataLength - The data length.
 * @return The amount of byte written.
 **/
int write_to_socket(int socket, char* data, unsigned int dataLength);

/**
 * @brief Reads from the given socket <dataLength> bytes and store it into the space pointed by <data>.
 * @param socket - The socket to read from.
 * @param data - A pointer on the data allocated space.
 * @param dataLength - The number of bytes to read from the socket.
 * @pre dataLength is less than the size of allocated area pointed by data.
 * @return \li The number of read bytes
 **/
int read_from_socket(int socket, char* data, unsigned int dataLength);

#endif
