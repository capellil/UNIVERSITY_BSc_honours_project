/**
 * @file net2_protocol.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 24/01/2015
 **/

#ifndef NET2_PROTOCOL_INCLUDED 
#define NET2_PROTOCOL_INCLUDED

extern const unsigned int NET2_MESSAGE_MAX_LENGTH;

enum net2_protocol_e { NOTHING = 0, 
                       SEND = 1,
                       ACK = 2,
                       REJECT_CHANNEL = 8,
                       OPEN = 14,
                       REQUEST_ACK = 22};

/**
 * @class net2_message_t
 * @brief Contains a message in a defined format.
 **/
struct net2_message_t
{
    enum net2_protocol_e _type; ///< SEND / ACK ...
    unsigned int _source; ///< Source channel number
    unsigned int _destination; ///< Destination channel number
    unsigned int _data_length; ///< Data length
    void* _data; ///< Data to be sent
};

/**
 * @class net2_message_linked_element_t
 * @brief A linked element composing a linekd list.
 **/
struct net2_message_linked_element_t
{
    struct net2_message_t* _my_message; ///< Contained message.
    struct net2_message_linked_element_t* _next_message; ///< Next message.
};

/**
 * @brief Displays the description of the given net2 message.
 * @param net2_message The message to display.
 **/
void net2_message_print(struct net2_message_t* net2_message);

#endif // NET2_NETWORK_PROTOCOL_INCLUDED INCLUDED
