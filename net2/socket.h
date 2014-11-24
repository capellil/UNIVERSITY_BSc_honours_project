/**
 * @file socket.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef THD_SOCKET_INCLUDED
#define THD_SOCKET_INCLUDED

#define THD_LOCALHOST_IPV4 0x7F000001

#include <sys/types.h> // Compulsory for AF_INET, SOCK_STREAM...
#include <sys/socket.h> // Obvious...
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP
#include <arpa/inet.h> // inet_ntoa

/**
 * @class net2_socket_t
 * @brief Structure containing the socket and both its address and port.
 **/
struct net2_socket_t
{
	int _socket; ///< The socket file descriptor (e.g "The socket itself")
	struct sockaddr_in _address; ///< The socket address structure (e.g : Its address, port, address family and so on).
};

/**
 * @brief Prints out the information on the given net2_socket : its file descriptor, address and port.
 * @param socket - The socket the be printed out.
 * @pre socket != NULL
 **/
void print_net2_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket used for communication with a remote machine. Uses TCP/IP.
 * @return <ul>
    	       <li>SUCCESS : Non negative number that is the socket descriptor
 		       <li>FAILED : -1, errno is set approprietaly
	       </ul>
 **/
int create_socket();

/**
 * @brief Creates a socket at the given address with the given port.
 * @param address - The address to be used.
 * @param port - The port to be used.
 * @param net2_socket - A pointer to store the created net2 socket in.
 * @return <ul>
 		       <li>SUCCESS : 0
               <li>FAILED : Negative number, errno is set approprietaly
           </ul>
 **/
int create_net2_socket(int address, unsigned short port, struct net2_socket_t* net2_socket);

/**
 * @brief Listens from the given socket.
 * @param socket - The socket to listen on.
 * @return <ul>
               <li>SUCCESS : 0
		       <li>FAILED : different from 0, errno is set approprietaly
	       </ul>
 **/
int listen_on_socket(int socket);

/**
 * @brief Accept a connection from a client to the given server.
 * @param server - The server to listen on and accepting a connection onto.
 * @param client - A pointer to store data in.
 * @return <ul>
               <li>SUCCESS : Non-negative number that is the accepted socket descriptor
 		       <li>FAILED : -1, errno is set approprietaly
	       </ul>
 * @pre (server != NULL) AND (client != NULL)
 **/
int accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client);

/**
 * @brief Connects the given socket to the provided address and port.
 * @param net2_socket - The socket to be connected
 * @param address - The server address
 * @param port - The server port
 * @return <ul>
 			   <li>SUCCESS : 0
               <li>FAILED : -1
           </ul>
 **/
int connect_to_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port);

/**
 * @brief Writes the given data to the given socket.
 * @param socket - The socket to write to.
 * @param data - The data to be transmitted.
 * @param data_length - The data length.
 * @return <ul>
 		       <li>SUCCESS : Non-negative number that is he amount of byte written
               <li>FAILED : -1, errno is set approprietaly
           </ul>
 **/
int write_to_socket(int socket, char* data, unsigned int data_length);

/**
 * @brief Reads from the given socket <data_length> bytes and store it into the space pointed by <data>.
 * @param socket - The socket to read from.
 * @param data - A pointer on the data allocated space.
 * @param data_length - The number of bytes to read from the socket.
 * @pre data_length is less than the size of allocated area pointed by data.
 * @return <ul>
 			   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes
               <li>SUCCESS : 0 if the peer has performed an orderly shutdown
               <li>FAILED : -1, errno is set approprietaly
           </ul>
 **/
int read_from_socket(int socket, char* data, unsigned int data_length);

#endif
