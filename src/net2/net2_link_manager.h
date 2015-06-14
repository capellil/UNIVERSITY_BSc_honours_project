/**
 * @file net2_link_manager.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 05/01/2015
 **/

#ifndef NET2_LINK_MANAGER_INCLUDED
#define NET2_LINK_MANAGER_INCLUDED

#include <stdlib.h> // NULL | malloc
#include <stdbool.h> // bool

#include "net2_link.h"

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

/**
 * @brief The link manager structure. It contains a collection of links and ensures that one a link is registered per node.
 **/
struct net2_link_manager_t
{
    struct net2_link_linked_element_t* _links; ///< Net2 links collection
};

/**
 * @brief Returns a pointer on the link manager (which is itself a pointer on the singleton instance, hence "**").
 * @return A pointer on the link manager.
 **/
struct net2_link_manager_t** net2_link_manager_get_instance(void);

/**
 * @brief Instances the net2 link manager singleton.
 * @return <ul>
			   <li>SUCCESS : 0
			   <li>FAILURE : -1
		   </ul>
 **/
int net2_link_manager_create();

/**
 * @brief Inits the link manager : if the link server singleton has not been instanced yet, it is instanced with a NULL list of links.
 * @return <ul>
			   <li>SUCCESS : 0
			   <li>FAILURE : -1
			   <li>ALREADY INSTANCED : -2
		   </ul>
 **/
int net2_link_manager_init();

/**
 * @brief Registers the given link into the net2 link manager's links collection. It will check first that no link related to the same node already exists.
 * @param net2_link A pointer on the link to register.
 * @return <ul>
               <li>SUCCESS : 0
               <li>FAILURE : -1
           </ul>
 * @pre <ul>
            <li>net2_link_manager != NULL
            <li>net2_link != NULL
        </ul>
 **/
int net2_link_manager_register_link(struct net2_link_t* net2_link);

/**
 * @brief Retrieves the link corresponding to the given IP address and the port.
 * @param net2_link A pointer on a net2_link to store the found pointer or NULL if not found.
 * @param ip_address The IPv4 address to check.
 * @param port The port to check.
 **/
int net2_link_manager_get_link(struct net2_link_t** net2_link, unsigned int ip_address, unsigned short port);

/**
 * @brief Checks if the given IP address + port exist in the link manager links. (e.g If another link already communicate to this address + port)
 * @param address The IP address to check.
 * @param port The port to check.
 * @param found A pointer on a boolean to store the comparison result in.
 * @return <ul>
               <li>SUCCESS : 0
               <li>FAILURE : -1
           </ul>
 **/
int net2_link_manager_check_address_and_port(unsigned int address, unsigned short port, bool* found);

/**
 * @brief Checks if the given socket exists in the link manager links.
 * @param net2_socket The net2 socket to be compared with all registered links sockets.
 * @param found A pointer on a boolean to store the comparison result in.
 * @return <ul>
               <li>SUCCESS : 0
               <li>FAILURE : -1
           </ul>
 **/
int net2_link_manager_check_socket(struct net2_socket_t* net2_socket, bool* found);

#endif // NET2_LINK_MANAGER_INCLUDED INCLUDED

