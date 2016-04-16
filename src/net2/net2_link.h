/**
 * @file net2_link.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 18/01/2015
 **/

#ifndef NET2_LINK_INCLUDED
#define NET2_LINK_INCLUDED

#include <pthread.h> // pthread_t

#include "net2_socket.h"
#include "net2_protocol.h"

/**
 * @brief Incomplete declaration to avoid errors with the mutual inclusion with net2_protocol.h
 **/
struct net2_message_t;

/**
 * @brief Contains the link being run as well as its thread.
 **/
struct net2_link_thread_linked_element_t
{
    pthread_t* _my_thread;
    struct net2_link_t* _my_link;
    struct net2_link_thread_linked_element_t* _next_link;
};

/**
 * @brief A linked element containing a net2 link, forming a linked list when chained.
 **/
struct net2_link_linked_element_t
{
    struct net2_link_linked_element_t* _next_link; ///< A pointer on the next net2 link
    struct net2_link_t* _my_link; ///< A pointer on the contained net2 link
};

/**
 * @brief Link layer reception unit. It has a function to read from the given socket.
 **/
struct net2_link_rx_t
{
    int (*read) (struct net2_socket_t*, void* data, unsigned int data_length); ///< The function to read from the link.
};

/**
 * @brief Link layer transmission unit. It has a function to write to the given socket.
 **/
struct net2_link_tx_t
{
    int (*write) (struct net2_socket_t*, void* data, unsigned int data_length); ///< The function to write to the link.
};

/**
 * @brief Contains the two links to send and receive data as well as the two function to deal with both.
 **/
struct net2_link_t
{
    struct net2_socket_t* _net2_socket; ///< The net2 socket to communicate with
    struct net2_link_rx_t _net2_link_rx; ///< The link RX
    struct net2_link_tx_t _net2_link_tx; ///< The link TX
    int (*read) (struct net2_link_t* net2_link_rx, void* data, unsigned int data_length); ///< The function to read from the link
    int (*write) (struct net2_link_t* net2_link_tx, void* data, unsigned int data_length); ///< The function to write to the link
};

/**
 * @brief Reads from a link RX.
 * @param net2_socket The net2 socket to read from.
 * @param data A pointer on the buffer to fill from reading.
 * @param data_length The number of bytes to read.
 * @return <ul>
 *		   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes.
 *             <li>SUCCESS : 0 if the peer has performed an orderly shutdown.
 *             <li>FAILED : -1, errno is set appropriately.
 *         </ul>
 * @pre net2_link_rx != NULL
 **/
int net2_link_rx_read(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Writes to a link TX.
 * @param net2_socket The net2 socket to write to.
 * @param data A pointer on the buffer to be written.
 * @param data_length The number of bytes to write.
 * @return <ul>
 *	       <li>SUCCESS : Non-negative number that is the amount of byte written.
 *             <li>FAILED : -1, errno is set appropriately.
 *         </ul>
 **/
int net2_link_tx_write(struct net2_socket_t* net2_socket, void* data, unsigned int data_length);

/**
 * @brief Reads from a link.
 * @param net2_link The link to read from.
 * @param data A pointer on the buffer to fill from reading.
 * @param data_length The number of bytes to read.
 * @return <ul>
 *		   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes.
 *             <li>SUCCESS : 0 if the peer has performed an orderly shutdown.
 *             <li>FAILED : -1, errno is set appropriately.
 *         </ul>
 **/
int net2_link_read(struct net2_link_t* net2_link, void* data, unsigned int data_length);

/**
 * @brief Writes to a link.
 * @param net2_link The link to write to.
 * @param data A pointer on the buffer to be written.
 * @param data_length The number of bytes to write.
 * @return <ul>
 *	       <li>SUCCESS : 0
 *             <li>FAILED
 *             <ul>
 *                 <li>-1 : The remote peer performed an orderly shutdown.
 *                 <li>-2 : Other cases.
 *         </ul>
 **/
int net2_link_write(struct net2_link_t* net2_link, void* data, unsigned int data_length);

/**
 * @brief Converts the given net2 message into raw bytes and write them to the link through net2_link_write function.
 * @param net2_link The net2 link to use to communicate.
 * @param net2_message The net2 message to send..
 * @return <ul>
 *	       <li>SUCCESS : 0.
 *             <li>FAILED : -1.
 *         </ul>
 **/
int net2_link_send(struct net2_link_t* net2_link, struct net2_message_t* net2_message);

/**
 * @brief Creates a link : stores the given net2 socket in the link and assigns all function pointers on the link as well as on its link RX and TX.
 * @param net2_link A pointer to an allocated memory to store in the created link.
 * @param net2_socket The net2 socket to store in the net2 link.
 * @pre <ul>
 *          <li>net2_link != NULL
 *          <li>net2_socket != NULL
 *      </ul>
 **/
void net2_link_create(struct net2_link_t* net2_link, struct net2_socket_t* net2_socket);

/**
 * @brief Compares the given net2_link with the given address + port to determine if there is already a link to the same address and port. A link will be duplicated if is instanced twice from the same sender (e.g from the same address + port).
 * @param net2_link The link to be compared.
 * @param ip_address The IP address to be tested.
 * @param port The port to be tested.
 * @return <ul>
 *             <li>IDENTICAL : != 0
 *             <li>DIFFERENT : 0
 *         </ul>
 **/
int net2_link_compare_to_address_and_port(struct net2_link_t* net2_link, unsigned int ip_address, unsigned short port);

/**
 * @brief Compares the given link and sockets to check if they are communicating to the same address and port. A link will be duplicated if is instanced twice from the same sender (e.g from the same address + port).
 * @param net2_link The link to be compared.
 * @param net2_socket The socket to be compared.
 * @return <ul>
 *             <li>IDENTICAL : != 0
 *             <li>DIFFERENT : 0
 *         </ul>
 **/
int net2_link_compare_to_socket(struct net2_link_t* net2_link, struct net2_socket_t* net2_socket);

/**
 * @brief Compares both net2_links to determine if they are going to clash by communicating to the same address and port. A link will be duplicated if is instanced twice from the same sender (e.g from the same address + port).
 * @param net2_link_a The link to be compared 1.
 * @param net2_link_b The link to be compared 2.
 * @return <ul>
 *             <li>IDENTICAL : != 0
 *             <li>DIFFERENT : 0
 *         </ul>
 **/
int net2_link_compare_to_link(struct net2_link_t* net2_link_a, struct net2_link_t* net2_link_b);

/**
 * @brief Runs a link : reads indefinitely to accept connections with channels for example.
 * @param net2_link_to_run The net2 link to run.
 * @return <ul>
 *             <li>SUCCESS : net2_link_to_run
 *             <li>FAILURE : NULL
 *         </ul>
 * @pre net2_link_to_run != NULL
 **/
void* net2_link_run(void* net2_link_to_run);

/**
 * @brief Initialises the given linked element with the given link. Also sets the next link pointer to NULL;
 * @param link The link to assign to the given linked element.
 * @param element The linked element to initialise.
 **/
void net2_link_init_linked_element(struct net2_link_t* link, struct net2_link_linked_element_t* element);

/**
 * @brief Appends the given link into the given linked list of links. If the link is already is the current_list, nothing is done.
 * @param current_list The link linked list to be modified.
 * @param link The new element to append to the given linked list.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 * @pre <ul>
 *          <li>current_list != NULL
 *          <li>new_element != NULL
 *      </ul>
 **/
int net2_link_append_to_linked_element(struct net2_link_t* link, struct net2_link_linked_element_t** current_list);

#endif // NET2_LINK_INCLUDED INCLUDED
