#include <string.h> // strlen | memcpy
#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <pthread.h> // pthread_mutex_lock

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

#include "net2_link.h"
#include "net2_protocol.h"
#include "net2_channel.h"
#include "net2_channel_manager.h"

int net2_link_rx_read(struct net2_socket_t* net2_socket, void* data, unsigned int data_length)
{
    #ifdef NET2_DEBUG
        int result = net2_socket_read(net2_socket, data, data_length);
        
        // Did the read succeed ?
        if(!result)
        {
            // Yes, the read succeeded.
            net2_debug_success("net2_link_rx_read");
        }
        else if(result == -1)
        {
            // The peer has performed an orderly shutdown.
            net2_debug_failure("net2_link_rx_read", "The peer has performed an orderly shutdown.");
        }
        else if(result == -2)
        {
            // No, the read failed.
            net2_debug_failure("net2_link_rx_read", "The read failed.");
        }
        else
        {
            // TODO Handles this case
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
        if(!result)
        {
            // Yes, the write succeeded.
            net2_debug_success("net2_link_write");
        }
        else if(result == -1)
        {
            // No, nothing has been written
            net2_debug_failure("net2_link_write", "Nothing has been written.");
        }
        else if(result == -2)
        {
            // No, the write failed.
            net2_debug_failure("net2_link_write", "The write failed.");
        }
        else
        {
            // Unknown returned value
            net2_debug_failure("net2_link_write", "Unknown returned value.");
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
        if(!result)
        {
            // Yes, the read succeeded.
            net2_debug_success("net2_link_read");
        }
        else if(result == -1)
        {
            // The peer has performed an orderly shutdown.
            net2_debug_failure("net2_link_read", "The peer has performed an orderly shutdown.");
        }
        else if(result == -2)
        {
            // No, the read failed.
            net2_debug_failure("net2_link_read", "The read failed.");
        }
        else
        {
            // TODO Handles this case
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
        if(!result)
        {
            // Yes, the write succeeded.
            net2_debug_success("net2_link_tx_write");
        }
        else if(result == -1)
        {
            // No, nothing has been written.
            net2_debug_failure("net2_link_tx_write", "Nothing has been written.");
        }
        else if(result == -2)
        {
            // No, the write failed.
            net2_debug_failure("net2_link_tx_write", "The write failed.");
        }
        else
        {
            // Unknown returned value
            net2_debug_failure("net2_link_tx_write", "Unknown returned value.");
        }
        
        return result;
    #else        
        return net2_link_tx_write(net2_link->_net2_socket, data, data_length);
    #endif
}

int net2_link_send(struct net2_link_t* net2_link, struct net2_message_t* net2_message)
{  
    // Function result
    int result = 0;
    void* data = NULL;
    unsigned int data_length = 0;
    
    switch(net2_message->_type)
    {
        case SEND :
            data_length = sizeof(unsigned int) // the type
                        + sizeof(unsigned int) // source
                        + sizeof(unsigned int) // destination
                        + net2_message->_data_length // length of data
                        + sizeof(unsigned int); // the variable containing the length of the data
            break;
        case ACK :
            data_length = sizeof(unsigned int) // the type
                        + sizeof(unsigned int); // destination
            break;
        case OPEN : 
            data_length = sizeof(unsigned int) // the type
                        + sizeof(unsigned int) // destination
                        + sizeof(unsigned int); // sourcel
            break;
        default :
            result = -1;
            break;
    }
    
    // Was the type correct ?
    if(!result)
    {
        // Yes, the type was correct.
        data = (void*)malloc(data_length);
        
        // TEST : Did the dynamic allocation succeed ?
        if(data)
        {
            void* temp = data;
            
            switch(net2_message->_type)
            {
                case SEND :
                    //spy("SEND SENT");
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // the type
                    memcpy(temp, &(net2_message->_destination), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // destination
                    memcpy(temp, &(net2_message->_source), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // source
                    memcpy(temp, &(net2_message->_data_length), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // the variable containing the length of the data
                    memcpy(temp, net2_message->_data, net2_message->_data_length);
                    break;
                case ACK :
                    //spy("ACK SENT");
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // the type
                    memcpy(temp, &(net2_message->_destination), sizeof(unsigned int));
                    break;
                case OPEN :
                    //spy("OPEN SENT");
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // the type
                    memcpy(temp, &(net2_message->_destination), sizeof(unsigned int));
                    temp += sizeof(unsigned int); // the type
                    memcpy(temp, &(net2_message->_source), sizeof(unsigned int));
                    break;
                default :
                    // TODO Handle those types as well
                    break;
            }
            
            result = net2_link_write(net2_link, data, data_length);
            // TEST : Did the writing succeed ?
            if(!result)
            {
                result = 0;
                //spy("HAS BEEN SENT");
                // Yes, the writing succeeded.
                #ifdef NET2_DEBUG
                    net2_debug_success("net2_link_send");
                #endif
            }
            else if(result == -1)
            {
                // No, nothing has been written.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_send", "Nothing has been written.");
                #endif
            }
            else if(result == -2)
            {
                // No, the writing failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_send", "The writing failed.");
                #endif
            }
            else
            {
                // Unknown returned value.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_send", "Unknown returned value.");
                #endif
            }
            
            free(data);
        }
        else
        {
            // No, the data dynamic allocation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_link_send", "The data dynamic allocation failed.");
            #endif
        }
    }
    else
    {
        // No, the type was not correct.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_send", "The type was not correct.");
        #endif
    }
    
    return result;
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

void* net2_link_run(void* net2_link_to_run)
{
    struct net2_link_t* net2_link = (struct net2_link_t*)net2_link_to_run;
    
    int result = 0;
    bool condition = true;
    
    struct net2_message_t* net2_message;
    
    // TEST : Did the message dynamic allocation succeed ?
    while(condition && (net2_message = (struct net2_message_t*)malloc(sizeof(struct net2_message_t))))
    {
        // Yes, the message dynamic allocation succeeded.
        result = net2_link_read(net2_link, &(net2_message->_type), sizeof(net2_message->_type));
        
        // TEST : Did the message type reading succeed ?
        if(!result)
        {
            // Yes, the message type reading succeeded.
            result = net2_link_read(net2_link, &(net2_message->_destination), sizeof(net2_message->_destination));
            
            // TEST : Did the message destination reading succeed ?
            if(!result)
            {
                // Yes, the message destination reading succeeded.
                // TEST : Is the message of known type ?
                switch(net2_message->_type)
                {
                    case SEND :
                        // Yes, the message if of known type.
                        result = net2_link_read(net2_link, &(net2_message->_source), sizeof(net2_message->_source));
                        
                        // TEST : Did the message source reading succeed ?
                        if(!result)
                        {
                            // Yes, the message source reading succeeded.
                            result = net2_link_read(net2_link, &(net2_message->_data_length), sizeof(net2_message->_data_length));
                            
                            // TEST : Did the message data length reading succeed ?
                            if(!result)
                            {
                                // Yes, the message data length reading succeeded.
                                net2_message->_data = (void*)malloc(net2_message->_data_length);
                                
                                // TEST : Did the data dynamic allocation succeed ?
                                if(net2_message->_data)
                                {
                                    // Yes, the data dynamic allocation succeeded.
                                    result = net2_link_read(net2_link, net2_message->_data, net2_message->_data_length);
                                    
                                    // TEST : Did the message data reading succeed ?
                                    if(!result)
                                    {
                                        // Yes, the message data reading succeeded.
                                    }
                                    else
                                    {
                                        // No, the message data reading failed.
                                        condition = (result == -2);
                                        result = -7;
                                    }
                                }
                                else
                                {
                                    // No, the data dynamic allocation failed.
                                    result = -6;
                                }
                            }
                            else
                            {
                                // No, the message data length reading failed.
                                condition = (result == -2);
                                result = -5;
                            }
                        }
                        else
                        {
                            // No, the message source reading failed.
                            condition = (result == -2);
                            result = -4;
                        }
                        break;
                    case ACK :
                        // Yes, the message if of known type.
                        break;
                    case OPEN :
                        // Yes, the message if of known type.
                        result = net2_link_read(net2_link, &(net2_message->_source), sizeof(net2_message->_source));
                        
                        // TEST : Did the message source reading succeed ?
                        if(!result)
                        {
                            // Yes, the message source reading succeeded.
                        }
                        else
                        {  
                            // No, the message source reading failed.
                            condition = (result == -2);
                            result = -8;
                        }   
                        break;
                    default :
                        // No, the message is of unknown type.
                        result = -3;
                        break;
                }
                
                if(!result)
                {
                    // Yes, it was a correct message type.
                    void* channel = NULL;
                    enum net2_channel_type_e channel_type;
                    result = net2_channel_manager_get_channel(&channel, &channel_type, net2_message->_destination);
                    
                    // TEST : Did the research into the channel manager succeed ?
                    if(!result)
                    {
                        // Yes, the research into the channel manager succeeded.
                        // TEST : Has a channel been found ?
                        if(channel)
                        {
                            // Yes, a channel has been found for this channel number.
                            if(channel_type == CHANNEL_INPUT)
                            {
                                struct net2_channel_input_t** channel_input = (struct net2_channel_input_t**)&channel;
                                
                                if(net2_message->_type == OPEN)
                                {
                                    (*channel_input)->_link = net2_link;
                                    pthread_mutex_lock(&((*channel_input)->_mutex));
                                    pthread_cond_signal(&((*channel_input)->_cond));
                                    pthread_mutex_unlock(&((*channel_input)->_mutex));
                                    #ifdef NET2_DEBUG
                                        net2_debug_success("net2_link_run OPEN");
                                    #endif
                                }
                                else if(net2_message->_type == SEND)
                                {
                                    result = net2_channel_input_add_message_to_buffer(*channel_input, net2_message);
                                
                                    // TEST : Has the message add succeed ?
                                    if(!result)
                                    {
                                        // Yes, the message add succeeded.
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_link_run SEND");
                                        #endif
                                    }
                                    else
                                    {
                                        result = -9;
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_link_run", "The message add failed.");
                                        #endif
                                    }
                                }
                                else
                                {
                                    result = -10;
                                    #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_link_run", "The message has an unknown type.");
                                    #endif
                                }
                            }
                            else
                            {
                                struct net2_channel_output_t** channel_output = (struct net2_channel_output_t**)&channel;
                                
                                if(net2_message->_type == ACK)
                                {
                                    result = net2_channel_output_add_message_to_buffer(*channel_output, net2_message);
                                    if(!result)
                                    {
                                        // Yes, the message add succeeded.
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_link_run ACK");
                                        #endif
                                    }
                                    else
                                    {
                                        result = -11;
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_link_run", "The message add failed.");
                                        #endif
                                    }
                                }
                                else
                                {
                                    result = -12;
                                    #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_link_run", "The message has an unknown type.");
                                    #endif
                                }
                            }
                        }
                        else
                        {
                            // No, any channel with this channel number has been found
                            result = -13;
                            #ifdef NET2_DEBUG
                                char buffer[512];
                                sprintf(buffer, "Any channel with this channel number %d has been found.", net2_message->_destination);
                                net2_debug_failure("net2_link_run", buffer);
                            #endif
                        }
                    }
                    else
                    {
                        // No, the reserach into the channel manager failed.
                        result = -14;
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_link_run", "The channel has not been found.");
                        #endif
                    }
                }
                else
                {
                    // No, it was not a correct message type, or at least it was not known.
                    result = -15;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_link_run", "It was not a correct message type, or at least it was not known.");
                    #endif
                }
            }
            else
            {
                // No, the message destination reading failed.
                condition = (result == -2);
                result = -2;
            }
        }
        else
        {
            // No, the message type reading failed.
            condition = (result == -2);
            result = -1;
        }
         
        if(result)
        {
            if(net2_message->_data)
            {
                free(net2_message->_data);
            }
            
            free(net2_message);
        }
    }
    
    return NULL; // FIXME Replace returned value
}
