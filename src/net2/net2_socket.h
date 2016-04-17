/**
 * @file net2_socket.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 18/01/2015
 **/

#ifndef NET2_SOCKET_INCLUDED
#define NET2_SOCKET_INCLUDED

#include <sys/types.h> // AF_INET, SOCK_STREAM...
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP
#include <arpa/inet.h> // inet_ntoa
#include "net2_debug.h"

/**
 * @brief Structure containing the socket and its address structure.
 **/
struct net2_socket_t
{
    int _socket; ///< The socket file descriptor (e.g "The socket itself")
    struct sockaddr_in _address; ///< The socket address structure (e.g : Its address, port, address family and so on).
};

/**
 * @brief Creates a socket and binds it to the given port. Uses TCP/IP protocols.
 * @param[out] net2_socket A pointer to store the created net2 socket in.
 * @param[in] port The port to be used. (Host byte order)
 * @return <ul>
 *             <li>ON SUCCESS : 0.</li>
 *             <li>ON FAILURE : 
 *             <ul>
 *                 <li>-1 : socket creation failed.
 *                 <li>-2 : socket SOL_REUSEADDR option set failed.
 *                 <li>-3 : socket binding failed when trying with given port.
 *                 <li>-4 : the socket information extraction failed.
 *             </ul>
 *             </li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: net2_socket is bound to the given port.</li>
 *           <li>ON FAILURE: net2_socket is left untouched.</li>
 *       </ul>
 **/
int net2_create_and_bind_socket(struct net2_socket_t* net2_socket, unsigned short int port);

/**
 * @brief Listens on the given socket to accept connections later on. 
 *
 * IMPORTANT: The listening does not start any acceptation mechanism though.
 * @param[in] net2_socket The net2 socket to listen on.
 * @return <ul>
 *             <li>SUCCESS : 0.</li>
 *             <li>FAILED : -1.</li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: net2_socket listens.</li>
 *           <li>ON FAILURE: net2_socket is left untouched.</li>
 *       </ul>
 **/
int net2_listen_from_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Accepts a connection from the given server net2 socket and stores it into the given client net2 socket.
 * @param[in] server The server to listen on and accepting a connection onto.
 * @param[out] client A pointer to store the client net2 socket in.
 * @return <ul>
 *             <li>ON SUCCESS : 0.</li>
 *             <li>ON FAILURE : -1.</li>
 *         </ul>
 * @pre <ul>
 *          <li>server points to a valid net2_socket_t structure.</li>
 *          <li>client points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: client contains the net2_socket of the accepted socket.</li>
 *           <li>ON FAILURE: client is left untouched.</li>
 *       </ul>
 **/
int net2_accept_from_socket(struct net2_socket_t* server, struct net2_socket_t* client);

/**
 * @brief Creates and connects the given socket to the provided address and port.
 * @param[out] net2_socket A pointer to store the net2_socket_t structure once created and connected.
 * @param[in] address The IP address to connect to. (Host byte order)
 * @param[in] port The port to connect to. (Host byte order)
 * @return <ul>
 *             <li>ON SUCCESS : 0.</li>
 *             <li>ON FAILURE : 
 *             <ul>
 *                 <li>-1: Something went wrong in the TCP/Ipv4 socket creation.</li>
 *                 <li>-2: Socket SOL_REUSEADDR option set failed.</li>
 *                 <li>-3: The socket connection failed.</li>
 *             </ul>
 *             </li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: net2_socket contains the socket created and connected to the remote socket at the given address and port.</li>
 *           <li>ON FAILURE: net2_socket is left untouched.</li>
 *       </ul>
 **/
int net2_create_and_connect_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short int port);

/**
 * @brief Connects the given socket to the provided address and port.
 * @param[in] net2_socket The socket to be connected.
 * @param[in] address The IP address to connect to. (Host byte order)
 * @param[in] port The port to connect to. (Host byte order)
 * @return <ul>
 *             <li>ON SUCCESS : 0.</li>
 *             <li>ON FAILURE : -1.</li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: net2_socket is connected to the socket at the given address and port.</li>
 *           <li>ON FAILURE: net2_socket is not connected to the socket at the given address and port.</li>
 *       </ul>
 **/
int net2_connect_socket(struct net2_socket_t* net2_socket, unsigned int address, unsigned short int port);

/**
 * @brief Writes the given data to the given socket.
 * @param[in] net2_socket The net2 socket to write to.
 * @param[in] data The data to be transmitted.
 * @param[in] data_length The data length.
 * @return <ul>
 *             <li>ON SUCCESS : 0.</li>
 *             <li>ON FAILURE : -1.</li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *          <li>data points to a valid net2_socket_t structure of size data_length.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: the message is sent to the socket net2_socket is connected to.</li>
 *           <li>ON FAILURE: the message is not sent to the remote socket.</li>
 *       </ul>
 **/
