#include "prefix_process.h"
#include "successor_process.h"
#include "copy_process.h"
#include "console_process_int.h"
#include "channel.h"
#include "processes_list.h"

#include <stdio.h> // printf
#include <stdlib.h> // NULL | EXIT_FAILURE | EXIT_SUCCESS

int main(int argc, char* argv[])
{
	int result = EXIT_FAILURE;
	
	// Creation of the channel between the prefix and the copy
	struct one_2_one_channel_t* prefix_to_copy = create_one_2_one_channel();
	if(prefix_to_copy)
	{
		// Creation of the channel between the copy and the console
		struct one_2_one_channel_t* copy_to_console = create_one_2_one_channel();
		if(copy_to_console)
		{
			// Creation of the channel between the copy and the successor
			struct one_2_one_channel_t* copy_to_successor = create_one_2_one_channel();
			if(copy_to_successor)
			{
				// Creation of the channel between the successor and the prefix
				struct one_2_one_channel_t* successor_to_prefix = create_one_2_one_channel();
				if(successor_to_prefix)
				{
					// Prefix
					int prefix_value = 0;
					struct prefix_process* prefix = create_prefix_process(prefix_value, successor_to_prefix->channel_input_end, prefix_to_copy->channel_output_end);
					if(prefix)
					{
						// Copy
						struct copy_process* copy = create_copy_process(prefix_to_copy->channel_input_end, copy_to_console->channel_output_end, copy_to_successor->channel_output_end);
						if(copy)
						{
							// Successors
							struct successor_process* successor = create_successor_process(copy_to_successor->channel_input_end, successor_to_prefix->channel_output_end);
							if(successor)
							{
								// Console
								char console_prefix[] = "";
								char console_suffix[] = "";
								struct console_process_int* console = create_console_process_int(console_prefix, console_suffix, copy_to_console->channel_input_end);
								if(console)
								{
									// Processes list
									struct processes_list_t* processes_list = create_processes_list(4);
									if(processes_list)
									{
										insert_processes_list(processes_list, prefix, prefix->run);
										insert_processes_list(processes_list, successor, successor->run);
										insert_processes_list(processes_list, copy, copy->run);
										insert_processes_list(processes_list, console, console->run);

										run_in_parallel(processes_list);
										
										result = EXIT_SUCCESS;
									}
									else
									{
										printf("FATAL ERROR : Impossible to create the processes list.\n");
									}
								}
								else
								{
									printf("FATAL ERROR : Impossible to create the console process.\n");
								}
							}
							else
							{
								printf("FATAL ERROR : Impossible to create the successor process.\n");
							}
						}
						else
						{
							printf("FATAL ERROR : Impossible to create the copy process.\n");
						}
					}
					else
					{
						printf("FATAL ERROR : Impossible to create the prefix process.\n");
					}
				}
				else
				{
					printf("FATAL ERROR : Impossible to create the channel between the successor output and the prefix input.\n");
				}
			}
			else
			{
				printf("FATAL ERROR : Impossible to create the channel between the copy second output and the successor input.\n");
			}
		}
		else
		{
			printf("FATAL ERROR : Impossible to create the channel between the copy first output and the console input.\n");
		}
	}
	else
	{
		printf("FATAL ERROR : Impossible to create the channel between the prefix output and the copy input.\n");
	}

	return result;
}
