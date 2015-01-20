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
 * @class net2_link_linked_element_t
 * @brief A linked element containing a net2 link, forming a linked list when chained. They are used in the link manager for dynamically growing structure.
 **/
struct net2_link_linked_element_t
{
    struct net2_link_linked_element_t* _next_link; ///< A pointer on the next net2 link
    struct net2_link_t* _my_link; ///< A pointer on the contained net2 link
};

/**
 * @class net2_link_manager_t
 * @brief The link manager structure. It contains a collection of links and ensures that one a link is registered per node.
 **/
struct net2_link_manager_t
{
    struct net2_link_linked_element_t* _net2_links; ///< Net2 links collection
};

/**
 * @brief Returns the link manager instance. Follows the singleton pattern; only one instance can be created at a time.
 * @param net2_link_manager A pointer to store in the net2 link manager instance.
 * @return <ul>
               <li>SUCCESS : 0
               <li>FAILURE : -1
           </ul>
 **/
int net2_link_manager_get_instance(struct net2_link_manager_t** net2_link_manager);

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