int net2_write_to_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Reads up to data_length bytes from the given socket and stores it into the space pointed by data.
 * @param[in] net2_socket The net2 socket to read from.
 * @param[out] data A pointer on the data allocated space.
 * @param[in] data_length The number of bytes to read from the socket.
 * @return <ul>
 *             <li>ON SUCCESS : 0.
 *             <li>ON FAILURE:
 *             <ul>
 *                 <li>-1 : if the peer has performed an orderly shutdown.
 *                 <li>-2 : otherwise.
 *             </ul>
 *             </li>
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a non-NULL net2_socket.
 *          <li>data_length is less or equal to the size of the allocated memory area pointed by data.
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: data points to a.</li>
 *           <li>ON FAILURE: the message is not sent to the remote socket.</li>
 *       </ul>
 **/
int net2_read_from_socket(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Closes the socket (both for transmission and reception).
 * @param[out] net2_socket the net2 socket to be shutdown.
 * @return <ul>     
 *             <li>ON SUCCESS : 0.
 *             <li>ON FAILURE : -1.
 *         </ul>
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: the net2_socket provided is closed.</li>
 *           <li>ON FAILURE: the net2_socket is not closed.</li>
 *       </ul>
 **/
int net2_close_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Returns - in host byte order - the IP address of the given net2 socket.
 * @param[in] net2_socket The net2 socket to extract the IP from.
 * @return The IP address - in host byte order - of the given socket.
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 **/
unsigned int net2_get_ip_of_socket(struct net2_socket_t* net2_socket);

/**
 * @brief Returns - in host byte order - the port of the given net2 socket.
 * @param[in] net2_socket The net2 socket to extract the port from.
 * @return The port - in host byte order - of the given socket.
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 **/
unsigned short int net2_get_port_of_socket(struct net2_socket_t* net2_socket);

/**
 * @brief This function creates a net2_socket and connects it to the given remote socket.
 * @param[in] ip_address ASCII representation of an IPv4 address in host byte order.
 * @param[in] port The port number of the remote socket in host byte order.
 * @param[out] client_socket A pointer to the net2_socket_t structure to use to store the remote net2 socket.
 * @return <ul>
 *            <li>ON SUCCESS: 0.</li>
 *            <li>ON FAILURE:
 *            <ul>
 *                <li>-1: The given ip address is not a valid IPv4 address.</li>
 *                <li>-2: The family variable does not contain a valid internet family.</li>
 *                <li>-3: The client socket creation failed.</li>
 *                <li>-4: The socket connection failed.</li>
 *            </ul>
 *            </li>
 *        </ul>
 * @pre <ul>
 *          <li>ip_address contains a '\0' terminated character string.</li>
 *          <li>client_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: client_socket contains the socket connected to the given address and port.</li>
 *           <li>ON FAILURE: client_socket is left untouched.</li>
 *       </ul>
 **/
int net2_create_client_socket(char* ip_address, unsigned short int port, struct net2_socket_t* client_socket);

/**
 * @brief This function creates a net2_socket and opens a server on the given port.
 * @param[in] port The port number to use for the server in host byte order.
 * @param[out] server_socket A pointer to the net2_socket_t structure to use to store the server net2 socket.
 * @return <ul>
 *            <li>ON SUCCESS: 0.</li>
 *            <li>ON FAILURE:
 *            <ul>
 *                <li>-1: Something went wrong in the creation & binding process.</li>
 *                <li>-2: Something went wrong in the listening process.</li>
 *            </ul>
 *            </li>
 *        </ul>
 * @pre <ul>
 *          <li>server_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 * @post <ul>
 *           <li>ON SUCCESS: server_socket contains the server socket listening on the given port.</li>
 *           <li>ON FAILURE: server_socket is left untouched.</li>
 *       </ul>
 **/
int net2_create_server_socket(unsigned short int port, struct net2_socket_t* server_socket);

/**
 * @brief Prints out all the information in the given net2_socket next to the given header.
 * @param heading The message to be displayed as a heading.
 * @param net2_socket The socket to be printed out.
 * @pre <ul>
 *          <li>net2_socket points to a valid net2_socket_t structure.</li>
 *      </ul>
 **/
void net2_print_socket(char* heading, struct net2_socket_t* net2_socket);

#endif // NET2_SOCKET_INCLUDED INCLUDED