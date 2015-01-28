#include <stdio.h>

#include "net2_debug.h"

const unsigned int NET2_DEBUG_MAX_MESSAGE_LENGTH = 256;

void net2_debug_success(char* function_name)
{
    printf("FUNCTION SUCCESS : %s\n", function_name);
}

void net2_debug_failure(char* function_name, char* message)
{
    printf("FUNCTION FAILURE : \n");
    printf("\t- Function : %s\n", function_name);
    printf("\t- Message : %s\n", message);    
    perror("\t- PError "); 
    fflush(stdout);
}

void spy(char* message)
{
    printf("SPY : %s\n", message);
    fflush(stdout);
}
