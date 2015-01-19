#include <string.h> // strlen | memcpy
#include <stdlib.h> // malloc

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

#include "net2_link.h"

int net2_link_rx_read(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
    #ifdef NET2_DEBUG
        int result = net2_socket_read(net2_socket, data, data_length);
        
        // Did the read succeed ?
        if(result > 0)
        {
            // Yes, the read succeeded.
            net2_debug_success("net2_link_rx_read");
        }
        else if(!result)
        {
            // The peer has performed an orderly shutdown.
            net2_debug_failure("net2_link_rx_read", "The peer has performed an orderly shutdown.");
        }
        else
        {
            // No, the read failed.
            net2_debug_failure("net2_link_rx_read", "The read failed.");
        }
        
        return result;
    #else        
        return net2_socket_read(net2_socket, data, data_length);
    #endif
}

int net2_link_tx_write(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
    #ifdef NET2_DEBUG
        int result = net2_socket_write(net2_socket, data, data_length);
        
        // Did the write succeed ?
        if(result >= 0)
        {
            // Yes, the write succeeded.
            net2_debug_success("net2_link_tx_write");
        }
        else
        {
            // No, the write failed.
            net2_debug_failure("net2_link_tx_write", "The write failed.");
        }
        
        return result;
    #else        
        return net2_socket_write(net2_socket, data, data_length);
    #endif
}

int net2_link_read(struct net2_link_t* net2_link, void* data, unsigned int data_length)
{
    #ifdef NET2_DEBUG
        int result = net2_link_rx_read(net2_link->_net2_socket, data, data_length);
        
        // Did the read succeed ?
        if(result > 0)
        {
            // Yes, the read succeeded.
            net2_debug_success("net2_link_read");
        }
        else if(!result)
        {
            // The peer has performed an orderly shutdown.
            net2_debug_failure("net2_link_read", "The peer has performed an orderly shutdown.");
        }
        else
        {
            // No, the read failed.
            net2_debug_failure("net2_link_read", "The read failed.");
        }
        
        return result;
    #else        
        return net2_link_rx_read(net2_link->_net2_socket, data, data_length);
    #endif
}

int net2_link_write(struct net2_link_t* net2_link, void* data, unsigned int data_length)
{
    #ifdef NET2_DEBUG
        int result = net2_link_tx_write(net2_link->_net2_socket, data, data_length);
        
        // Did the write succeed ?
        if(result >= 0)
        {
            // Yes, the write succeeded.
            net2_debug_success("net2_link_tx_write");
        }
        else
        {
            // No, the write failed.
            net2_debug_failure("net2_link_tx_write", "The write failed.");
        }
        
        return result;
    #else        
        return net2_link_tx_write(net2_link->_net2_socket, data, data_length);
    #endif
}

void net2_link_create(struct net2_link_t* net2_link, struct net2_socket_t* net2_socket)
{    
    net2_link->_net2_socket = net2_socket;
    net2_link->read = &net2_link_read;
    net2_link->write = &net2_link_write;
    net2_link->_net2_link_rx.read = &net2_link_rx_read;
    net2_link->_net2_link_tx.write = &net2_link_tx_write;
}

int net2_link_compare_to_address_and_port(struct net2_link_t* net2_link, unsigned int ip_address, unsigned short port)
{    
    return (net2_link->_net2_socket->_address.sin_addr.s_addr == htonl(ip_address) &&
            net2_link->_net2_socket->_address.sin_port == htons(port));
}

int net2_link_compare_to_socket(struct net2_link_t* net2_link, struct net2_socket_t* net2_socket)
{
    return net2_link_compare_to_address_and_port(net2_link, net2_socket_get_ip(net2_socket), net2_socket_get_port(net2_socket));
}

int net2_link_compare_to_link(struct net2_link_t* net2_link_a, struct net2_link_t* net2_link_b)
{
    return net2_link_compare_to_socket(net2_link_a, net2_link_b->_net2_socket);
}
