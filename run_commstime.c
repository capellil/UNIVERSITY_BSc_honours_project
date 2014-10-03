#include "prefix_process.h"
#include "successor_process.h"
#include "copy_process.h"
#include "console_process_int.h"
#include "channel.h"
#include "processes_list.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	// Channels
	struct one_2_one_channel_t* prefix_to_copy = create_one_2_one_channel();
	struct one_2_one_channel_t* copy_to_console = create_one_2_one_channel();
	struct one_2_one_channel_t* copy_to_successor = create_one_2_one_channel();
	struct one_2_one_channel_t* successor_to_prefix = create_one_2_one_channel();

	// Prefix
	int prefix_value = 0;
	struct prefix_process* prefix = create_prefix_process(prefix_value, successor_to_prefix->channel_input_end, prefix_to_copy->channel_output_end);

	// Copy
	struct copy_process* copy = create_copy_process(prefix_to_copy->channel_input_end, copy_to_console->channel_output_end, copy_to_successor->channel_output_end);

	// Successors
	struct successor_process* successor = create_successor_process(copy_to_successor->channel_input_end, successor_to_prefix->channel_output_end);

	// Console
	char console_prefix[] = "";
	char console_suffix[] = "";
	struct console_process_int* console = create_console_process_int(console_prefix, console_suffix, copy_to_console->channel_input_end);

	// Processes list
	struct processes_list_t* processes_list = create_processes_list(4);

	insert_processes_list(processes_list, prefix, prefix->run);
	insert_processes_list(processes_list, successor, successor->run);
	insert_processes_list(processes_list, copy, copy->run);
	insert_processes_list(processes_list, console, console->run);

	run_in_parallel(processes_list);

	return EXIT_SUCCESS;
}
