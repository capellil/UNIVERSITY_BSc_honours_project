#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include <sys/socket.h> // sock_addr
#include <string.h> // memcpy
#include <pthread.h> // pthread_mutex_lock

#include "net2_node.h"
#include "net2_channel.h"
#include "net2_channel_manager.h"
#include "net2_protocol.h"
#include "net2_socket.h"

int net2_channel_output_create(struct net2_channel_output_t* net2_channel_output, char* ip_address_char, unsigned short port, unsigned int remote_channel_number)
{   
    int result = 0;
    
    struct in_addr ip_address;
    
    // TEST : Is the given address a valid IPv4 address ?    
    if(inet_pton(AF_INET, ip_address_char, &ip_address) > 0)
    {        
        // Yes, the given address is a valid IPv4 address.        
        struct net2_link_t* net2_link = NULL;
        
        // Yes, the connection with the remote node succeeded.
        net2_channel_output->_link = net2_link;
        net2_channel_output->_state = INACTIVE;
        net2_channel_output->_remote_number = remote_channel_number;
        net2_channel_output->_messages = NULL;
        net2_channel_output->_remote_port = port;
        net2_channel_output->_remote_address = ntohl(ip_address.s_addr);
        pthread_cond_init(&(net2_channel_output->_cond), NULL);
        pthread_mutex_init(&(net2_channel_output->_mutex), NULL);
        
        if(!net2_channel_manager_register_channel_output(net2_channel_output))
        {
            // Yes, the channel output registration succeeded.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the channel output registration failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The channel output registration failed.");
            #endif
        }
    }
    else
    {
        // No, the given address is not a valid IPv4 address.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The given address is not a valid IPv4 address.");
        #endif
    }
    
    return result;
}

int net2_channel_output_write_integer(struct net2_channel_output_t* net2_channel_output, int value)
{
    int result = 0;
    
    // TEST : Is the channel in a functional state, or has it been correctly connected if it was INACTIVE ?
    if(net2_channel_output->_state == OK_OUTPUT || (net2_channel_output->_state == INACTIVE && !net2_channel_output_connect(net2_channel_output)))
    {
        // Yes, the channel is now correctly connected.
        struct net2_message_t integer_message;
        integer_message._type = SEND;
        integer_message._source = net2_channel_output->_number;
        integer_message._destination = net2_channel_output->_remote_number;
        integer_message._data = &value;
        integer_message._data_length = sizeof(int);
        
        pthread_mutex_lock(&(net2_channel_output->_mutex));
        
        // TEST : Did the writing succeed ?
        if(!net2_link_send(net2_channel_output->_link, &integer_message))
        {
            // Yes, the writing succeeded.
            if(!(net2_channel_output->_messages))
            {
                pthread_cond_wait(&(net2_channel_output->_cond), &(net2_channel_output->_mutex));
            }
            
            struct net2_message_linked_element_t* container = net2_channel_output->_messages;
            struct net2_message_t* message_to_read = container->_my_message;
        
            if(!(net2_channel_output->_messages->_next_message))
            {
                net2_channel_output->_messages = NULL;
            }
            else
            {
                net2_channel_output->_messages = net2_channel_output->_messages->_next_message;
            }
            
            // TEST : Is the message of the expected type ?
            if(message_to_read->_type == ACK)
            {
                // Yes, the message is of expected type.   
                free(message_to_read);
                free(container); 
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the message is not of expected type.
                if(message_to_read->_data)
                {
                    free(message_to_read->_data);
                }
                
                free(message_to_read);
                free(container);
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The message is not of expected type.");
                #endif
            }
        }
        else
        {
            // No, the writing failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The writing failed.");
            #endif
        }
        
        pthread_mutex_unlock(&(net2_channel_output->_mutex));
    }
    else
    {
        // No, the channel is still not properly connected.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel is still not properly connected.");
        #endif
    }
    
    return result;
}

int net2_channel_output_connect(struct net2_channel_output_t* net2_channel_output)
{
    int result = 0;
    
    // TEST : Did the node succeed to connect the channel ?
    if(!net2_node_connect(&(net2_channel_output->_link), net2_channel_output->_remote_address, net2_channel_output->_remote_port))
    {
        // Yes, the node succeeded to connect the channel.
        net2_channel_output->_state = OK_OUTPUT;
    }
    else
    {
        // No, the node failed to connect the channel.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The node failed to connect the channel.");
        #endif
    }
    
    return result;
}

