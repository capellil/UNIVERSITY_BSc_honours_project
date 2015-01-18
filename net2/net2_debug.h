#ifndef NET2_DEBUG_INCLUDED
#define NET2_DEBUG_INCLUDED

/**
 * @file net2_debug.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 25/12/2014
 **/

/**
 * @brief Displays the name of the function that has been successfully executed.
 * @param function_name The function name to be displayed.
 **/
void net2_debug_success(char* function_name);

/**
 * @brief Displays the name of the function and a message explaining what error has been encountered during the function execution. The message returned by perror is also displayed.
 * @param function_name The function name to be displayed.
 * @param message Error message to be displayed.
 **/
void net2_debug_failure(char* function_name, char* message);

/**
 * @brief Useful when inserting some checkpoints in the code. It displays the given message to show if a specific point in the code has been reached or not.
 * @param message Message to be displayed.
 **/
void spy(char* message);

#endif // NET2_DEBUG_INCLUDED INCLUDED
