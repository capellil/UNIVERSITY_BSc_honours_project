#include "channel.h"
#include "number_generator_process.h"
#include "delayer_process.h"
#include "console_process.h"
#include "processes_list.h"

#include <stdlib.h> // NULL

int main(int argc, char* argv[])
{
	// Creation of the channel between the number generator process and the delayer process
	struct one_2_one_channel_t* number_2_delayer = create_one_2_one_channel();

	// Creation of the channel between the delayer process and the console process
	struct one_2_one_channel_t* delayer_2_console = create_one_2_one_channel();

	// Creation of the process generating numbers
	struct number_generator_process* number_generator_process = create_number_generator_process(0, 100, 1, number_2_delayer->channel_output_end);

	// Creation of the process delaying transmission
	struct delayer_process* delayer_process = create_delayer_process(200000, number_2_delayer->channel_input_end, delayer_2_console->channel_output_end);

	// Creation of the process displaying what it receives
	char prefix[] = "Received from number generator process : \"";
	char suffix[] = "\".";
	struct console_process* console_process = create_console_process(prefix, suffix, delayer_2_console->channel_input_end);

	struct processes_list_t* processes_list = create_processes_list(3);
	if(processes_list != NULL)
	{
		insert_processes_list(processes_list, number_generator_process, number_generator_process->run);
		insert_processes_list(processes_list, delayer_process, delayer_process->run);
		insert_processes_list(processes_list, console_process, console_process->run);
		
		run_in_parallel(processes_list);
	}
	else
	{
		free(number_generator_process);
		number_generator_process = NULL;
		free(delayer_process);
		delayer_process = NULL;
		free(console_process);
		console_process = NULL;
	}

	free(number_2_delayer);
	free(delayer_2_console);

	return EXIT_SUCCESS;
}
