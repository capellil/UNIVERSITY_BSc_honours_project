#include <stdio.h> // printf
#include <stdlib.h> // NULL

#include "net2_node.h"
#include "net2_link_server.h"
#include "net2_link_manager.h"
#include "net2_channel_manager.h"

struct net2_node_t** net2_node_get_instance(void)
{
    static struct net2_node_t* net2_node_singleton = NULL;
    
    return &net2_node_singleton;
}

int net2_node_create(unsigned short port)
{    
    int result = 0;

    struct net2_node_t** net2_node = net2_node_get_instance();
    *net2_node = (struct net2_node_t*)malloc(sizeof(struct net2_node_t));
    
    // TEST : Did the node dynamic allocation succeed ?
    if(*net2_node)
    {
        // Yes, the node dynamic allocation succeeded.
        // TEST : Did the link manager initialisation succeed ?
        if(!net2_link_manager_init())
        {
            // Yes, the link manager initialisation succeeded.
            // TEST : Did the link server initialisation succeed ?
            if(!net2_link_server_init(port))
            {
                // Yes, the link server initialisation succeeded.
                // TEST : Did the channel manager initialisation succeed ?
                if(!net2_channel_manager_init())
                {
                    // Yes, the channel manager initialisation succeeded.
                    #ifdef NET2_DEBUG
                        net2_debug_success();
                    #endif
                }
                else
                {
                    // No, the channel manager initialisation failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("The channel manager initialisation failed.");
                    #endif
                }
            }
            else
            {
                // No, the link server initialisation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The link server initialisation failed.");
                #endif
            }
        }
        else
        {
            // No, the link manager initialisation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The link manager initialisation failed.");
            #endif
        }
    }
    else
    {
        // No, the node dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The node dynamic allocation failed.");
        #endif
    }
        
    return result;
}

int net2_node_init(unsigned short port)
{   
    int result = 0;
    
    // TEST : Is the node not already instanced ?
    if(!(*net2_node_get_instance()))
    {
        // Yes, the node is not already instanced.        
        // TEST : Did the node creation succeed ?
        if(!net2_node_create(port))
        {
            // Yes, the node creation succeeded.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the node creation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The node creation failed.");
            #endif
        }
    }
    else
    {
        // No, the node is already instanced.
        result = -2;
        #ifdef NET2_DEBUG
            net2_debug_failure("The node is already instanced.");
        #endif
    }
    
    return result;
}

int net2_node_connect(struct net2_link_t** net2_link, unsigned int ip_address, unsigned short port)
{
    int result = 0;
    
    bool found = false;
            
    // TEST : Did the link manager research succeed
    if(!net2_link_manager_check_address_and_port(ip_address, port, &found))
    {
        // Yes, the link manager research succeeded.
        // TEST : Is the node at the given address and port is available ?
        if(!found)
        {
            // Yes, the node at the given address and port is available.
            struct net2_socket_t* socket = (struct net2_socket_t*)malloc(sizeof(struct net2_socket_t));
            
            // TEST : Did the socket dynamic allocation succeed ?
            if(socket)
            {
                // Yes, the socket dynamic allocation succeeded.            
                // TEST : Did the socket creation and connection succeed ?
                if(net2_create_and_connect_socket(socket, ip_address, port))
                {
                    // Yes, the socket connection succeeded.
                    signed char data;
                    unsigned int data_length = sizeof(signed char);
                    
                    // TEST : Did the socket read succeed ?
                    if(net2_read_from_socket(socket, (void*)&data, data_length) >= 0)
                    {
                        // Yes, the socket read succeeded.
                        // TEST : Has the connection been allowed ?
                        if(!data)
                        {
                            // Yes, the connection has been allowed.
                            *net2_link = (struct net2_link_t*)malloc(sizeof(struct net2_link_t));
                            
                            // TEST : Did the net2 link dynamic allocation succeed ?
                            if(*net2_link)
                            {
                                // Yes, the net2 link dynamic allocation succeeded.
                                net2_link_create(*net2_link, socket);
                                (*net2_link)->_net2_socket->_address.sin_addr.s_addr = htonl(ip_address);
                                (*net2_link)->_net2_socket->_address.sin_port = htons(port);
                                
                                // TEST : Did the link registration succeed ?
                                if(!net2_link_manager_register_link(*net2_link))
                                {
                                    // Yes, the link registration succeeded.
	                                // TEST : Did the new link run succeed ?
                                    if(!net2_link_server_new_link_to_run(*net2_link))
                                    {
                                        // Yes, the new link run succeeded.
                                        #ifdef NET2_DEBUG
                                            net2_debug_success();
                                        #endif
                                    }
                                    else
                                    {
                                        // Yes, the new link run failed.
                                        result = -1;
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("The new link run failed.");
                                        #endif
                                    }
                                }
                                else
                                {
                                    // No, the link registration failed.
                                    result = -1;
                                    #ifdef NET2_DEBUG
                                        net2_debug_failure("The link registration failed.");
                                    #endif
                                }
                            }
                            else
                            {
                                // No, the net2 link dynamic allocation failed.
                                result = -1;
                                #ifdef NET2_DEBUG
                                    net2_debug_failure("The link registration failed.");
                                #endif
                            }
                        }
                        else
                        {
                            // No, the connection has not been allowed.
                            result = -1;
                            #ifdef NET2_DEBUG
                                net2_debug_failure("The connection has not been allowed.");
                            #endif
                        }
                    }
                    else
                    {
                        // No, the socket read failed.
                        result = -1;
                        #ifdef NET2_DEBUG
                            net2_debug_failure("The socket read failed.");
                        #endif
                    }
                }
                else
                {
                    // No, the socket connection failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("Something went wrong in the creation & connection of the socket.");
                    #endif
                }
            }
            else
            {
                // No, the socket dynamic allocation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The socket dynamic allocation failed.");
                #endif
            }
        }
        else
        {
            // No, the node at the given address and port is not available (e.g there is already a link to it).
            // TEST : Did the link manager succeed to get the link back ?
            if(!net2_link_manager_get_link(net2_link, ip_address, port))
            {
                // Yes, the link manager succeeded to get the link back.
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the link manager failed to get the link back.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The socket creation failed.");
                #endif
            }
        }
    }
    else
    {
        // No, the link manager research failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The link manager research failed.");
        #endif
    }
    
    return result;
}

int net2_node_close()
{
    int result = 0;
    
    struct net2_link_server_t** temp = net2_link_server_get_instance();
    
    // TEST : Is the node not already instanced ?
    if(*temp)
    {
        // Yes, the node is not already instanced.   
        pthread_join(*((*temp)->_server_thread), NULL);
        #ifdef NET2_DEBUG
            net2_debug_success();
        #endif
    }
    else
    {
        // No, the node is not already instanced.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The node is not already instanced.");
        #endif
    }
    
    return result;
}
