#include <stdio.h>

#include "net2_debug.h"

const unsigned int NET2_DEBUG_MAX_MESSAGE_LENGTH = 256;

void net2_debug_success()
{
    printf("FUNCTION SUCCESS : %s\n", __FUNCTION__);
}

void net2_debug_failure(char* message)
{
    printf("FUNCTION FAILURE : \n");
    printf("\t- Function : %s\n", __FUNCTION__);
    printf("\t- Message : %s\n", message);    
    perror("\t- PError "); 
    fflush(stdout);
}

void spy(char* message)
{
    printf("SPY : %s\n", message);
    fflush(stdout);
}
