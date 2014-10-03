/**
 * @file processes_list.h
 * @author Ludovic Capelli
 * @version 1.0
 * @date 26/09/2014
 **/

#include <stdio.h> // NULL
#include <stdlib.h> // free | malloc

/**
 * @brief A dynamic list of processes
 * 
 * It contains an array of processes pointers and an array of function pointers for the processes run function.
 **/
struct processes_list_t
{
	/// All the processes
	void** processes;
	
	/// All the processes run function to be called to execute the corresponding process.
	void* (**runs)(void*);
	
	/// Indicates how many processes are stored.
	size_t used;
	
	/// Indicates the current maximum size of the processes_list_t
	size_t size;
};

/**
 * @brief Creates and initialises the given processes list to the given initial size.
 * @param processes_list[out] The processes list to be initialised.
 * @param initial_size[in] The size the processes list will be initialised to.
 * @return The created processes list.
 * @pre \li processes_list != NULL
 * @post \li processes_list contains <initial_size> processes or NULL if it failed.
 **/
struct processes_list_t* create_processes_list(size_t initial_size);

/**
 * @brief Inserts the given process with the given run function in the given processes list.
 * @param processes_list[out] The processes list to insert elements in.
 * @param process[in] The process to be inserted.
 * @param run[in] The process run function to be inserted.
 * @pre \li processes_list != NULL
 			\li process != NULL
 			\li run != NULL
 * @post The process and process run function have been inserted in the given processes list.
 **/
void insert_processes_list(struct processes_list_t* processes_list, void* process, void* (*run)(void*));

/**
 * @brief Frees every process from the processes list, and the processes list itself then.
 * @param processes_list[out] The processes list to be freed.
 * @pre \li processes_list has not been freed yet.
 			\li processes_list processes have not been freed yet.
 * @post \li Every processes_list process has beed freed.
 			 \li processes_list has been freed and is now equal to NULL.
 **/
void free_processes_list(struct processes_list_t* processes_list) ;

/**
 * @brief Runs in parallel the given processes list. Each process will be run in a different thread.
 * @param processes_list[in] The processes list containing the processes to be run in parallel.
 * @pre \li processes_list != NULL
 * @post \li processes_list processes are run in a thread for each.
 **/
void run_in_parallel(struct processes_list_t* processes);
