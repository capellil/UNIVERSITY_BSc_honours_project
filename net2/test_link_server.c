#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP
#include <string.h> // strcpy

#include "net2_link.h"

int main(int argc, char* argv[])
{
    unsigned int programme_return = EXIT_SUCCESS;
    
    printf("Checking the number of arguments. Is there the right number of arguments...");
    if(argc == 2)
    {
        printf("yes.\n");
        
        printf("Checking the port. Is %d a valid port...", atoi(argv[1]));
        if(atoi(argv[1]) >= 0 && atoi(argv[1]) < 65536)
        {
            printf("yes.\n");            
            struct net2_socket_t socket;
            
            printf("Checking the socket creation and binding. Did the socket creation and binding succeed...");
            if(net2_socket_create_and_bind(&socket, atoi(argv[1])) >= 0)
            {
                printf("yes.\n");
                
                printf("Tries to listen on the socket. Did the listening succeed...");
                if(!net2_socket_listen(&socket))
                {
                    printf("yes.\n");
                    
                    struct net2_socket_t client_socket;
                    printf("Tries to accept the client connection. Did the client connection acceptation succeed...");
                    if(net2_socket_accept(&socket, &client_socket) >= 0)
                    {
                        printf("yes.\n");
                        printf("The server is now connected to the server.\n");
                        
                        struct net2_link_t server;                        
                        net2_link_create(&server, &client_socket);
	                
	                    char data[13];
	                    unsigned int data_length = 13;
	                    
	                    printf("Waits for client to send a message. Has the message been well received...");
                        if(net2_link_read(&server, (void*)data, data_length) >= 0)
                        {
                            printf("yes.\n");
                            printf("The server said : \"%s\".\n", data);
                            
                            strcpy(data, "Hello client\0");
                            
                            printf("Reply by sending \"%s\". Has the reply been sent...", data);
	                        if(net2_link_write(&server, (void*)data, data_length) != -1)
	                        {
	                            printf("yes.\n");
                            }
                            else
                            {
                                printf("no.\n");
                                programme_return = EXIT_FAILURE;
                            }
                        }
                        else
                        {
                            printf("no.\n");
    			            programme_return = EXIT_FAILURE;
                        }
                    }
                    else
                    {
                        printf("no.\n");
                        programme_return = EXIT_FAILURE;
                    }
                }
                else
                {
                    printf("no.\n");
                    programme_return = EXIT_FAILURE;
                }
            }
            else
            {
                printf("no.\n");
                programme_return = EXIT_FAILURE;
            }
        }
        else
        {
            printf("no.\n");
            programme_return = EXIT_FAILURE;
        }
    }
    else
    {
        printf("no.\n");
        programme_return = EXIT_FAILURE;
    }

    return programme_return;
}
