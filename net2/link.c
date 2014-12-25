#include <string.h> // strlen | memcpy
#include <stdlib.h> // malloc

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

#include "link.h"

int net2_read_from_link_rx(struct net2_link_rx_t* net2_link_rx, void* data, unsigned int data_length)
{
    return net2_read_from_socket(net2_link_rx->_net2_socket, data, data_length);
}

int net2_write_to_link_tx(struct net2_link_tx_t* net2_link_tx, void* data, unsigned int data_length)
{
    return net2_write_to_socket(net2_link_tx->_net2_socket, data, data_length);
}

int net2_create_link_rx(struct net2_link_rx_t* net2_link_rx, unsigned short port)
{
    int result = 0;
    // "net2_socket" dynamical allocation
    struct net2_socket_t* net2_socket = (struct net2_socket_t*)malloc(sizeof(struct net2_socket_t));
    
    // TEST : Is "net2_socket" dynamic allocation OK ?
    if(net2_socket)
    {
        // "net2_socket" dynamic allocation OK
        // Creates and binds the socket to the given port (e.g : "port")
        result = net2_create_and_bind_socket(port, net2_socket);
        
        // TEST : Is "net2_socket2" creation and binding OK ?
        if(!result)
        {
            // "net2_socket2" creation and binding OK 
            net2_link_rx->_net2_socket = net2_socket;
            net2_link_rx->_net2_partner_socket = NULL;
            net2_link_rx->read = &net2_read_from_link_rx;
            #ifdef NET2_DEBUG
                net2_debug_success("net2_create_link_rx");
            #endif
        }
        else
        {
            // "net2_socket2" creation and binding failed 
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_create_link_rx", "net2_socket creation and binding failed");
            #endif
        }
    }
    else
    {
        // "net2_socket" dynamic allocation failed
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_create_link_rx", "net2_socket dynamic allocation failed");
        #endif
    }
    
    return result;
}

int net2_create_link_tx(struct net2_link_tx_t* net2_link_tx)
{
    int result = 0;
    // "net2_socket" dynamic allocation
    struct net2_socket_t* net2_socket = (struct net2_socket_t*)malloc(sizeof(struct net2_socket_t));
    
    // TEST : Is "net2_socket" dynamic allocation OK ?
    if(net2_socket)
    {
        // "net2_socket" dynamic allocation OK
        // net2_socket creation
        result = net2_create_and_store_socket(net2_socket);
        
        // TEST : Is net2_socket creation OK ?
        if(result >= 0)
        {
            // net2_socket creation OK
            net2_link_tx->_net2_socket = net2_socket;
            net2_link_tx->write = &net2_write_to_link_tx;
            #ifdef NET2_DEBUG
                net2_debug_success("net2_create_link_tx");
            #endif
        }
        else
        {
            // net2_socket creation failed
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_create_link_tx", "net2_socket creation failed");
            #endif
        }
    }
    else
    {
        // "net2_socket" dynamic allocation failed
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_create_link_tx", "net2_socket dynamic allocation failed");
        #endif
    }
    
    return result;
}

int net2_read_from_link(struct net2_link_t* net2_link, void* data, unsigned int data_length)
{
    int result = net2_read_from_link_rx(net2_link->_net2_link_rx, data, data_length);
    
    // TEST : Did link RX reading succeed ?
    if(!result)
    {
        // Link RX reading succeeded.
        // Send the acknowledge
        char data[] = "SALUT";
        unsigned int data_length = strlen(data);
        
        result = net2_write_to_link_tx(net2_link->_net2_link_tx, data, data_length);
        
        // TEST : Link RX acknowledge message.
        if(result)
        {
            // Link RX acknowledge message successfully sent
            #ifdef NET2_DEBUG
                net2_debug_success("net2_read_from_link");
            #endif
        }
        else
        {
            // Link RX acknowledge message sending failed
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_read_from_link", "Link RX acknowledge message sending failed");
            #endif
        }
    }
    else
    {
        // Link RX reading failed.
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_read_from_link", "link RX reading failed");
        #endif
    }
    
    return result;
}

int net2_write_to_link(struct net2_link_t* net2_link, void* data, unsigned int data_length)
{
    int result = net2_write_to_link_tx(net2_link->_net2_link_tx, data, data_length);
    
    // TEST : Is the message transmission OK?
    if(!result)
    {
        // The message transmission is OK
        // Read the acknowledge message
        unsigned int data_length = 2;
        char data[data_length];
        
        result = net2_read_from_link_rx(net2_link->_net2_link_rx, data, data_length);
        
        // TEST : Is the acknowledge message reading OK ?
        if(!result)
        {
            // Acknowledge message reading OK
            #ifdef NET2_DEBUG
                net2_debug_success("net2_write_to_link");
            #endif
        }
        else
        {
            // Acknowledge message reading failed
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_write_to_link", "Acknowledge message reading failed");
            #endif
        }
    }
    else
    {
        // The message transmission failed
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_write_to_link", "The message transmission failed");
        #endif
    }
    
    return result;
}

