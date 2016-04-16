#include <stdio.h> // printf
#include <stdlib.h> // malloc

#include "net2_channel_manager.h"

struct net2_channel_manager_t** net2_channel_manager_get_instance()
{
    static struct net2_channel_manager_t* net2_channel_manager_singleton = NULL;
    
    return &net2_channel_manager_singleton;
}

int net2_channel_manager_create()
{
    // Function result
    int result = 0;
    
    struct net2_channel_manager_t** net2_channel_manager = net2_channel_manager_get_instance();
    *net2_channel_manager = (struct net2_channel_manager_t*)malloc(sizeof(struct net2_channel_manager_t));
    
    // TEST : Did the channel manager dynamic allocation succeed ?
    if(*net2_channel_manager)
    {
        // Yes, the channel manager dynamic allocation succeeded.
        (*net2_channel_manager)->_channel_inputs = NULL;
        (*net2_channel_manager)->_channel_outputs = NULL;
        #ifdef NET2_DEBUG
            net2_debug_success();
        #endif
    }
    else
    {
        // No, the channel manager dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel manager dynamic allocation failed.");
        #endif
    }
    
    return result;
}

int net2_channel_manager_init()
{
    int result = 0;
    
    // TEST : Is the channel manager not instanced yet ?
    if(!*(net2_channel_manager_get_instance()))
    {
        // Yes, the channel manager can be instanced.
        result = net2_channel_manager_create(); 
        
        // TEST : Did the channel manager creation succeed ?
        if(!result)
        {
            // Yes, the channel manager creation succeeded.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the channel manager creation failed.
            #ifdef NET2_DEBUG
                net2_debug_failure("Link manager creation failed.");
            #endif
        }
    }
    else
    {
        // No, the channel manager is already instanced.
        result = -2;
    }
    
    return result;
}

