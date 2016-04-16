#include "net2_link_manager.h"

struct net2_link_manager_t** net2_link_manager_get_instance(void)
{
    // The pointer on the unique instance of link_manager.
    static struct net2_link_manager_t* link_manager = NULL;
    
    return &link_manager;
}

int net2_link_manager_create()
{
    // Function result
    int result = 0;
    
    // We dynamically allocate the link manager
    struct net2_link_manager_t** link_manager = net2_link_manager_get_instance();
    *link_manager = (struct net2_link_manager_t*)malloc(sizeof(struct net2_link_manager_t));
    
    // TEST : Is link manager dynamic allocation OK ?
    if(*link_manager)
    {
        // Yes, the link manager dynamic allocation is OK
        (*link_manager)->_links = NULL;
        #ifdef NET2_DEBUG
            net2_debug_success();
        #endif
    }
    else
    {
        // No, the link manager dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("Link manager dynamic allocation");
        #endif
    }
    
    return result;
}

int net2_link_manager_init()
{   
    int result = 0;
    
    // TEST : Is the link manager not instanced yet ?
    if(!*(net2_link_manager_get_instance()))
    {
        // Yes, the link manager can be instanced.
        result = net2_link_manager_create(); 
        
        // TEST : Did the link manager creation succeed ?
        if(!result)
        {
            // Yes, the link manager creation succeeded.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the link manager creation failed.
            #ifdef NET2_DEBUG
                net2_debug_failure("Link manager creation failed.");
            #endif
        }
    }
    else
    {
        // No, the link manager is already instanced.
        result = -2;
    }
    
    return result;
}

int net2_link_manager_register_link(struct net2_link_t* net2_link)
{
    // The function result
    int result = 0;
    
    struct net2_link_manager_t** net2_link_manager = net2_link_manager_get_instance();
    
    // TEST : Is the link manager already instanced ?
    if(*net2_link_manager)
    {        
        // Yes, the link manager is already instanced. 
        // TEST : Did the add of the link to the linked list of links succeed ?       
        if(!net2_link_append_to_linked_element(net2_link, &((*net2_link_manager)->_links)))
        {
            // Yes, the new link has been correctly appended to the linked list of links.
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // Yes, the new link has not been appended to the linked list of links.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("The link manager is not already instanced.");
            #endif
        }
    }
    else
    {
        // No, the link manager is not already instanced.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The link manager is not already instanced.");
        #endif
    }
    
    return result;
}

int net2_link_manager_get_link(struct net2_link_t** net2_link, unsigned int ip_address, unsigned short port)
{
    // The function result
    int result = 0;
    
    struct net2_link_manager_t** net2_link_manager = net2_link_manager_get_instance();
    
    // TEST : Is the link manager already instanced ?
    if(*net2_link_manager)
    {        
        // Yes, the link manager is already instanced.
        struct net2_link_linked_element_t* temp = (*net2_link_manager)->_links;
        
        // TEST : Has the link manager some links registered already ?
        if(temp)
        {
            // Yes, the link manager already has at least 1 net2 link registered.        
            // Parses the link manager linked list of links
            while(!net2_link_compare_to_address_and_port(temp->_my_link, ip_address, port) && temp->_next_link)
            {
                // We go to the next net2 link.
                temp = temp->_next_link;   
            }
            
            // TEST : Did the link manager find a link ?
            if(net2_link_compare_to_address_and_port(temp->_my_link, ip_address, port))
            {
                // Yes, the link manager found a link.
                *net2_link = temp->_my_link;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
            else
            {
                // No, the link manager did not find any link.
                *net2_link = NULL;
                #ifdef NET2_DEBUG
                    net2_debug_success();
                #endif
            }
        }
        else
        {
            // No, the link manager does not have any registered link yet.
            *net2_link = NULL;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
    }
    else
    {
        // No, the link manager is not already instanced.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The link manager is not already instanced.");
        #endif
    }
    
    return result;
}

int net2_link_manager_check_address_and_port(unsigned int address, unsigned short port, bool* found)
{
    int result = 0;
    
    struct net2_link_manager_t** net2_link_manager = net2_link_manager_get_instance();
    
    // TEST : Is the link manager already instanced ?
    if(*net2_link_manager)
    {
        // Yes, the link manager is already instanced.
        struct net2_link_linked_element_t* temp = (*net2_link_manager)->_links;
        
        // TEST : Has the link manager at least one link ?
        if(temp)
        {
            // Yes, the link manager has at least one link.
            // Parse the link manager linked list of links.
            while(!net2_link_compare_to_address_and_port(temp->_my_link, address, port) && temp->_next_link)
            {
                temp = temp->_next_link;
            }
            
            *found = (net2_link_compare_to_address_and_port(temp->_my_link, address, port));
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
        else
        {
            // No, the link manager does not have any link yet.
            *found = false;
            #ifdef NET2_DEBUG
                net2_debug_success();
            #endif
        }
    }
    else
    {
        // No, the link manager is not already instanced.
        *found = false;
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("The link manager is not already instanced.");
        #endif
    }
    
    return result;
}

int net2_link_manager_check_socket(struct net2_socket_t* net2_socket, bool* found)
{
    return net2_link_manager_check_address_and_port(net2_socket_get_ip(net2_socket), net2_socket_get_port(net2_socket), found);
}
