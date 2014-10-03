#include "channel.h"
#include "number_generator_process.h"
#include "delayer_process.h"
#include "console_process_int.h"
#include "processes_list.h"

#include <stdio.h> // printf
#include <stdlib.h> // NULL | EXIT_SUCCESS | EXIT_FAILURE

int main(int argc, char* argv[])
{
	int result = EXIT_FAILURE;
	
	// Creation of the channel between the number generator process and the delayer process
	struct one_2_one_channel_t* number_to_delayer = create_one_2_one_channel();
	if(number_to_delayer)
	{
		// Creation of the channel between the delayer process and the console process
		struct one_2_one_channel_t* delay_to_console = create_one_2_one_channel();
		if(delay_to_console)
		{
			// Creation of the process generating numbers
			struct number_generator_process* number_generator_process = create_number_generator_process(0, 100, 1, number_to_delayer->channel_output_end);
			if(number_generator_process)
			{
				// Creation of the process delaying transmission
				struct delayer_process* delayer_process = create_delayer_process(200000, number_to_delayer->channel_input_end, delay_to_console->channel_output_end);
				if(delayer_process)
				{
					// Creation of the process displaying what it receives
					char prefix[] = "Received from number generator process : \"";
					char suffix[] = "\".";
					struct console_process_int* console_process_int = create_console_process_int(prefix, suffix, delay_to_console->channel_input_end);
					if(console_process_int)
					{
						struct processes_list_t* processes_list = create_processes_list(3);
						if(processes_list != NULL)
						{
							insert_processes_list(processes_list, number_generator_process, number_generator_process->run);
							insert_processes_list(processes_list, delayer_process, delayer_process->run);
							insert_processes_list(processes_list, console_process_int, console_process_int->run);
		
							run_in_parallel(processes_list);
						
							result = EXIT_SUCCESS;
						}
						else
						{
							printf("FATAL ERROR : Impossible to create processes list.");
						}
					}
					else
					{
						printf("FATAL ERROR : Impossible to create the console process.");
					}
				}
				else
				{
					printf("FATAL ERROR : Impossible to create the delay process.");
				}
			}
			else
			{
				printf("FATAL ERROR : Impossible to create the number generator process.");
			}
		}
		else
		{
			printf("FATAL ERROR : Impossible to create the channel between the delay output and the console input.");
		}
	}
	else
	{
		printf("FATAL ERROR : Impossible to create the channel between the number generator output and the delay input.");
	}

	return result;
}
