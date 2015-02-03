#include <stdio.h> // printf

#include "net2_protocol.h"

const unsigned int NET2_MESSAGE_MAX_LENGTH = 1 << 8;

void net2_message_print(struct net2_message_t* net2_message)
{
    switch(net2_message->_type)
    {
        printf("MESSAGE DESCRIPTION\n"); fflush(stdout);
        printf("\t- Message type : "); fflush(stdout);
        case SEND :
            printf("SEND\n"); fflush(stdout);
            printf("\t- Message destination : %u\n", net2_message->_destination); fflush(stdout);
            printf("\t- Message source : %u\n", net2_message->_source); fflush(stdout);
            printf("\t- Message data length : %u\n", net2_message->_data_length); fflush(stdout); 
            if(net2_message->_data)
            {
                for(unsigned int i = 0; i < net2_message->_data_length; i++)
                {
                    printf("\t- Message data : %c\n", ((char*)(net2_message->_data))[i]); fflush(stdout);
                }
            }   
            else
            {
                printf("\t- Message data : NULL\n"); fflush(stdout);
            }
            break;
        case ACK :
            printf("ACK\n"); fflush(stdout);
            printf("\t- Message destination : %u\n", net2_message->_destination); fflush(stdout);
            break;
        case OPEN :
            printf("OPEN\n"); fflush(stdout);
            printf("\t- Message destination : %u\n", net2_message->_destination); fflush(stdout);
            printf("\t- Message source : %u\n", net2_message->_source); fflush(stdout);
            break;
        default :
            printf("UNKNOWN -> %u\n", net2_message->_type); fflush(stdout);
            break;
    }
}
