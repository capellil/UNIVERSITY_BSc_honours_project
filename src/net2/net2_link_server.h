/**
 * @file net2_link_server.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#ifndef NET2_LINK_SERVER_INCLUDED
#define NET2_LINK_SERVER_INCLUDED

#include <pthread.h>

#include "net2_socket.h"
#include "net2_link.h"

/**
 * @brief The link server is in charge of handling incoming connections requests from remote nodes which want to create a communication between one of their channel and on of its channel. It therefore checks from the link manager than those channels are not already connected with a net2 link.
 **/
struct net2_link_server_t
{
    struct net2_socket_t _server_socket; ///< Socket from where it will accept connections from.
    pthread_t* _server_thread; ///< Thread running indefinitely
    struct net2_link_thread_linked_element_t* _link_threads;
};

/**
 * @brief Returns a pointer on the link server (which is itself a pointer on the singleton instance, hence "**").
 * @return A pointer on the link server.
 **/
struct net2_link_server_t** net2_link_server_get_instance(void);

/**
 * @brief Instances the net2 link server singleton.
 * @param port The port to listen on.
 * @return <ul>
			   <li>SUCCESS : 0
			   <li>FAILURE : -1
		   </ul>
 **/
int net2_link_server_create(unsigned short port);

/**
 * @brief Inits the link server : if the link server singleton has not been instanced yet, it is instanced and starts listening & accepting incoming connections requests.
 * @param port The port to listen on.
 * @return <ul>
			   <li>SUCCESS : 0
			   <li>FAILURE : -1
			   <li>ALREADY INSTANCED : -2
		   </ul>
 **/
int net2_link_server_init(unsigned short port);

/**
 * @brief Runs the net2 link server : starts listening and accepting incoming connections requests. It has a fancy signature because it will be executed in a different thread and thread format in C is : "void* (*) (void*)".
 * @param net2_link_server_to_run A pointer on the net2 link server to be started.
 * @return <ul>
		       <li>SUCCESS : net2_link_server_to_run
               <li>FAILURE : NULL
           </ul>
 * @pre net2_link_server_to_run != NULL
 **/
void* net2_link_server_run(void* net2_link_server_to_run);

/**
 * @brief Runs the given link in a new thread to read message from this link continuously.
 * @param link_to_run The link to run.
 * @return <ul>
			   <li>SUCCESS : 0
			   <li>FAILURE : -1
		   </ul>
 **/
int net2_link_server_new_link_to_run(struct net2_link_t* link_to_run);

#endif // NET2_LINK_SERVER_INCLUDED INCLUDED
