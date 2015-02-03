#include <stdlib.h> // NULL
#include <pthread.h> // pthread_t | pthread_create and so on...
#include <string.h> // memcpy
#include <stdbool.h> // bool

#ifdef NET2_DEBUG
    #include "net2_debug.h"
#endif

#include "net2_link_server.h"
#include "net2_link_manager.h"
#include "net2_link.h"

struct net2_link_server_t** net2_link_server_get_instance(void)
{
    static struct net2_link_server_t* net2_link_server_singleton = NULL;
    
    return &net2_link_server_singleton;
}

int net2_link_server_init(unsigned short port)
{
    int result = 0;

    struct net2_link_server_t** net2_link_server = net2_link_server_get_instance();
    
    // TEST : Is the link server singleton already instanced ?
    if(*net2_link_server)
    {
        // Yes, the link server singleton is already instanced.
        result = -2;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_server_create", "The link server singleton is already instanced.");
        #endif
    }
    else
    {
        // No, the link server singleton is not already instanced.
		// Instanciate the link server singleton.
		result = net2_link_server_create(port);

		// TEST : Did the link server singleton instanciation succeed ?
		if(!result)
		{
			// Yes, the link server singleton instanciation succeeded.
			// Get the instance
			net2_link_server = net2_link_server_get_instance();
			
			// Create a thread to run the link server on
			pthread_t* link_server_thread = (pthread_t*)malloc(sizeof(pthread_t));
			
			// TEST : Did the thread dynamic allocation succeed ? 
			if(link_server_thread)
		    {
		        // Yes, the thrrad dynamic allocation succeeded.
			    (*net2_link_server)->_server_thread = link_server_thread;
			    void* (*net2_link_server_run_function_pointer) (void*) = &net2_link_server_run;
			    void* net2_link_server_parameter = (void*)*net2_link_server;
			    pthread_create(link_server_thread, NULL, net2_link_server_run_function_pointer, net2_link_server_parameter);
			    #ifdef NET2_DEBUG
				    net2_debug_success("net2_link_server_init");
			    #endif
		    }
		    else
		    {
			    // No, the thread dynamic allocation failed.
			    result = -1;
			    #ifdef NET2_DEBUG
				    net2_debug_failure("net2_link_server_init", "Thread dynamic allocation failed.");
			    #endif
		    }
		}
		else
		{
			// No, the link server singleton instanciation failed.
			result = -1;
			#ifdef NET2_DEBUG
				net2_debug_failure("net2_link_server_init", "Link server singleton instance failed.");
			#endif
		}
	}
	
	return result;
}

int net2_link_server_create(unsigned short port)
{
	int result = 0;
    
	// Dynamically allocate the link server
	struct net2_link_server_t** net2_link_server_singleton = net2_link_server_get_instance();
	*net2_link_server_singleton = (struct net2_link_server_t*)malloc(sizeof(struct net2_link_server_t));

    // TEST : Did the link server dynamic allocation succeed ?
    if(*net2_link_server_singleton)
    {
        // Yes, the link server dynamic allocation succeed.
		// Create the links server socket and binds it to the given port.
		result = net2_socket_create_and_bind(&((*net2_link_server_singleton)->_server_socket), port);
		
		// TEST : Did the server socket creation and binding succeed ?
		if(!result)
		{
			// Yes, the server socket creation and binding succeeded.
			// Listen on the server socket
			result = net2_socket_listen(&((*net2_link_server_singleton)->_server_socket));
			
			// TEST : Did the server socket listening succeed ?
			if(!result)
			{
				// Yes, the server socket listening succeeded.
		        #ifdef NET2_DEBUG
		            net2_debug_success("net2_link_server_create");
		        #endif
			}
			else
			{
				// No, the server socket listening failed.
        		result = -1;
		        #ifdef NET2_DEBUG
		            net2_debug_failure("net2_link_server_create", "The server socket listening failed.");
		        #endif
			}
		}
		else
		{
			// No, the server socket creation and binding failed.
        	result = -1;
	        #ifdef NET2_DEBUG
	            net2_debug_failure("net2_link_server_create", "The server socket creation and binding failed.");
    	    #endif
		}
    }
    else
    {
        // No, the link server dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_server_create", "The link server dynamic allocation failed.");
        #endif
    }
    
    return result;
}

