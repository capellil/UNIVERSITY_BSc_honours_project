#include "channel.h"
#include "number_generator_process.h"
#include "console_process_int.h"
#include "processes_list.h"

#include <stdlib.h> // NULL | EXIT_SUCCESS

int main(int argc, char* argv[])
{
	// Creation of the channel
	struct one_2_one_channel_t* one_2_one_channel = create_one_2_one_channel();

	// Creation of the process generating numbers
	struct number_generator_process* number_generator_process = create_number_generator_process(0, 200, 1, one_2_one_channel->channel_output_end);

	// Creation of the process displaying what it receives
	char prefix[] = "Received from number generator process : \"";
	char suffix[] = "\".";
	struct console_process_int* console_process_int = create_console_process_int(prefix, suffix, one_2_one_channel->channel_input_end);
	
	struct processes_list_t* processes_list = create_processes_list(2);
	if(processes_list != NULL)
	{
		insert_processes_list(processes_list, number_generator_process, number_generator_process->run);
		insert_processes_list(processes_list, console_process_int, console_process_int->run);
	
		run_in_parallel(processes_list);
	}
	else
	{
		free(number_generator_process);
		number_generator_process = NULL;
		free(console_process_int);
		console_process_int = NULL;
	}

	free(one_2_one_channel);
	
	return EXIT_SUCCESS;
}
