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
                        net2_debug_success("net2_node_create");
                    #endif
                }
                else
                {
                    // No, the channel manager initialisation failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_node_create", "The channel manager initialisation failed.");
                    #endif
                }
            }
            else
            {
                // No, the link server initialisation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_node_create", "The link server initialisation failed.");
                #endif
            }
        }
        else
        {
            // No, the link manager initialisation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_node_create", "The link manager initialisation failed.");
            #endif
        }
    }
    else
    {
        // No, the node dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_node_create", "The node dynamic allocation failed.");
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
                net2_debug_success("net2_node_init");
            #endif
        }
        else
        {
            // No, the node creation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_node_init", "The node creation failed.");
            #endif
        }
    }
    else
    {
        // No, the node is already instanced.
        result = -2;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_node_init", "The node is already instanced.");
        #endif
    }
    
    return result;
}

int net2_node_connect(struct net2_link_t* net2_link, unsigned int ip_address, unsigned short port)
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
            struct net2_socket_t socket;
            
            // TEST : Did the socket creation succeed ?
            if(net2_socket_create(&socket) >= 0)
            {
                // Yes, the socket creation succeeded.
                // TEST : Did the socket connection succeed ?
                if(!net2_socket_connect(&socket, ip_address, port))
                {
                    // Yes, the socket connection succeeded.
                    signed char data;
                    unsigned int data_length = sizeof(signed char);
                    
                    // TEST : Did the socket read succeed ?
                    if(net2_socket_read(&socket, (void*)&data, data_length) >= 0)
                    {
                        // Yes, the socket read succeeded.
                        // TEST : Has the connection been allowed ?
                        if(!data)
                        {
                            // Yes, the connection has been allowed.
                            net2_link_create(net2_link, &socket);
                            
                            // TEST : Did the link registration succeed ?
                            if(!net2_link_manager_register_link(net2_link))
                            {
                                // Yes, the link registration succeeded.
                                #ifdef NET2_DEBUG
                                    net2_debug_success("net2_node_connect");
                                #endif
                            }
                            else
                            {
                                // No, the link registration failed.
                                result = -1;
                                #ifdef NET2_DEBUG
                                    net2_debug_failure("net2_node_connect", "The link registration failed.");
                                #endif
                            }
                        }
                        else
                        {
                            // No, the connection has not been allowed.
                            result = -1;
                            #ifdef NET2_DEBUG
                                net2_debug_failure("net2_node_connect", "The connection has not been allowed.");
                            #endif
                        }
                    }
                    else
                    {
                        // No, the socket read failed.
                        result = -1;
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_node_connect", "The socket read failed.");
                        #endif
                    }
                }
                else
                {
                    // No, the socket connection failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_node_connect", "The socket connection failed.");
                    #endif
                }
            }
            else
            {
                // No, the socket creation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_node_connect", "The socket creation failed.");
                #endif
            }
        }
        else
        {
            // No, the node at the given address and port is not available (e.g there is already a link to it).
            // TEST : Did the link manager succeed to get the link back ?
            if(net2_link_manager_get_link(net2_link, ip_address, port))
            {
                // Yes, the link manager succeeded to get the link back.
                #ifdef NET2_DEBUG
                    net2_debug_success("net2_node_connect");
                #endif
            }
            else
            {
                // No, the link manager failed to get the link back.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_node_connect", "The socket creation failed.");
                #endif
            }
        }
    }
    else
    {
        // No, the link manager research failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_node_connect", "The link manager research failed.");
        #endif
    }
    
    return result;
}
