#ifndef THD_LINK_INCLUDED
#define THD_LINK_INCLUDED

/**
 * @file link.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 20/12/2014
 **/

#include "socket.h"

/**
 * @class net2_link_rx_t
 * @brief Link layer reception unit. It contains the net2 socket to communicate through as well as we the write function.
 **/
struct net2_link_rx_t
{
    struct net2_socket_t* _net2_socket; ///< The socket to communicate through.
    int (*read) (struct net2_link_rx_t* net2_link_rx, unsigned int data_length, void* data); ///< The function to read from the link.
};

/**
 * @class net2_link_tx_t
 * @brief Link layer transmission unit. It contains the net2 socket to communicate through as well as we the write function.
 **/
struct net2_link_tx_t
{
    struct net2_socket_t* _net2_socket; ///< The socket to communicate through.
    int (*write) (struct net2_link_tx_t* net2_link_tx, unsigned int data_length, void* data); ///< The function to write to the link.
};

/**
 * @brief Reads from a link RX.
 * @param net2_link_rx The link RX to read from.
 * @param data_length The number of bytes to read.
 * @param data A pointer on the buffer to fill from reading.
 **/
int net2_read_from_link_rx(struct net2_link_rx_t* net2_link_rx, unsigned int data_length, void* data);

/**
 * @brief Writes to a link TX.
 * @param net2_link_tx The link TX to write to.
 * @param data_length The number of bytes to write.
 * @param data A pointer on the buffer to be written.
 **/
int net2_write_to_link_tx(struct net2_link_tx_t* net2_link_tx, unsigned int data_length, void* data);

/**
 * @class net2_link_t
 * @brief Contains the two links to send and receive data as well as the two function to deal with both.
 **/
struct net2_link_t
{
    struct net2_link_rx_t* _net2_link_rx;
    struct net2_link_tx_t* _net2_link_tx;
    int (*read) (struct net2_link_t* net2_link_rx, unsigned int data_length, void* data);
    int (*write) (struct net2_link_t* net2_link_tx, unsigned int data_length, void* data);
};

/**
 * @brief Reads from a link.
 * @param net2_link The link to read from.
 * @param data_length The number of bytes to read.
 * @param data A pointer on the buffer to fill from reading.
 **/
int net2_read_from_link(struct net2_link_t* net2_link, unsigned int data_length, void* data);

/**
 * @brief Writes to a link.
 * @param net2_link The link to write to.
 * @param data_length The number of bytes to write.
 * @param data A pointer on the buffer to be written.
 **/
int net2_write_to_link(struct net2_link_t* net2_link, unsigned int data_length, void* data);

#endif
