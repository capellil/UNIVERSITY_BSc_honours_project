#ifndef THD_LINK_INCLUDED
#define THD_LINK_INCLUDED

/**
 * @file net2_link.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 20/12/2014
 **/

#include "net2_socket.h"

/**
 * @class net2_link_rx_t
 * @brief Link layer reception unit. It contains the net2 socket to listen and accept on as well as the partner socket to read from. Finally, it also contains the read function.
 **/
struct net2_link_rx_t
{
    struct net2_socket_t* _net2_socket; ///< The net2 socket to accept connections from.
    struct net2_socket_t* _net2_partner_socket; ///< The net2 socket to read from.
    int (*read) (struct net2_link_rx_t* net2_link_rx, void* data, unsigned int data_length); ///< The function to read from the link.
};

/**
 * @class net2_link_tx_t
 * @brief Link layer transmission unit. It contains the net2 socket to communicate through as well as we the write function.
 **/
struct net2_link_tx_t
{
    struct net2_socket_t* _net2_socket; ///< The socket to communicate through.
    int (*write) (struct net2_link_tx_t* net2_link_tx, void* data, unsigned int data_length); ///< The function to write to the link.
};

/**
 * @brief Creates a link RX by creating its socket and binding it to the given port. Does not start listening nor accepting process.
 * @param net2_link_rx A pointer to an already allocated memory area to store in the created link RX.
 * @param port The port to listen and accept on.
 * @return <ul>
               <li>SUCCESS : 0.
               <li>FAILURE : Negative number, errno is set appropriately.
           </ul>
 * @pre net2_link_rx points to an already allocated memory area.
 * @post <ul>
             <li>SUCCESS : <ul>
                               <li>net2_link_rx->_net2_socket created and binded to the port "port". 
                               <li>net2_link_rx->_partner_socket = NULL
                               <li>net2_link_rx->read = &net2_read_from_link_rx.
                           </ul>
             <li>FAILURE : net2_link_rx unchanged.
         </ul>
 **/
int net2_create_link_rx(struct net2_link_rx_t* net2_link_rx, unsigned short port);

/**
 * @brief Creates a link TX. Does not start the connection process.
 * @param net2_link_tx A pointer to an already allocated memory area to store in the created link TX.
 * @return <ul>
               <li>SUCCESS : 0.
               <li>FAILURE : -1.
           </ul>
 * @pre net2_link_tx points to an allocated memory area.
 * @post <ul>
             <li>SUCCESS : net2_link_tx->_net2_socket created and net2_link_tx->write = &net2_write_to_link_tx.
             <li>FAILURE : net2_link_tx unchanged.
         </ul>
 **/
int net2_create_link_tx(struct net2_link_tx_t* net2_link_tx);

/**
 * @brief Reads from a link RX.
 * @param net2_link_rx The link RX to read from.
 * @param data A pointer on the buffer to fill from reading.
 * @param data_length The number of bytes to read.
 * @return <ul>
 			   <li>SUCCESS : Non-negative and non-null number that is the number of read bytes.
               <li>SUCCESS : 0 if the peer has performed an orderly shutdown.
               <li>FAILED : -1, errno is set appropriately.
           </ul>
 * @pre net2_link_rx != NULL
 **/
int net2_read_from_link_rx(struct net2_link_rx_t* net2_link_rx, void* data, unsigned int data_length);

/**
 * @brief Writes to a link TX.
 * @param net2_link_tx The link TX to write to.
 * @param data A pointer on the buffer to be written.
 * @param data_length The number of bytes to write.
 * @return <ul>
 		       <li>SUCCESS : Non-negative number that is the amount of byte written.
               <li>FAILED : -1, errno is set appropriately.
           </ul>
 **/
int net2_write_to_link_tx(struct net2_link_tx_t* net2_link_tx, void* data, unsigned int data_length);

/**
 * @class net2_link_t
 * @brief Contains the two links to send and receive data as well as the two function to deal with both.
 **/
struct net2_link_t
{
    struct net2_link_rx_t* _net2_link_rx; ///< The link RX
    struct net2_link_tx_t* _net2_link_tx; ///< The link TX
    int (*read) (struct net2_link_t* net2_link_rx, void* data, unsigned int data_length); ///< The function to read from the link
    int (*write) (struct net2_link_t* net2_link_tx, void* data, unsigned int data_length); ///< The function to write to the link
};

/**
 * @brief Reads from a link.
 * @param net2_link The link to read from.
 * @param data A pointer on the buffer to fill from reading.
 * @param data_length The number of bytes to read.
 **/
int net2_read_from_link(struct net2_link_t* net2_link, void* data, unsigned int data_length);

/**
 * @brief Writes to a link.
 * @param net2_link The link to write to.
 * @param data A pointer on the buffer to be written.
 * @param data_length The number of bytes to write.
 **/
int net2_write_to_link(struct net2_link_t* net2_link, void* data, unsigned int data_length);

/**
 * @brief Creates a client link : the one sending packets.
 * @param net2_link A pointer to an allocated memory to store in the created link.
 * @param client_port The port of the link RX, to receive communications on.
 * @param server_address The address of the partner (e.g : server).
 * @param server_port The port of the partner (e.g : server).
 * @return <ul>
               <li>SUCCESS : >= 0
               <li>FAILURE : < 0
           </ul>
 * @pre net2_link points to an allocated memory area.
 **/
int net2_create_client_link(struct net2_link_t* net2_link, unsigned short client_port, unsigned int server_address, unsigned short server_port);

/**
 * @brief Creates a server link : the one receiving packets.
 * @param net2_link A pointer to an allocated memory to store in the created link.
 * @param server_port The port of the link RX, to receive communications on.
 * @return <ul>
               <li>SUCCESS : >= 0
               <li>FAILURE : < 0
           </ul>
 * @pre net2_link points to an allocated memory area.
 **/
int net2_create_server_link(struct net2_link_t* net2_link, unsigned short server_port);

#endif
