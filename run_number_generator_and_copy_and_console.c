#include "channel.h"
#include "number_generator_process.h"
#include "copy_process.h"
#include "console_process_int.h"
#include "processes_list.h"

#include <stdlib.h> // NULL
#include <pthread.h> // pthread_t

int main(int argc, char* argv[])
{
	// Creation of the channel
	struct one_2_one_channel_t* number_to_copy_channel = create_one_2_one_channel();
	struct one_2_one_channel_t* copy_1_to_console_1_channel = create_one_2_one_channel();
	struct one_2_one_channel_t* copy_2_to_console_2_channel = create_one_2_one_channel();

	// Creation of the process generating numbers
	struct number_generator_process* number_generator_process = create_number_generator_process(0, 200, 1, number_to_copy_channel->channel_output_end);

	// Create of the process to copy in parallel 	
	struct copy_process* copy_process = create_copy_process(number_to_copy_channel->channel_input_end, copy_1_to_console_1_channel->channel_output_end, copy_2_to_console_2_channel->channel_output_end);

	// Creation of the process displaying what it receives
	char prefix1[] = "Console 1 : \"";
	char prefix2[] = "Console 2 : \"";
	char suffix[] = "\".";
	struct console_process_int* console_process_int_1 = create_console_process_int(prefix1, suffix, copy_1_to_console_1_channel->channel_input_end);
	struct console_process_int* console_process_int_2 = create_console_process_int(prefix2, suffix, copy_2_to_console_2_channel->channel_input_end);

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
		free(copy_process);
		copy_process = NULL;
		free(console_process_int_1);
		console_process_int_1 = NULL;
		free(console_process_int_2);
		console_process_int_2 = NULL;
	}

	free(number_to_copy_channel);
	free(copy_1_to_console_1_channel);
	free(copy_2_to_console_2_channel);
	
	return EXIT_SUCCESS;
}