int net2_channel_output_add_message_to_buffer(struct net2_channel_output_t* net2_channel_output, struct net2_message_t* net2_message)
{
    int result = 0;
    
    struct net2_message_linked_element_t* new_element = (struct net2_message_linked_element_t*)malloc(sizeof(struct net2_message_linked_element_t));
    
    // TEST : Did the new element dynamic allocation succeed ?
    if(new_element)
    {
        // Yes, the new element dynamic allocation succeeded.
        new_element->_my_message = net2_message;
        new_element->_next_message = NULL;
        pthread_mutex_lock(&(net2_channel_output->_mutex));
        struct net2_message_linked_element_t* temp = net2_channel_output->_messages;
        
        // TEST : Is there already at least one message ?
        if(temp)
        {
            // Yes, there is at least one message.
            while(temp->_next_message)
            {
                temp = temp->_next_message;
            }
            
            temp->_next_message = new_element;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            net2_channel_output->_messages = new_element;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        
        pthread_cond_signal(&(net2_channel_output->_cond));
        
        pthread_mutex_unlock(&(net2_channel_output->_mutex));
    }
    else
    {
        // No, the new element dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The new element dynamic allocation failed.");
        #endif
    }
    
    return result;
}

int net2_channel_input_create(struct net2_channel_input_t* net2_channel_input, unsigned int virtual_channel_number)
{
    int result = 0;
    bool found = false;
        
    // TEST : Did the research into the channel manager succeed ?
    if(!net2_channel_manager_check_number(virtual_channel_number, &found))
    {
        // Yes, the research into the channel manager succeeded.
        // TEST : Is this channel number free ?
        if(!found)
        {
            // Yes, this channel number is free.
            pthread_cond_init(&(net2_channel_input->_cond), NULL);
            pthread_mutex_init(&(net2_channel_input->_mutex), NULL);
            pthread_mutex_lock(&(net2_channel_input->_mutex));            
            
            // TEST : Did the channel registration succeed ?
            if(!net2_channel_manager_register_channel_input(net2_channel_input, virtual_channel_number))
            {
                // Yes, the channel registration succeeded.
                net2_channel_input->_messages = NULL;
                net2_channel_input->_state = INACTIVE;
                
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the channel registration failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The channel registration failed.");
                #endif
            }
            
            pthread_mutex_unlock(&(net2_channel_input->_mutex));
        }
        else
        {
            // No, the channel manager already has a channel with this number.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The channel manager already has a channel with this number.");
            #endif
        }
    }
    else
    {
        // No, the research into the channel manager failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The research into the channel manager failed.");
        #endif
    }
    
    return result;
}

int net2_channel_input_read_integer(struct net2_channel_input_t* net2_channel_input, int* value)
{
    int result = 0;
     
    // Yes, the channel has at least one established link already.
    pthread_mutex_lock(&(net2_channel_input->_mutex));
    
    // TEST : Is there any messages to read ?
    if(!(net2_channel_input->_messages))
    {
        // No, there is not any messages to read.
        // Waiting for a new message to be received.
        pthread_cond_wait(&(net2_channel_input->_cond), &(net2_channel_input->_mutex));
    }  
    
    struct net2_message_linked_element_t* container = net2_channel_input->_messages;
    struct net2_message_t* message_to_read = container->_my_message;

    if(!(net2_channel_input->_messages->_next_message))
    {
        net2_channel_input->_messages = NULL;
    }
    else
    {
        net2_channel_input->_messages = net2_channel_input->_messages->_next_message;
    }
    
    // TEST : Is the message of the expected type ?
    if(message_to_read->_type == SEND)
    {
        // Yes, the message is of expected type.
        memcpy(value, (int*)(message_to_read->_data), sizeof(int));
        
        struct net2_message_t ack_message;
        ack_message._type = ACK;
        ack_message._destination = message_to_read->_source;
        
        // TEST : Has the ACK message been correctly sent ?
        if(!net2_link_send(message_to_read->_link, &ack_message)) 
        {
            // Yes, the ACK message has been correctly sent.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the ACK message has not been correctly sent.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The ACK message has not been correctly sent.");
            #endif
        }
        
        free(message_to_read->_data);
        free(message_to_read);
        free(container); 
    }
    else
    {
        // No, the message is not of expected type.
        if(message_to_read->_data)
        {
            free(message_to_read->_data);
        }
        free(container); 
        
        result = -1;
        #ifdef NET2_DEBUG
            char buffer[64];
            sprintf(buffer, "The message is not of expected type, value of %d.", message_to_read->_type);
            net2_debug_failure(buffer);
        #endif
        free(message_to_read);
    }
    
    pthread_mutex_unlock(&(net2_channel_input->_mutex));    
    
    return result;
}

int net2_channel_input_add_message_to_buffer(struct net2_channel_input_t* net2_channel_input, struct net2_message_t* net2_message)
{
    int result = 0;
    pthread_mutex_lock(&(net2_channel_input->_mutex));
    
    struct net2_message_linked_element_t* new_element;
    new_element = (struct net2_message_linked_element_t*)malloc(sizeof(struct net2_message_linked_element_t));
    
    // TEST : Did the new element dynamic allocation succeed ?
    if(new_element)
    {
        // Yes, the new element dynamic allocation succeeded.
        new_element->_my_message = net2_message;
        new_element->_next_message = NULL;
        struct net2_message_linked_element_t* temp = net2_channel_input->_messages;
        
        // TEST : Is there already at least one message ?
        if(temp)
        {
            // Yes, there is at least one message.
            while(temp->_next_message)
            {
                temp = temp->_next_message;
            }
            
            temp->_next_message = new_element;
            
            // TEST : Did the add of the link to the channel input linked list of links succeed ?
            if(!net2_link_append_to_linked_element(net2_message->_link, &(net2_channel_input->_links)))
            {
                // Yes, the add of the link to the channel input linked list of links succeeded.
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the add of the link to the channel input linked list of links failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The add of the link to the channel input linked list of links failed.");
                #endif  
            }
        }
        else
        {
            net2_channel_input->_messages = new_element;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        
        pthread_cond_signal(&(net2_channel_input->_cond));
    }
    else
    {
        // No, the new element dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The new element dynamic allocation failed.");
        #endif
    }
    
    pthread_mutex_unlock(&(net2_channel_input->_mutex));
    
    return result;
}
