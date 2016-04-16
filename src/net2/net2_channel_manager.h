/**
 * @file net2_channel_manager.h
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#ifndef NET2_CHANNEL_MANAGER_INCLUDED
#define NET2_CHANNEL_MANAGER_INCLUDED

#include <stdbool.h>

#include "net2_channel.h"

#ifdef NET2_DEBUG   
    #include "net2_debug.h"
#endif

struct net2_channel_output_linked_element_t
{
    struct net2_channel_output_t* _my_channel; ///< Contained channel output.
    struct net2_channel_output_linked_element_t* _next_channel; ///< Next channel output.
};

struct net2_channel_input_linked_element_t
{
    struct net2_channel_input_t* _my_channel; ///< Contained channel input.
    struct net2_channel_input_linked_element_t* _next_channel; ///< Next channel input.
};

/**
 * @brief Handles the channels; it registers them and is then able to return a channel corresponding to a channel number.
 **/
struct net2_channel_manager_t
{
    struct net2_channel_output_linked_element_t* _channel_outputs; ///< The linked list of channel outputs
    struct net2_channel_input_linked_element_t* _channel_inputs; ///< The linked list of channel inputs
};

/**
 * @brief Returns the singleton of the channel manager.
 * @return The singleton instance of the channel manager.
 **/
struct net2_channel_manager_t** net2_channel_manager_get_instance();

/**
 * @brief Creates the singleton instance of the channel manager.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_channel_manager_create();

/**
 * @brief Inits the channel manager : if its singleton is not already instanced, it instances it.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *             <li>ALREADY INSTANCED : -2
 *         </ul>
 **/
int net2_channel_manager_init();

/**
 * @brief Checks if the given number is available or not.
 * @param channel_number The channel number to check.
 * @param found A pointer on a boolean to store the research result.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_channel_manager_check_number(unsigned int channel_number, bool* found);

/**
 * @brief Registers the channel at the given channel number.
 * @param net2_channel_output The net2 channel output to register.
 * @param channel_number The channel number to use.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_channel_manager_register_channel_output(struct net2_channel_output_t* net2_channel_output);

/**
 * @brief Registers the channel at the given channel number.
 * @param net2_channel_input The net2 channel input to register.
 * @param channel_number The channel number to use.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_channel_manager_register_channel_input(struct net2_channel_input_t* net2_channel_input, unsigned int channel_number);

/**
 * @brief Searches the channel corresponding to the given channel number.
 *
 * If the channel is NOT found, the net2_channel_generic pointer is set to NULL, otherwise it is set to the address of the found channel.
 * @param net2_channel_generic A pointer on a pointer on a not allocated area to store the address of the found channel. MUST BE "void**" because it cannot be determined if the found channel will be of type input or output.
 * @param net2_channel_type A pointer on a channel type to store the found channel type (CHANNEL_INPUT or CHANNEL_OUTPUT).
 * @param channel_number The channel number to search.
 * @return <ul>
 *             <li>SUCCESS : 0
 *             <li>FAILURE : -1
 *         </ul>
 **/
int net2_channel_manager_get_channel(void** net2_channel_generic, enum net2_channel_type_e* net2_channel_type, unsigned int channel_number);

#endif // NET2_CHANNEL_MANAGER_INCLUDED INCLUDED