int net2_create_client_link(struct net2_link_t* net2_link, unsigned short client_port, unsigned int server_address, unsigned short server_port)
{
    int result = 0;
    // Dynamically allocates the net2 link RX
    struct net2_link_rx_t* net2_link_rx = (struct net2_link_rx_t*)malloc(sizeof(struct net2_link_rx_t));
    
    // TEST : Is "net_link_rx" dynamic allocation OK ?
    if(net2_link_rx)
    {  
        // Dynamic allocation OK
        // Creates the net2 link RX with the given port (e.g "client_port")
        result = net2_create_link_rx(net2_link_rx, client_port);
        
        // TEST : Net2 link RX creation OK ?
        if(!result)
        {
            // Net2 link RX creation OK
            // Starts listening on the net2 link RX
            result = net2_listen_on_socket(net2_link_rx->_net2_socket);
            
            // TEST : Did net2 link RX listening succeed ?
            if(!result)
            {
                // Net2 link RX listening succeeded
                // Dynamically allocates the net2 link TX
                struct net2_link_tx_t* net2_link_tx = (struct net2_link_tx_t*)malloc(sizeof(struct net2_link_tx_t));
                
                // TEST : Is net2 link TX dynamic allocation OK ?
                if(net2_link_tx)
                {
                    // Net2 link TX dynamic allocation OK
                    // Creates the net2 link TX
                    result = net2_create_link_tx(net2_link_tx);       
                    
                    // TEST : Net2 link TX creation OK ?
                    if(result != -1)
                    {             
                        // Net2 link TX creation OK
                        // Connects to the socket at the given address + port (e.g "server_address" and "server_port")
                        result = net2_connect_to_socket(net2_link_tx->_net2_socket, server_address, server_port);
                        
                        // TEST : Connection OK ?
                        if(!result)
                        {
                            // Connection OK
                            // Sends the reception socket port to the server
                            const unsigned int DATA_LENGTH= sizeof(unsigned short);
                            char data[DATA_LENGTH];
                            memcpy(data, &client_port, DATA_LENGTH);
                            
                            // TEST : Is port transmission OK ?
                            if(net2_write_to_link_tx(net2_link_tx, data, DATA_LENGTH) >= 0)
					        {
					            // Port transmission OK
					            // Dynamically allocates the net2 socket for the partner
					            struct net2_socket_t* partner_socket = (struct net2_socket_t*)malloc(sizeof(struct net2_socket_t));
					            
					            // TEST : Did "partner_socket" dynamic allocation work ?
					            if(partner_socket)
					            {
					                // "partner_socket" dynamic allocation OK.
					                // Accepts the partner connection on the reception socket.
					                result = net2_accept_from_socket(net2_link_rx->_net2_socket, partner_socket);
					                
					                // TEST : Did net2 link RX acceptation work ?
					                if(result >= 0)
					                {
					                    // Net2 link RX acceptation OK
					                    net2_link_rx->_net2_socket = partner_socket;
					                    net2_link->_net2_link_rx = net2_link_rx;
                                        net2_link->_net2_link_tx = net2_link_tx;
                                        net2_link->read = &net2_read_from_link;
                                        net2_link->write = &net2_write_to_link;  
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_create_client_link");
                                        #endif 
                                    }
                                    else
                                    {
                                        // Net2 link RX acceptation failed
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_create_client_link", "Link RX acceptation failed");
                                        #endif
                                    }
					            }
					            else
					            {
					                // "partner_socket" dynamic allocation failed
					                result = -1;
					                #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_create_client_link", "Partner socket dynamic allocation failed");
                                    #endif
					            }
				            }
					        else
					        {
					            // Port transmission failed
					            result = -1;
					            #ifdef NET2_DEBUG
                                    net2_debug_failure("net2_create_client_link", "Port transmission failed");
                                #endif
					        }
                        }
                        else
                        {
                            // Connection failed
                            #ifdef NET2_DEBUG
                                net2_debug_failure("net2_create_client_link", "Connection failed");
                            #endif
                        }
                    }
                    else
                    {
                        // Net2 link TX creation failed
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_create_client_link", "Link TX creation failed");
                        #endif
                    }
                }
                else
                {
                    // Net2 link TX dynamic allocation failed
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_create_client_link", "Link TX dynamic allocation failed");
                    #endif
                }
            }
            else
            {
                // Net2 link RX listening failed
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_create_client_link", "Link RX listening failed");
                #endif
            }
        }
        else
        {
            // Net2 link RX creation failed
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_create_client_link", "Link RX creation failed");
            #endif
        }
    }
    else
    {
        // Link RX dynamic allocation failed
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_create_client_link", "Link RX dynamic allocation failed");
        #endif
    }
    
    return result;
}

