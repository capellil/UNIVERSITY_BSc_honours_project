/**
 * @file net2_socket.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 18/01/2015
 **/

#ifndef NET2_SOCKET_INCLUDED
#define NET2_SOCKET_INCLUDED

#include <sys/types.h> // Compulsory for AF_INET, SOCK_STREAM...
#include <sys/socket.h> // Obvious...
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP
#include <arpa/inet.h> // inet_ntoa

/**
 * @class net2_socket_t
 * @brief Structure containing the socket and its address structure.
 **/
struct net2_socket_t
{
	int _socket; ///< The socket file descriptor (e.g "The socket itself")
	struct sockaddr_in _address; ///< The socket address structure (e.g : Its address, port, address family and so on).
};

/**
 * @brief Prints out the information on the given net2_socket : its file descriptor, address and port.
 * @param heading The message to be displayed as a heading.
 * @param socket The socket to be printed out.
 * @pre net2_socket != NULL.
 **/
void net2_socket_print(char* heading, struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket used for communication and stores it into the given pointer. Uses TCP/IP protocols.
 * @param net2_socket A pointer to store the created net2 socket in.
 * @return <ul>
 		       <li>SUCCESS : >= 0.
               <li>FAILED : -1, errno is set appropriately.
           </ul>
 * @pre net2_socket != NULL.
 **/
int net2_socket_create(struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket and binds it to the given port. Uses TCP/IP protocols.
 * @param net2_socket A pointer to store the created net2 socket in.
 * @param port The port to be used.
 * @return <ul>
 		       <li>SUCCESS : 0.
               <li>FAILED : Negative number, errno is set appropriately.
           </ul>
 * @pre net2_socket != NULL.
 **/
int net2_socket_create_and_bind(struct net2_socket_t* net2_socket, unsigned short port);

/**
 * @brief Listens on the given socket to accept connections later on. Does not start any acceptation mechanism though.
 * @param net2_socket The net2 socket to listen on.
 * @return <ul>
               <li>SUCCESS : 0.
		       <li>FAILED : different from 0, errno is set appropriately.
	       </ul>
 * @pre net2_socket != NULL.
 **/
int net2_socket_listen(struct net2_socket_t* net2_socket);

/**
 * @brief Accepts a connection from the given server net2 socket and stores it into the given client net2 socket.
 * @param server The server to listen on and accepting a connection onto.
 * @param client A pointer to store the client net2 socket in.
 * @return <ul>
               <li>SUCCESS : >= 0.
 		       <li>FAILED : -1, errno is set appropriately.
	       </ul>
 * @pre <ul>
            <li>server != NULL.
            <li>client != NULL.
        </ul>
 **/
int net2_socket_accept(struct net2_socket_t* server, struct net2_socket_t* client);

/**
 * @brief Connects the given socket to the provided address and port.
 * @param net2_socket The socket to be connected.
 * @param address The IP address to connect to.
 * @param port The port to connect to.
 * @return <ul>
 			   <li>SUCCESS : 0.
               <li>FAILED : -1.
           </ul>
 * @pre net2_socket != NULL.
 **/
int net2_socket_connect(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port);

/**
 * @brief Writes the given data to the given socket.
 * @param net2_socket The net2 socket to write to.
 * @param data The data to be transmitted.
 * @param data_length The data length.
 * @return <ul>
 		       <li>SUCCESS : Non-negative number that is the amount of byte written.
               <li>FAILED : -1, errno is set appropriately.
           </ul>
 * @pre net2_socket != NULL.
 **/
int net2_socket_write(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Reads from the given socket <data_length> bytes and store it into the space pointed by <data>.
 * @param net2_socket The net2 socket to read from.
 * @param data A pointer on the data allocated space.
 * @param data_length The number of bytes to read from the socket.
 * @return <ul>
 			   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes.
               <li>SUCCESS : 0 if the peer has performed an orderly shutdown.
               <li>FAILED : -1, errno is set appropriately.
           </ul>
 * @pre <ul>
            <li>net2_socket != NULL
            <li>data_length is less or equal to the size of the allocated memory area pointed by data.
        </ul>
 **/
int net2_socket_read(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Closes the socket (both for transmission and reception).
 * @param net2_socket the net2 socket to be shutdown.
 * @return <ul>     
 	   	  	   <li> SUCCESS : 0.
 		       <li> FAILURE : -1.
	       </ul>
 * @pre <ul>
            <li>net2_socket != NULL.
            <li>net2_socket contains an active socket.
        </ul>
 **/
int net2_socket_close(struct net2_socket_t* net2_socket);

/**
 * @brief Returns the ip address of the given net2 socket (host order).
 * @param net2_socket The socket to extract the ip from.
 * @return The IP address of the given socket in unsigned integer format (host byte order).
 * @pre net2_socket points to an already allocated memory area.
 **/
unsigned int net2_socket_get_ip(struct net2_socket_t* net2_socket);

/**
 * @brief Returns the port of the given net2 socket (host order).
 * @param net2_socket The socket to extract the ip from.
 * @return The port of the given socket in unsigned integer format (host byte order).
 * @pre net2_socket points to an already allocated memory area.
 **/
unsigned short net2_socket_get_port(struct net2_socket_t* net2_socket);

#endif // NET2_SOCKET_INCLUDED INCLUDED
