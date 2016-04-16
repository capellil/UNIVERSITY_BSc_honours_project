#ifndef NET2_DEBUG_INCLUDED
#define NET2_DEBUG_INCLUDED

/**
 * @file net2_debug.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 25/12/2014
 **/

/// This variable stores the maximum length ot debug messages.
const unsigned int NET2_DEBUG_MAX_MESSAGE_LENGTH;

#ifdef NET2_DEBUG 
 	/**
	 * @brief Displays the name of the function that has been successfully executed.
	 **/
    #define net2_debug_success() printf("FUNCTION SUCCESS : %s\n", __FUNCTION__);

	/**
	 * @brief Displays the name of the function and a message explaining what error has been encountered during the function execution. The message returned by perror is also displayed.
	 * @param message Error message to be displayed.
	 **/
    #define net2_debug_failure(message) printf("FUNCTION FAILURE : \n"); \
                                        printf("\t- Function : %s\n", __FUNCTION__); \
                                        printf("\t- Message : %s\n", message); \
                                        perror("\t- PError "); \
                                        fflush(stdout);
#else
    /**
	 * @brief Displays the name of the function that has been successfully executed.
	 **/
    #define net2_debug_success() {}

	/**
	 * @brief Displays the name of the function and a message explaining what error has been encountered during the function execution. The message returned by perror is also displayed.
	 * @param message Error message to be displayed.
	 **/
    #define net2_debug_failure(message) {}
#endif

/**
 * @brief Useful when inserting some checkpoints in the code. It displays the given message to show if a specific point in the code has been reached or not.
 * @param message Message to be displayed.
 **/
void spy(char* message);

#endif // NET2_DEBUG_INCLUDED INCLUDED