int net2_create_server_link(struct net2_link_t* net2_link, unsigned short server_port)
{
    int result = 0;
    // Dynamically allocates the net2 link RX
    struct net2_link_rx_t* net2_link_rx = (struct net2_link_rx_t*)malloc(sizeof(struct net2_link_rx_t));
    
    // TEST : Is "net_link_rx" dynamic allocation OK ?
    if(net2_link_rx)
    {  
        // Dynamic allocation OK
        // Creates the net2 link RX with the given port (e.g "client_port")
        result = net2_create_link_rx(net2_link_rx, server_port);
        
        // TEST : Net2 link RX creation OK ?
        if(!result)
        {
            // Net2 link RX creation OK
            // Starts listening on the net2 link RX
            result = net2_listen_on_socket(net2_link_rx->_net2_socket);
            
            // TEST : Did net2 link RX listening succeed ?
            if(!result)
            {
                // Net2 link RX listening succeeded
                // Dynamically allocates the partner socket
                struct net2_socket_t* partner_socket = (struct net2_socket_t*)malloc(sizeof(struct net2_socket_t));
                
                // TEST : Is partner socket dynamic allocation OK ?
                if(partner_socket)
                {
                    // Partner socket dynamic allocation OK
                    // Accepts the partner connection on the reception socket.
                    result = net2_accept_from_socket(net2_link_rx->_net2_socket, partner_socket);
                    
                    // TEST : Did net2 link RX acceptation work ?
                    if(result >= 0)
                    {
                        // Net2 link RX acceptation OK
                        // Read the partner port from link RX
                        const unsigned int DATA_LENGTH = sizeof(unsigned short);
                        char data[DATA_LENGTH];
                        
                        // TEST : Has the port been received ?
                        if(net2_read_from_socket(partner_socket, data, DATA_LENGTH) >= 0)
                        {
                            // The port has been received.
                            unsigned short partner_port;
					        memcpy(&partner_port, data, DATA_LENGTH);
					        
                            // Dynamically allocates the net2 link TX
                            struct net2_link_tx_t* net2_link_tx = (struct net2_link_tx_t*)malloc(sizeof(struct net2_link_tx_t));
                            
                            // TEST : Is net2 link TX dynamic allocation OK ?
                            if(net2_link_tx)
                            {
                                // Net2 link TX dynamic allocation OK
                                // Creates the net2 link TX
                                result = net2_create_link_tx(net2_link_tx);       
                                
                                // TEST : Net2 link TX creation OK ?
                                if(result != -1)
                                {                
                                    // Net2 link TX creation OK
                                    // Connects to the socket at the given address + port
                                    result = net2_connect_to_socket(net2_link_tx->_net2_socket, net2_get_ip_of_socket(partner_socket), partner_port);
                                    
                                    // TEST : Connection OK ?
                                    if(!result)
                                    {
                                        // Connection OK
		                                // Net2 link RX acceptation OK
		                                net2_link_rx->_net2_socket = partner_socket;
		                                net2_link->_net2_link_rx = net2_link_rx;
                                        net2_link->_net2_link_tx = net2_link_tx;
                                        net2_link->read = &net2_read_from_link;
                                        net2_link->write = &net2_write_to_link;   
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_create_server_link");
                                        #endif
			                        }
                                    else
                                    {
                                        // Connection failed
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_create_server_link", "Connection failed");
                                        #endif
                                    }
                                }
                                else
                                {
                                    // Net2 link TX creation failed
                                    #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_create_server_link", "Link TX creation failed");
                                    #endif
                                }
                            }
                            else
                            {
                                // Net2 link TX dynamic allocation failed
                                #ifdef NET2_DEBUG
                                    net2_debug_failure("net2_create_server_link", "Link TX dynamic allocation failed");
                                #endif
                            }
                        }
                        else
                        {
                            // Port reception failed
                            #ifdef NET2_DEBUG
                                net2_debug_failure("net2_create_server_link", "Port reception failed");
                            #endif
                        }
                    }
                    else
                    {
                        // Net2 link RX acceptation failed
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_create_server_link", "Link RX acceptation failed");
                        #endif
                    }
                }
                else
                {
                    // Partner socket dynamic allocation failed
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_create_server_link", "Partner socket dynamic allocation failed");
                    #endif
                }
            }
            else
            {
                // Net2 link RX listening failed
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_create_server_link", "Link RX listening failed");
                #endif
            }
        }
        else
        {
            // Net2 link RX creation failed
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_create_server_link", "Link RX creation failed");
            #endif
        }
    }
    else
    {
        // "net_link_rx" dynamic allocation failed
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_create_server_link", "Link RX dynamic allocation failed");
        #endif
    }
    
    return result;
}
