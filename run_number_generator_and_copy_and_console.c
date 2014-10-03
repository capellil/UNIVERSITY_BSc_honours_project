#include "channel.h"
#include "number_generator_process.h"
#include "copy_process.h"
#include "console_process_int.h"
#include "processes_list.h"

#include <stdio.h> // printf
#include <stdlib.h> // NULL | EXIT_SUCCESS

int main(int argc, char* argv[])
{
	int result = EXIT_SUCCESS;
	
	// Creation of the channel between the number generator and the copy channel
	struct one_2_one_channel_t* number_to_copy_channel = create_one_2_one_channel();
	if(number_to_copy_channel)
	{
		// Creation of the channel from the copy first output to the first console
		struct one_2_one_channel_t* copy_1_to_console_1_channel = create_one_2_one_channel();
		if(copy_1_to_console_1_channel)
		{
			// Creation of the channel from the copy second output to the second console
			struct one_2_one_channel_t* copy_2_to_console_2_channel = create_one_2_one_channel();
			if(copy_2_to_console_2_channel)
			{
				// Creation of the process generating numbers
				struct number_generator_process* number_generator_process = create_number_generator_process(0, 200, 1, number_to_copy_channel->channel_output_end);
				if(number_generator_process)
				{
					// Create of the process to copy in parallel 	
					struct copy_process* copy_process = create_copy_process(number_to_copy_channel->channel_input_end, copy_1_to_console_1_channel->channel_output_end, copy_2_to_console_2_channel->channel_output_end);
					if(copy_process)
					{
						// Creation of the first console process
						char prefix1[] = "Console 1 : \"";
						char suffix[] = "\".";
						struct console_process_int* console_process_int_1 = create_console_process_int(prefix1, suffix, copy_1_to_console_1_channel->channel_input_end);
						if(console_process_int_1)
						{
							// Creation of the second console process
							char prefix2[] = "Console 2 : \"";
							struct console_process_int* console_process_int_2 = create_console_process_int(prefix2, suffix, copy_2_to_console_2_channel->channel_input_end);
							if(console_process_int_2)
							{
								struct processes_list_t* processes_list = create_processes_list(4);
								if(processes_list != NULL)
								{
									insert_processes_list(processes_list, number_generator_process, number_generator_process->run);
									insert_processes_list(processes_list, copy_process, copy_process->run);
									insert_processes_list(processes_list, console_process_int_1, console_process_int_1->run);
									insert_processes_list(processes_list, console_process_int_2, console_process_int_2->run);
		
									run_in_parallel(processes_list);
								}	
								else
								{
									printf("FATAL ERROR : Impossible to create the processes list.\n");
								}
							}
							else
							{
								printf("FATAL ERROR : Impossible to create the second console process.\n");
							}
						}
						else
						{
							printf("FATAL ERROR : Impossible to create the first console process.\n");
						}
					}
					else
					{
						printf("FATAL ERROR : Impossible to create the copy process.\n");
					}
				}
				else
				{
					printf("FATAL ERROR : Impossible to create the number generator process.\n");
				}
			}
			else
			{
				printf("FATAL ERROR : Impossible to create the channel between the second copy output to the second console input.\n");
			}
		}
		else
		{
			printf("FATAL ERROR : Impossible to create the channel between the first copy output to the first console input.\n");
		}
	}
	else
	{
		printf("FATAL ERROR : Impossible to create the channel between the number generator output and the copy input.\n");
	}
	
	return result;
}
