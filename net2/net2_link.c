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

int net2_link_send(struct net2_link_t* net2_link, struct net2_message_t* net2_message)
{  
    // Function result
    int result = 0;
    void* data = NULL;
    unsigned int data_length = 0;
    
    switch(net2_message->_type)
    {
        case SEND :
            data_length = sizeof(unsigned short) // the type
                        + sizeof(unsigned int) // source
                        + sizeof(unsigned int) // destination
                        + net2_message->_data_length // length of data
                        + sizeof(unsigned int); // the variable containing the length of the data
            break;
        case ACK :
            data_length = sizeof(unsigned short) // the type
                        + sizeof(unsigned int); // destination
            break;
        case OPEN : 
            data_length = sizeof(unsigned short) // the type
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
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned short));
                    temp += sizeof(unsigned short); // the type
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
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned short));
                    temp += sizeof(unsigned short); // the type
                    memcpy(temp, &(net2_message->_destination), sizeof(unsigned int));
                    break;
                case OPEN :
                    //spy("OPEN SENT");
                    memcpy(temp, &(net2_message->_type), sizeof(unsigned short));
                    temp += sizeof(unsigned short); // the type
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
                // TODO Handle this case
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
    
    int result;
    unsigned int data_length = NET2_MESSAGE_MAX_LENGTH;
    void* data = (void*)malloc(data_length);
    void* data_origin = data;
    int number_of_read_bytes;
    
    // TEST : Did the data dynamic allocation succeed ?
    if(data)
    {
        // Yes, the data dynamic allocation succeeded.
        while(!(number_of_read_bytes = net2_link_read(net2_link, data, data_length)))
        {
            result = 0;
            
            struct net2_message_t* net2_message = (struct net2_message_t*)malloc(sizeof(struct net2_message_t));
                
            // TEST : Did the message dynamic allocation succeed ?
            if(net2_message)
            {
                unsigned short type;
                memcpy(&type, (unsigned short*)data, sizeof(unsigned short));
                
                // Yes, the message dynamic allocation succeeded.
                switch(type)
                {
                    case SEND :
                        //spy("SEND RECEIVED");
                        memcpy(&(net2_message->_type), data, sizeof(unsigned short));
                        data += sizeof(unsigned short); // the type
                        memcpy(&(net2_message->_destination), data, sizeof(unsigned int));
                        data += sizeof(unsigned int); // destination
                        memcpy(&(net2_message->_source), data, sizeof(unsigned int));
                        data += sizeof(unsigned int); // source
                        memcpy(&(net2_message->_data_length), data, sizeof(unsigned int));
                        data += sizeof(unsigned int); // the variable containing the length of the data
                        net2_message->_data = (void*)malloc(net2_message->_data_length);

                        // TEST : Did the message data dynamic allocation succeed ?
                        if(net2_message->_data)
                        {
                            // Yes, the message data dynamic allocation succeeded.
                            memcpy(net2_message->_data, data, net2_message->_data_length);
                        }
                        else
                        {
                            // No, the message data dynamic allocation failed.
                            result = -1;
                            #ifdef NET2_DEBUG
                                net2_debug_failure("net2_link_run", "The message data dynamic allocation failed.");
                            #endif
                        }
                        break;
                    case ACK :
                        //spy("ACK RECEIVED");
                        memcpy(&(net2_message->_type), data, sizeof(unsigned short));
                        data += sizeof(unsigned short); // the type
                        memcpy(&(net2_message->_destination), data, sizeof(unsigned int));
                        break;
                    case OPEN :
                        //spy("OPEN RECEIVED");
                        memcpy(&(net2_message->_type), data, sizeof(unsigned short));
                        data += sizeof(unsigned short); // the type
                        memcpy(&(net2_message->_destination), data, sizeof(unsigned int));
                        data += sizeof(unsigned int); // the destination
                        memcpy(&(net2_message->_source), data, sizeof(unsigned int));
                        break;
                    default : 
                        result = -1;
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_link_run", "The message type was not correct.");
                        #endif
                        break;
                }
                
                // TEST : Was it a correct message type ?
                if(!result)
                {
                    // Yes, it was a correct message type.
                    void* channel = NULL;
                    enum net2_channel_type_e channel_type;
                    
                    // TEST : Did the research into the channel manager succeed ?
                    if(!net2_channel_manager_get_channel(&channel, &channel_type, net2_message->_destination))
                    {
                        // Yes, the research into the channel manager succeeded.
                        // TEST : Has a channel been found ?
                        if(channel)
                        {
                            // Yes, a channel has been found for this channel number.
                            if(channel_type == CHANNEL_INPUT)
                            {
                                struct net2_channel_input_t** channel_input = (struct net2_channel_input_t**)&channel;
                                
                                if(type == OPEN)
                                {
                                    (*channel_input)->_link = net2_link;
                                    pthread_mutex_lock(&((*channel_input)->_mutex));
                                    pthread_cond_signal(&((*channel_input)->_cond));
                                    pthread_mutex_unlock(&((*channel_input)->_mutex));
                                    #ifdef NET2_DEBUG
                                        net2_debug_success("net2_link_run OPEN");
                                    #endif
                                }
                                else
                                {
                                    // TEST : Has the message add succeed ?
                                    //spy("ADDS TO BUFFER INPUT");
                                    if(!net2_channel_input_add_message_to_buffer(*channel_input, net2_message))
                                    {
                                        // Yes, the message add succeeded.
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_link_run SEND");
                                        #endif
                                    }
                                    else
                                    {
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_link_run", "The message add failed.");
                                        #endif
                                    }
                                }
                            }
                            else
                            {
                                struct net2_channel_output_t** channel_output = (struct net2_channel_output_t**)&channel;
                                
                                if(type == ACK)
                                {
                                    //spy("ADDS TO BUFFER OUTPUT");
                                    if(!net2_channel_output_add_message_to_buffer(*channel_output, net2_message))
                                    {
                                        // Yes, the message add succeeded.
                                        #ifdef NET2_DEBUG
                                            net2_debug_success("net2_link_run ACK");
                                        #endif
                                    }
                                    else
                                    {
                                        #ifdef NET2_DEBUG
                                            net2_debug_failure("net2_link_run", "The message add failed.");
                                        #endif
                                    }
                                }
                                else
                                {
                                    #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_link_run", "The message has incorrect type.");
                                    #endif
                                }
                            }
                        }
                        else
                        {
                            // No, any channel with this channel number has been found
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
                        #ifdef NET2_DEBUG
                            net2_debug_failure("net2_link_run", "The channel has not been found.");
                        #endif
                    }
                }
                else
                {
                    // No, it was not a correct message type, or at least it was not known.
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_link_run", "It was not a correct message type, or at least it was not known.");
                    #endif
                }
            }
            else
            {
                // No, the message dynamic allocation failed.
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_run", "The message dynamic allocation failed.");
                #endif
            }
            
            data = data_origin;
        }
        
        if(number_of_read_bytes == -1)
        {
            //spy("The peer has orderely shutdown.\n");
            // TODO Inform channels it is broken.
        }
        else if(number_of_read_bytes == -2)
        {
            //spy("There has been a problem reading.\n");
            // TODO Inform channels it is broken.
        }
        else
        {   
            // TODO Handles this case
        }
    }
    else
    {
        // No, the data dynamic allocation failed.
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_run", "The data dynamic allocation failed.");
        #endif
    }
    
    return NULL;
}
