#include <stdio.h>

#include "net2_debug.h"

const unsigned int NET2_DEBUG_MAX_MESSAGE_LENGTH = 512;

void spy(char* message)
{
    printf("SPY : %s\n", message);
    fflush(stdout);
}
