#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS
#include <stdbool.h> // bool

#include "net2_link_manager.h"

/**
 * @brief This application tests the link manager.<br>
   It first consists in instanciating the link manager singleton through net2_link_manager_get_instance function.<br>
   Then it creates a link, connected with the test_link_manager_client programme one. This application will check the functions net2_link_manager_check_socket and net2_link_manager_register_link before and after the link gets registered.
 **/
int main(int argc, char* argv[])
{
    struct net2_link_manager_t* link_manager = NULL;
    
    // TEST : Did the link manager get instance succeed ?
    if(!net2_link_manager_get_instance(&link_manager))
    {
        // Yes, the link manager get instance succeed.
        printf("Link manager ok.\n");
        
        struct net2_link_t link;
        
        // TEST : Did the server link creation succeed ?
        if(net2_create_server_link(&link, 3001) >= 0)
        {
            // Yes, the server link has been correctly created.
            printf("The connection has been created but link NOT REGISTERED => the socket and the link should NOT be findable in the link manager links.\n");
            
            bool socket_check_result = false;
            
            // TEST : Did the research into the link manager succeed ?
            if(!net2_link_manager_check_socket((&link)->_net2_link_tx->_net2_socket, &socket_check_result))
            {
                // Yes, the research into the link manager succeed.
                // TEST : Is the link socket already present in the link manager links ? (SHOULD NOT)
                if(!socket_check_result)
                {
                    // No, the link socket is not already present in the link manager links.
                    printf("The socket has not been found in the link manager AS EXPECTED.\n");
                    
                    // TEST : Did the link registration succeed ?
                    if(!net2_link_manager_register_link(&link))
                    {
                        // Yes, the link registration succeed.
                        printf("The link is now REGISTERED => the socket and the link should now be both findable in the link manager links.\n");
                        
                        // TEST : Did the research into the link manager succeed ?
                        if(!net2_link_manager_check_socket((&link)->_net2_link_tx->_net2_socket, &socket_check_result))
                        {
                            // Yes, the research into the link manager succeed.
                            // TEST : Is the link socket already present in the link manager links ? (SHOULD)
                            if(socket_check_result)
                            {
                                // Yes, the socket is already present in the link manager links.
                                printf("The socket has been found in the link manager AS EXPECTED.\n");
                                
                                // TEST : Did the link registration again succeed ? (SHOULD NOT)
                                if(net2_link_manager_register_link(&link))
                                {
                                    // No, the link registration failed.
                                    printf("The link registration failed AS EXPECTED.\n");
                                }
                                else
                                {
                                    // Yes, it succeeded again but it is not the right behaviour.
                                    printf("The link registration succeeded but should have failed.\n");
                                }   
                            }
                            else
                            {
                                // No, the link socket is not already present in the link manager links.
                                printf("The socket is not present in the link manager, but should have been.\n");
                            }
                        }
                        else
                        {
                            // No, the link socket is not already present in the link manager links.
                            printf("The socket has not been found in the link manager but should have been.\n");
                        }
                    }
                    else
                    {
                        // No, the link registration failed.
                        printf("The link registration failed.\n");
                    }
                }
                else
                {
                    // Yes, the socket is already present in the link manager links.
                    printf("The socket has been found in the link manager but should not have.\n");
                }
            }
            else
            {
                // No, the research in the link manager links failed.
                printf("The research in the link manager links failed.\n");
            }
        }
        else
        {
            // No, the server link creation failed.
            printf("The connection has failed.\n");
        }
    }
    else
    {
        // No, the link manager get instance failed.
        printf("Link manager get instance failed.\n");
    }

    return EXIT_SUCCESS;
}
