#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP

#include "net2_link.h"

int main(int argc, char* argv[])
{
    unsigned int programme_return = EXIT_SUCCESS;
    
    printf("Checking the number of arguments. Is there the right number of arguments...");
    if(argc == 3)
    {
        printf("yes.\n");
        struct sockaddr_in ip_address;
        
        printf("Checking the IP address. Is \"%s\" a valid IP address...", argv[1]);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
            
            printf("Checking the port. Is %d a valid port...", atoi(argv[2]));
            if(atoi(argv[2]) >= 0 && atoi(argv[2]) < 65536)
            {
                printf("yes.\n");            
                struct net2_socket_t socket;
                
                printf("Checking the socket creation. Did the socket creation succeed...");
                if(net2_socket_create(&socket) >= 0)
                {
                    printf("yes.\n");
                    
                    printf("Check the socket connection. Did the socket connection succeed...");
                    if(!net2_socket_connect(&socket, ip_address.sin_addr.s_addr, atoi(argv[2])))
                    {
                        printf("yes.\n");
                        printf("The client is now connected to the server.\n");
                        
                        struct net2_link_t client;
                        net2_link_create(&client, &socket);                        
			            
	                    char data[13] = "Hello server\0";
	                    unsigned int data_length = 13;
	                    
	                    printf("Test the connection by sending \"%s\" to the server. Has the message been correctly sent...", data);
	                    if(net2_link_write(&client, (void*)data, data_length) != -1)
	                    {
	                        printf("yes.\n");
	                        printf("Waits for the feedback from the server. Has the feedback been well received...");
	                        if(net2_link_read(&client, (void*)data, data_length) >= 0)
	                        {
	                            printf("yes.\n");
	                            printf("The server says : \"%s\".\n", data);
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
