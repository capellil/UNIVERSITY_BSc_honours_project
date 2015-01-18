#include "net2_link_manager.h"

int net2_link_manager_get_instance(struct net2_link_manager_t** net2_link_manager)
{
    // The pointer on the uniq instance of link_manager.
    static struct net2_link_manager_t* link_manager = NULL;
    
    // The function result
    int result = 0;
    
    // TEST : Is the link manager NULL
    if(!link_manager)
    {
        // Yes the link manager is NULL (e.g : not instanced yet)
        // We create its instance
        link_manager = (struct net2_link_manager_t*)malloc(sizeof(struct net2_link_manager_t));
        
        // TEST : Is link manager dynamic allocation OK ?
        if(link_manager)
        {
            // Yes, the link manager dynamic allocation is OK
            link_manager->_net2_links = NULL;
            *net2_link_manager = link_manager;
            #ifdef NET2_DEBUG
                net2_debug_success("net2_link_manager_get_instance");
            #endif
        }
        else
        {
            // No, the link manager dynamic allocation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_link_manager_get_instance", "Link manager dynamic allocation");
            #endif
        }
    }
    else
    {
        // No, the link manager is already instanced. Nothing to do.
        *net2_link_manager = link_manager;
    }
    
    return result;
}

int net2_link_manager_register_link(struct net2_link_t* net2_link)
{
    // The function result
    int result = 0;
    
    struct net2_link_manager_t* net2_link_manager = NULL;
    
    // TEST : Did we get the link manager instance without any problem ?
    if(!net2_link_manager_get_instance(&net2_link_manager))
    {        
        // Yes, we got the link manager instance without any problem.
        struct net2_link_linked_element_t* temp = net2_link_manager->_net2_links;
        
        // TEST : Has the link manager some links registered already ?
        if(temp)
        {
            // Yes, the link manager already has at least 1 net2 link registered.        
            // Parses the link manager linked list of links
            while(!net2_link_compare_to_link(net2_link, temp->_my_link) && temp->_next_link)
            {
                // We go to the next net2 link.
                temp = temp->_next_link;   
            }
            
            // We test if we stopped because we are at the last node and / or if we found another link pointing to the same address + port.
            // TEST : Has an identical link been found ?
            if(net2_link_compare_to_link(net2_link, temp->_my_link))
            {
                // Yes, an identical link has been found.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_manager_register_link", "The link to register points to the same address + port than a link already registered.");
                #endif
            }
            else
            {
                // No, any identical link has been found.
                // Dynamically allocate the new link element
                struct net2_link_linked_element_t* new_linked_element = (struct net2_link_linked_element_t*)malloc(sizeof(struct net2_link_linked_element_t));
                
                // TEST : Did the new link element dynamic allocation succeed ?
                if(new_linked_element)
                {
                    // Yes, the new link element dynamic allocation succeeded.
                    new_linked_element->_my_link = net2_link;
                    temp->_next_link = new_linked_element;
                }
                else
                {
                    // No, the new link element dynamic allocation failed.
                    result = -1;
                    #ifdef NET2_DEBUG
                        net2_debug_failure("net2_link_manager_register_link", "The new link element dynamic allocation failed.");
                    #endif
                }
            }
        }
        else
        {
            // No, the link manager does not have any net2 link registered yet.
            // Dynamically allocate the new link element
            struct net2_link_linked_element_t* new_linked_element = (struct net2_link_linked_element_t*)malloc(sizeof(struct net2_link_linked_element_t));
            
            // TEST : Did the new link element dynamic allocation succeed ?
            if(new_linked_element)
            {
                // Yes, the new link element dynamic allocation succeeded.
                new_linked_element->_my_link = net2_link;
                net2_link_manager->_net2_links = new_linked_element;
                #ifdef NET2_DEBUG
                    net2_debug_success("net2_link_manager_register_link");
                #endif
            }
            else
            {
                // No, the new link element dynamic allocation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_manager_register_link", "The new link element dynamic allocation failed.");
                #endif
            }
        }
    }
    else
    {
        // No, we had problems getting the link manager instance.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_manager_register_link", "We had problems getting the link manager instance.");
        #endif
    }
    
    return result;
}

int net2_link_manager_check_socket(struct net2_socket_t* net2_socket, bool* found)
{
    int result = 0;
    
    struct net2_link_manager_t* net2_link_manager = NULL;
    
    // TEST : Did we get the link manager instance without any problem ?
    if(!net2_link_manager_get_instance(&net2_link_manager))
    {
        // Yes, we got the link manager instance without any problem.
        struct net2_link_linked_element_t* temp = net2_link_manager->_net2_links;
        
        // TEST : Has the link manager at least one link ?
        if(temp)
        {
            // Yes, the link manager has at least one link.
            // Parse the link manager linked list of links.
            while(!net2_link_compare_to_socket(temp->_my_link, net2_socket) && temp->_next_link)
            {
                temp = temp->_next_link;
            }
            
            *found = (net2_link_compare_to_socket(temp->_my_link, net2_socket));
            #ifdef NET2_DEBUG
                net2_debug_success("net2_link_manager_check_socket");
            #endif
        }
        else
        {
            // No, the link manager does not have any link yet.
            *found = false;
        }
    }
    else
    {
        // No, we had problems getting the link manager instance.
        *found = false;
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_manager_check_socket", "We had problems getting the link manager instance.");
        #endif
    }
    
    return result;
}
