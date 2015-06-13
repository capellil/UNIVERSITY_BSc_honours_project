#include "channel.h"
#include "number_generator_process.h"
#include "multi_console_process.h"
#include "processes_list.h"
#include "delay_process.h"

#include <stdlib.h> // NULL

int main(int argc, char* argv[])
{
	int result = EXIT_FAILURE;
	
	// Creation of the channel between the first number generator to the delay process
	struct one_2_one_channel_t* number_1_to_delay = create_one_2_one_channel();
	if(number_1_to_delay)
	{
		// Creation of the channel between the delay process and the multi console process
		struct one_2_one_channel_t* delay_to_console = create_one_2_one_channel();
		if(delay_to_console)
		{
			// Creation of the channel between the second number generator and the multi console process
			struct one_2_one_channel_t* number_2_to_console = create_one_2_one_channel();
			if(number_2_to_console)
			{
				// Creation of the first number generator
				struct number_generator_process* number_generator_process_1 = create_number_generator_process(0, 200, 1, number_1_to_delay->channel_output_end);
				if(number_generator_process_1)
				{
					// Creation of the delay process
					struct delay_process* delay_process = create_delay_process(50, number_1_to_delay->channel_input_end, delay_to_console->channel_output_end);
					if(delay_process)
					{
						// Creation of the generic channel
						struct channel_end_generic_t* channel_end_generic_1 = (struct channel_end_generic_t*)malloc(sizeof(struct channel_end_generic_t));
						if(channel_end_generic_1)
						{
							channel_end_generic_1->type = CHANNEL_INPUT_TYPE;
							channel_end_generic_1->channel_end = (void*)(delay_to_console->channel_input_end);
							
							// Creation of the second number generator process
							struct number_generator_process* number_generator_process_2 = create_number_generator_process(200, 400, 1, number_2_to_console->channel_output_end);
							if(number_generator_process_2)
							{
								struct channel_end_generic_t* channel_end_generic_2 = (struct channel_end_generic_t*)malloc(sizeof(struct channel_end_generic_t));
								channel_end_generic_2->type = CHANNEL_INPUT_TYPE;
								channel_end_generic_2->channel_end = (void*)(number_2_to_console->channel_input_end);
	
								// Creation of the process displaying what it receives
								char prefix[] = "Received from number generator process : \"";
								char suffix[] = "\".";
								struct multi_console_process* multi_console_process = create_multi_console_process(prefix, suffix, 2);
								if(multi_console_process)
								{
									add_to_alternative(channel_end_generic_1, multi_console_process->alternative);
									add_to_alternative(channel_end_generic_2, multi_console_process->alternative);
	
									struct processes_list_t* processes_list = create_processes_list(3);
									if(processes_list != NULL)
									{
										insert_processes_list(processes_list, number_generator_process_1, number_generator_process_1->run);
										insert_processes_list(processes_list, delay_process, delay_process->run);
										insert_processes_list(processes_list, number_generator_process_2, number_generator_process_2->run);
										insert_processes_list(processes_list, multi_console_process, multi_console_process->run);
	
										run_in_parallel(processes_list);
									}
									else
									{
										printf("FATAL ERROR : Impossible to create the processes list.\n");
									}
								}
								else
								{
									printf("FATAL ERROR : Impossible to create the multi console process.\n");
								}
							}
							else
							{
								printf("FATAL ERROR : Impossible to create the second number generator process.\n");
							}
						}
						else
						{
							printf("FATAL ERROR : Impossible to create the generic channel.\n");
						}
					}
					else
					{
						printf("FATAL ERROR : Impossible to create the delay process.\n");
					}
				}
				else
				{
					printf("FATAL ERROR : Impossible to create the first number generator process.\n");
				}
			}
			else
			{
				printf("FATAL ERROR : Impossible to create the channel between the second number generator output and the multi console input.\n");
			}
		}
		else
		{
			printf("FATAL ERROR : Impossible to create the channel between the delay output and the multi console input.\n");
		}
	}
	else
	{
		printf("FATAL ERROR : Impossible to create the channel between the first number generator output and the delay input.\n");
	}
	
	return result;
}