void* net2_link_server_run(void* net2_link_server_to_run)
{
	struct net2_link_server_t* net2_link_server = (struct net2_link_server_t*)net2_link_server_to_run;
	struct net2_socket_t* server_socket = &(net2_link_server->_server_socket);
	struct net2_socket_t client_socket;
	void* run_result = NULL;
	bool check_socket_result = false;

	while(1)
	{	
		if(net2_socket_accept(server_socket, &client_socket) >= 0)
		{
			// The socket has been correctly accepted.
			// TEST : Did the research into the link manager succeed ?
			if(!net2_link_manager_check_socket(&client_socket, &check_socket_result))
			{
			    // Yes, the research into the link manager succeed ?
			    // TEST : Is the client socket already present in one of the link manager registered links ?
			    if(!check_socket_result)
			    {
			        // No, the socket is not already present in one of the link manager registered links.
			        // Dynamically allocate the link to store the socket in later on.
			        struct net2_link_t* client_link = (struct net2_link_t*)malloc(sizeof(struct net2_link_t));
			        
			        // TEST : Did the link dynamic allocation succeed ?
			        if(client_link)
			        {
			            // Yes, the link dynamic allocation succeed.
			            net2_link_create(client_link, &client_socket);
			            
			            // TEST : Did the link registration succeed ?
			            if(!net2_link_manager_register_link(client_link))
			            {
			                // Yes, the link registration succeeded.
			                signed char data = 0;
	                        unsigned int data_length = sizeof(signed char);
	                        
	                        // TEST : Has the confirmation been successfully sent to the client ?
		                    if(net2_socket_write(&client_socket, (void*)&data, data_length) >= 0)
		                    {
		                        // Yes, the confirmation has been successfully sent to the client.
		                        // TEST : Did the new link run succeed ?
		                        if(!net2_link_server_new_link_to_run(client_link))
		                        {
		                            // Yes, the new link run succeeded.
		                            run_result = net2_link_server_to_run;
		                            #ifdef NET2_DEBUG
                                        net2_debug_success("net2_link_server_run");
                                    #endif
		                        }
		                        else
		                        {
		                            // No, the new link run failed.
	                                run_result = NULL;
			                        #ifdef NET2_DEBUG
                                        net2_debug_failure("net2_link_server_run", "The new link run failed.");
                                    #endif
		                        }
	                        }
	                        else
	                        {
	                            // No, the confirmation failed during its communication.
	                            run_result = NULL;
			                    #ifdef NET2_DEBUG
                                    net2_debug_failure("net2_link_server_run", "The confirmation failed during its communication.");
                                #endif
	                        }
			            }
			            else
			            {
			                // No, the link registration failed.
			                run_result = NULL;
			                #ifdef NET2_DEBUG
                                net2_debug_failure("net2_link_server_run", "The link registration failed.");
                            #endif
			            }
			        }
			        else
			        {
			            // No, the link dynamic allocation failed.
			            run_result = NULL;
			            #ifdef NET2_DEBUG
                            net2_debug_failure("net2_link_server_run", "The link dynamic allocation failed.");
                        #endif
			        }
		        }
		        else
		        {
		            // Yes, the client socket is already present in the link manager links.
	                signed char data = -1;
	                unsigned int data_length = sizeof(signed char);
	                
	                // TEST : Has the confirmation been successfully sent to the client ?
		            if(net2_socket_write(&client_socket, (void*)&data, data_length) >= 0)
		            {
		                // Yes, the confirmation has been successfully sent to the client.
		                run_result = net2_link_server_to_run;
		                #ifdef NET2_DEBUG
		                    net2_debug_success("net2_link_server_run");
		                #endif
		            }
		            else
		            {
		                // No, the confirmation failed during its communication.
		                run_result = NULL;
		                #ifdef NET2_DEBUG
                            net2_debug_failure("net2_link_server_run", "The confirmation failed during its communication");
                        #endif
		            }
		        }
		    }
		    else
		    {
		        // No, the research into the link manager failed.
                run_result = NULL;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_server_run", "The research into the link manager failed.");
                #endif
		    }
		}
	}

	return run_result;
}

int net2_link_server_new_link_to_run(struct net2_link_t* link_to_run)
{
    int result = 0;

    struct net2_link_server_t** net2_link_server = net2_link_server_get_instance();
    
    // TEST : Is the link server already instanced ?
    if(*net2_link_server)
    {
        // Yes, the link server is already instanced.
        pthread_t* new_link_thread = (pthread_t*)malloc(sizeof(pthread_t));
		                            
        // TEST : Did the link thread dynamic allocation succeed ?
        if(new_link_thread)
        {
            // Yes, the link thread dynamic allocation succeeded.
            struct net2_link_thread_linked_element_t* new_element = (struct net2_link_thread_linked_element_t*)malloc(sizeof(struct net2_link_thread_linked_element_t));
            
            // TEST : Did the new element dynamic allocation succeed ?
            if(new_element)
            {
                // Yes, the new element dynamic allocation succeeded.
                new_element->_my_thread = new_link_thread;
                new_element->_my_link = link_to_run;
                struct net2_link_thread_linked_element_t** temp = &((*net2_link_server)->_link_threads);
                
                // TEST : Is there already at least one link ?
                if(*temp)
                {
                    // Yes, there is already at least one link.
                    while(*temp)
                    {
                        *temp = (*temp)->_next_link;
                    }
                    
                    *temp = new_element;
                }
                else
                {
                    // No, there is not any links yet.
                    *temp = new_element;
                }
                
                void* new_link_to_run_parameters = link_to_run;
                void* (*new_link_to_run) (void*) = &net2_link_run;
                pthread_create(new_link_thread, NULL, new_link_to_run, new_link_to_run_parameters);
                #ifdef NET2_DEBUG
                    net2_debug_success("net2_link_server_new_link_to_run");
                #endif
            }
            else
            {
                // No, the new element dynamic allocation failed.
                result = -1;
                #ifdef NET2_DEBUG
                    net2_debug_failure("net2_link_server_new_link_to_run", "The new element dynamic allocation failed.");
                #endif
            }
        }
        else
        {  
            // No, the link thread dynamic allocation failed.
            result = -1;
            #ifdef NET2_DEBUG
                net2_debug_failure("net2_link_server_new_link_to_run", "The link thread dynamic allocation failed.");
            #endif
        }
    }
    else
    {
        // No, the link thread dynamic allocation failed.
        result = -1;
        #ifdef NET2_DEBUG
            net2_debug_failure("net2_link_server_new_link_to_run", "The link server is not instanced yet.");
        #endif
    }
    
    return result;
}
