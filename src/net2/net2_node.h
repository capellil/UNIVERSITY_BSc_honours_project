/**
 * @file net2_node.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#ifndef NET2_NODE_INCLUDED
#define NET2_NODE_INCLUDED

#include "net2_socket.h"
#include "net2_link.h"

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

/**
 * @brief Structure used to identify a node : IP address + port (because scope : TCP / IP only).
 */
struct net2_node_id_t
{
    unsigned int _address; ///< The node IP address in host byte order.
    unsigned short int _port; ///< The node port number
};

/**
 * @brief Structure used to represent the node itself. It contains its node id structure.
 **/
struct net2_node_t
{ 
    struct net2_node_id_t _net2_node_id; ///< The node id
};

/**
 * @brief This function initiates a unique instance of the node.
 *
 * This implement a singleton pattern and relies on a unique instance of the node.
 * @return A pointer on the node.
 **/
struct net2_node_t** net2_node_get_instance(void);

/**
 * @brief If the net2 node singleton has not been instanced yet, it is created. That means it instances the link manager, also the link server on the given port.
 * @param port The port to be used to instance the link server (e.g the port to listen on).
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_node_create(unsigned short port);

/**
 * @brief Inits the node : if the node singleton has not been instanced yet, it is instanced.
 * @param port The port to be used to instance the link server (e.g the port to listen on).
 * @return <ul>
 *    		   <li>SUCCESS : 0
 *    		   <li>FAILURE : -1
 *    		   <li>ALREADY INSTANCED : -2
 *    	   </ul>
 **/
int net2_node_init(unsigned short port);

/**
 * @brief Connects the node to the remote node at the given ip address and port.
 * @param net2_link A pointer on a pointer on a link to store the corresponding contained socket.
 * @param ip_address The IPv4 address of the remote node.
 * @param port The port of the remote node.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_node_connect(struct net2_link_t** net2_link, unsigned int ip_address, unsigned short port);

/**
 * @brief Waits for the link server thread to finish.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_node_close();

#endif // NET2_NODE_INCLUDED INCLUDED
