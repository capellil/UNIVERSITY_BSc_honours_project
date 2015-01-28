/**
 * @file test_link_client.c
 * @author Capelli, Ludovic
 * @version 2.0
 * @date 22/01/2015
 **/
 
#include <stdio.h> // printf
#include <stdlib.h> // EXIT_SUCCESS | EXIT_FAILURE
#include <netinet/in.h> // sockaddr_in | IPPROTO_TCP | IPPROTO_UDP

#include "net2_link.h"

/**
 * @brief Creates a link behaving as the client : it connects to the given server (e.g : ip address and port). First, it creates the corresponding socket and the wraps it into a link.
 * 
 * Argument 0 : the name of the programme. (hidden)<br>
 * Argument 1 : The IP address to use.<br>
 * Argument 2 : The port to use.
 **/
int main(int argc, char* argv[])
{
    unsigned int programme_return = EXIT_SUCCESS;
    
    printf("Checking the number of arguments. Is there the right number of arguments..."); fflush(stdout);
    if(argc == 3)
    {
        printf("yes.\n");
        struct in_addr ip_address;
        
        printf("Checking the IP address. Is \"%s\" a valid IP address...", argv[1]); fflush(stdout);
        if(inet_pton(AF_INET, argv[1], &ip_address) > 0)
        {
            printf("yes.\n");
            
            printf("Checking the port. Is %d a valid port...", atoi(argv[2])); fflush(stdout);
            if(atoi(argv[2]) >= 0 && atoi(argv[2]) < 65536)
            {
                printf("yes.\n");            
                struct net2_socket_t socket;
                
                printf("Checking the socket creation. Did the socket creation succeed..."); fflush(stdout);
                if(net2_socket_create(&socket) >= 0)
                {
                    printf("yes.\n");
                    
                    printf("Check the socket connection. Did the socket connection succeed..."); fflush(stdout);
                    if(!net2_socket_connect(&socket, htonl(ip_address.s_addr), atoi(argv[2])))
                    {
                        printf("yes.\n");
                        printf("The client is now connected to the server.\n");
                        
                        struct net2_link_t client;
                        net2_link_create(&client, &socket);                        
			            
	                    char data[13] = "Hello server\0";
	                    unsigned int data_length = 13;
	                    
	                    printf("Test the connection by sending \"%s\" to the server. Has the message been correctly sent...", data); fflush(stdout);
	                    if(net2_link_write(&client, (void*)data, data_length) != -1)
	                    {
	                        printf("yes.\n");
	                        printf("Waits for the feedback from the server. Has the feedback been well received..."); fflush(stdout);
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
