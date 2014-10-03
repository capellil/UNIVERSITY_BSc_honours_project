#include "channel.h"
#include "number_generator_process.h"
#include "console_process.h"
#include "processes_list.h"

#include <stdlib.h> // NULL
#include <pthread.h> // pthread_t

int main(int argc, char* argv[])
{
	// Creation of the channel
	struct one_2_one_channel_t* one_2_one_channel = create_one_2_one_channel();

	// Creation of the process generating numbers
	struct number_generator_process* number_generator_process = create_number_generator_process(0, 200, 1, one_2_one_channel->channel_output_end);

	// Creation of the process displaying what it receives
	char prefix[] = "Received from number generator process : \"";
	char suffix[] = "\".";
	struct console_process* console_process = create_console_process(prefix, suffix, one_2_one_channel->channel_input_end);
	
	struct processes_list_t* processes_list = create_processes_list(2);
	if(processes_list != NULL)
	{
		insert_processes_list(processes_list, number_generator_process, number_generator_process->run);
		insert_processes_list(processes_list, console_process, console_process->run);
	
		run_in_parallel(processes_list);
	}
	else
	{
		free(number_generator_process);
		number_generator_process = NULL;
		free(console_process);
		console_process = NULL;
	}

	free(one_2_one_channel);
	
	return EXIT_SUCCESS;
}
