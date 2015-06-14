/**
 * @file net2_channel.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#ifndef NET2_CHANNEL_INCLUDED
#define NET2_CHANNEL_INCLUDED

#include <pthread.h>

#include "net2_link.h"
#include "net2_protocol.h"

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

enum net2_channel_state_e {INACTIVE, OK_INPUT, OK_OUTPUT};
enum net2_channel_type_e {CHANNEL_INPUT, CHANNEL_OUTPUT};

/**
 * @brief This structure is responsible for sending messages to a remote net2 channel input.
 **/
struct net2_channel_output_t
{
    struct net2_link_t* _link; ///< The link the channel communicates through.
    unsigned int _number; ///< Channel virtual number
    unsigned int _remote_number; ///< Remote channel virtual number
    enum net2_channel_state_e _state; ///< Current channel state
    struct net2_message_linked_element_t* _messages; ///< Messages
    unsigned short _remote_port; ///< Remote port
    unsigned int _remote_address; ///< Remote address
    pthread_mutex_t _mutex; ///< Used to ensure data integrity
    pthread_cond_t _cond; ///< Used if read is before the buffer get the data.
};

/**
 * @brief This structure is responsible for reading messages from a remote net2 channel output.
 **/
struct net2_channel_input_t
{
    struct net2_link_linked_element_t* _links; ///< Links the channel communicates through.
    unsigned int _number; ///< Channel virtual number
    enum net2_channel_state_e _state; ///< Current channel state
    struct net2_message_linked_element_t* _messages; ///< Messages
    pthread_mutex_t _mutex; ///< Used to ensure data integrity
    pthread_cond_t _cond; ///< Used if read is before the buffer get the data.
};

/**
 * @brief Creates the channel by asking the node to connect with the given node at (ip_address, port) to the channel with the given number.
 * @param net2_channel A pointer to a channel to store in the created one.
 * @param ip_address A string format IPv4 address (4 bytes values separated by ".") of the remote node.
 * @param port The port of the remote node.
 * @param remote_channel_number The channel number of the remote channel.
 * @param remote_channel_number
 * @return <ul>
               <li>SUCCESS : 0
               <li>FAILURE : -1
           </ul>
 * @pre net2_channel != NULL
 **/
int net2_channel_output_create(struct net2_channel_output_t* net2_channel_output, char* ip_address, unsigned short port, unsigned int remote_channel_number);
int net2_channel_output_write_integer(struct net2_channel_output_t* net2_channel_output, int value);
int net2_channel_output_add_message_to_buffer(struct net2_channel_output_t* net2_channel_output, struct net2_message_t* net2_message);
int net2_channel_output_connect(struct net2_channel_output_t* net2_channel_output);

int net2_channel_input_create(struct net2_channel_input_t* net2_channel_input, unsigned int remote_channel_number);
int net2_channel_input_read_integer(struct net2_channel_input_t* net2_channel_input, int* value);
int net2_channel_input_add_message_to_buffer(struct net2_channel_input_t* net2_channel_input, struct net2_message_t* net2_message);

#endif
