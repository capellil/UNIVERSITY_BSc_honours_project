/**
 * @file socket.h
 * @author Capelli Ludovic
 * @version 1.0
 * @date 15/09/2014
 **/

#ifndef THD_SOCKET_INCLUDED
#define THD_SOCKET_INCLUDED

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
 * @pre socket points to an already allocated memory area.
 **/
void net2_print_socket(char* heading, struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket used for communication and stores it in into the passed pointed memory location. Uses TCP/IP protocols.
 * @param net2_socket A pointer to store the created net2 socket in.
 * @return <ul>
 		       <li>SUCCESS : >= 0
               <li>FAILED : -1, errno is set appropriately
           </ul>
 * @pre net2_socket points to an already allocated memory area.
 **/
int net2_create_and_store_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Creates a socket and binds it to the given port. Uses TCP/IP protocols.
 * @param port The port to be used.
 * @param net2_socket A pointer to store the created net2 socket in.
 * @return <ul>
 		       <li>SUCCESS : 0.
               <li>FAILED : Negative number, errno is set appropriately
           </ul>
 * @pre net2_socket points to an already allocated memory area.
 * @post net2_socket contains a socket binded to the port "port".
 **/
int net2_create_and_bind_socket(unsigned short port, struct net2_socket_t* net2_socket);

/**
 * @brief Listens on the given socket to accept connections later on. Does not start any acceptation mechanism though.
 * @param net2_socket The net2 socket to listen on.
 * @return <ul>
               <li>SUCCESS : 0
		       <li>FAILED : different from 0, errno is set appropriately
	       </ul>
 * @pre net2_socket points to an already allocated memory area.
 * @post It is listening on net2_socket.
 **/
int net2_listen_on_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Accept a connection from a client to the given server.
 * @param server The server to listen on and accepting a connection onto.
 * @param client A pointer to store data in.
 * @return <ul>
               <li>SUCCESS : >= 0
 		       <li>FAILED : -1, errno is set appropriately
	       </ul>
 * @pre <ul>
            <li>server points to an already allocated memory area.
            <li>client points to an already allocated memory area.
        </ul>
 * @post client contains the accepted peer information : the socket as well as its address structure (e.g. : IP address and port to name a few).
 **/
int net2_accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client);

/**
 * @brief Connects the given socket to the provided address and port.
 * @param net2_socket The socket to be connected
 * @param address The server address
 * @param port The server port
 * @return <ul>
 			   <li>SUCCESS : 0
               <li>FAILED : -1
           </ul>
 * @pre net2_socket points to an already allocated memory area.
 * @post net2_socket is connected to the socket at the given IP address and port.
 **/
int net2_connect_to_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short port);

/**
 * @brief Writes the given data to the given socket.
 * @param net2_socket The net2 socket to write to.
 * @param data The data to be transmitted.
 * @param data_length The data length.
 * @return <ul>
 		       <li>SUCCESS : Non-negative number that is the amount of byte written
               <li>FAILED : -1, errno is set appropriately
           </ul>
 * @pre net2_socket points to an already allocated memory area.
 * @post "data" has been sent to the socket connected to "net2_socket".
 **/
int net2_write_to_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Reads from the given socket <data_length> bytes and store it into the space pointed by <data>.
 * @param net2_socket The net2 socket to read from.
 * @param data A pointer on the data allocated space.
 * @param data_length The number of bytes to read from the socket.
 * @return <ul>
 			   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes
               <li>SUCCESS : 0 if the peer has performed an orderly shutdown
               <li>FAILED : -1, errno is set appropriately
           </ul>
 * @pre <ul>
            <li>net2_socket points to an already allocated memory area.
            <li>data_length is less or equal to the size of the allocated memory area pointed by data.
        </ul>
 * @post "data_length" bytes have been read from "net_socket" and stored into "data".
 **/
int net2_read_from_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Closes the socket (both for transmission and reception).
 * @param net2_socket the net2 socket to be shutdown.
 * @return <ul>     
 	   	  	   <li> SUCCESS : 0
 		       <li> FAILURE : -1
	       </ul>
 * @pre <ul>
            <li>net2_socket points to an already allocated memory area.
            <li>net2_socket contains an active socket.
        </ul>
 * @post The socket contained in "net2_socket" is closed.
 **/
int net2_close_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Returns the ip address of the given net2 socket (host order_.
 * @param net2_socket The socket to extract the ip from.
 * @return The IP address of the given socket in unsigned integer format (host byte order).
 * @pre net2_socket points to an already allocated memory area.
 **/
unsigned int net2_get_ip_of_socket(struct net2_socket_t* net2_socket);

#endif