int net2_channel_manager_check_number(unsigned int channel_number, bool* found)
{
    int result = 0;
    
    struct net2_channel_manager_t** channel_manager = net2_channel_manager_get_instance();
    
    // TEST : Is the channel manager already instanced ?
    if(*channel_manager)
    {
        // Yes, the channel manager is already instanced.
        struct net2_channel_output_linked_element_t* temp_outputs = (*channel_manager)->_channel_outputs;
        
        while(temp_outputs && temp_outputs->_my_channel->_number != channel_number)
        {
            temp_outputs = temp_outputs->_next_channel;
        }
        
        // TEST : Did we find the given channel number in the channel outputs ?
        if(temp_outputs)
        {
            // Yes, we found the given channel number in the channel outputs.
            *found = true;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {   
            // No, we did not find the given channel number in the channel inputs.
            struct net2_channel_input_linked_element_t* temp_inputs = (*channel_manager)->_channel_inputs;
        
            while(temp_inputs && temp_inputs->_my_channel->_number != channel_number)
            {
                temp_inputs = temp_inputs->_next_channel;
            }
            
            // TEST : Did we find the given channel number in the channel inputs ?
            if(temp_inputs)
            {
                // Yes, we found the given channel number in the channel inputs.
                *found = true;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {   
                // No, we did not find the given channel number in the channel inputs.
                *found = false;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
        }
    }
    else
    {
        // No, the channel manager is not instanced yet.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel manager is not instanced yet.");
        #endif  
    }
    
    return result;
}

int net2_channel_manager_register_channel_output(struct net2_channel_output_t* net2_channel_output)
{
    // Function result
    int result = 0;
    
    struct net2_channel_manager_t** temp = net2_channel_manager_get_instance();
    
    // TEST : Is the channel manager already instanced ?
    if(*temp)
    {
        // Yes, the channel manager is already instanced.
        bool found = false;
        unsigned int channel_number = 0;
        
        // TEST : Did the research into the channel manager succeed ?
        while(!net2_channel_manager_check_number(channel_number, &found) && found) // TODO Test maximum unsigned int value not reached
        {
            channel_number++;
        }
        
        // TEST : Is the given channel number available ?
        if(!found)
        {
            // Yes, the given channel number is available.
            struct net2_channel_output_linked_element_t* new_element = (struct net2_channel_output_linked_element_t*)malloc(sizeof(struct net2_channel_output_linked_element_t));
            
            // TEST : Did the new element dynamic allocation succeed ?
            if(new_element)
            {
                // Yes, the new element dynamic allocation succeeded.                
                net2_channel_output->_number = channel_number;
                new_element->_my_channel = net2_channel_output;
                new_element->_next_channel = NULL;
                
                struct net2_channel_output_linked_element_t* current_element = (*temp)->_channel_outputs;
                
                // TEST : Is there at least one element ? 
                if(current_element)
                {
                    // Yes, there is at least one element.
                    while(current_element->_next_channel)
                    {
                        current_element = current_element->_next_channel;
                    }
                    
                    current_element->_next_channel = new_element;                    
                }
                else
                {
                    // No, there is not any elements yet.
                    (*temp)->_channel_outputs = new_element;   
                }
                
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the new element dynamic allocation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The new element dynamic allocation failed.");
                #endif 
            }
        }
        else
        {
            // No, no channel number has been found.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("No channel number has been found.");
            #endif 
        }
    }
    else
    {
        // No, the channel manager is not instanced yet.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel manager is not instanced yet.");
        #endif  
    }
    
    return result;
}

int net2_channel_manager_register_channel_input(struct net2_channel_input_t* net2_channel_input, unsigned int channel_number)
{
   // Function result
    int result = 0;
    
    struct net2_channel_manager_t** temp = net2_channel_manager_get_instance();
    
    // TEST : Is the channel manager already instanced ?
    if(*temp)
    {
        // Yes, the channel manager is already instanced.
        bool found = false;
        
        // TEST : Did the research into the channel manager succeed ?
        if(!net2_channel_manager_check_number(channel_number, &found))
        {
            // Yes, the research into the channel manager succeeded.
            // TEST : Is the given channel number available ?
            if(!found)
            {
                // Yes, the given channel number is available.
                struct net2_channel_input_linked_element_t* new_element = (struct net2_channel_input_linked_element_t*)malloc(sizeof(struct net2_channel_input_linked_element_t));
                
                // TEST : Did the new element dynamic allocation succeed ?
                if(new_element)
                {
                    // Yes, the new element dynamic allocation succeeded.                    
                    net2_channel_input->_number = channel_number;
                    new_element->_my_channel = net2_channel_input;
                    new_element->_next_channel = NULL;
                    
                    struct net2_channel_input_linked_element_t* current_element = (*temp)->_channel_inputs;
                    
                    // TEST : Is there at least one element ? 
                    if(current_element)
                    {
                        // Yes, there is at least one element.
                        while(current_element->_next_channel)
                        {
                            current_element = current_element->_next_channel;
                        }
                        
                        current_element->_next_channel = new_element;                    
                    }
                    else
                    {
                        // No, there is not any elements yet.
                        (*temp)->_channel_inputs = new_element; 
                    }
                    
                    #ifdef NET2_DEBUG
                        net2_debug_success();
                    #endif
                }
                else
                {
                    // No, the new element dynamic allocation failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("The new element dynamic allocation failed.");
                    #endif 
                }
            }
            else
            {
                // No, the given channel number is not available.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("The given channel number is not available.");
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
    }
    else
    {
        // No, the channel manager is not instanced yet.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel manager is not instanced yet.");
        #endif  
    }
    
    return result;
}

int net2_channel_manager_get_channel(void** net2_channel_generic, enum net2_channel_type_e* net2_channel_type, unsigned int channel_number)
{
    // Function result
    int result = 0;
    
    struct net2_channel_manager_t** temp = net2_channel_manager_get_instance();
    
    // TEST : Is the channel manager already instanced ?
    if(*temp)
    {
        // Yes, the channel manager is already instanced.
        struct net2_channel_output_linked_element_t* temp_output = (*temp)->_channel_outputs;
        
        while(temp_output && temp_output->_my_channel->_number != channel_number)
        {
            temp_output = temp_output->_next_channel;
        }
        
        // TEST : Did we find the given channel number in the outputs ?
        if(temp_output)
        {
            // Yes, we found the given channel number in the outputs.
            *net2_channel_generic = temp_output->_my_channel;
            *net2_channel_type = CHANNEL_OUTPUT;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, we did not find the given channel number in the outputs.
            struct net2_channel_input_linked_element_t* temp_input = (*temp)->_channel_inputs;
        
            while(temp_input && temp_input->_my_channel->_number != channel_number)
            {
                temp_input = temp_input->_next_channel;
            }
            
            // TEST : Did we find the given channel number in the inputs ?
            if(temp_input)
            {
                // Yes, we found the given channel number in the inputs.
                *net2_channel_generic = temp_input->_my_channel;
                *net2_channel_type = CHANNEL_INPUT;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, we did not find the given channel in the inputs neither.
                *net2_channel_generic = NULL;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
        }
    }
    else
    {
        // No, the channel manager is not instanced yet.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The channel manager is not instanced yet.");
        #endif  
    }
    
    return result;
}
