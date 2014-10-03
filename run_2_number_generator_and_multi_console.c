#include "channel.h"
#include "number_generator_process.h"
#include "multi_console_process.h"
#include "processes_list.h"
#include "delay_process.h"

#include <stdlib.h> // NULL

int main(int argc, char* argv[])
{
	// Creation of the channel
	struct one_2_one_channel_t* number_1_to_delayer = create_one_2_one_channel();
	struct one_2_one_channel_t* delayer_to_console = create_one_2_one_channel();
	struct one_2_one_channel_t* number_2_to_console = create_one_2_one_channel();

	// Creation of the process generating numbers
	struct number_generator_process* number_generator_process_1 = create_number_generator_process(0, 200, 1, number_1_to_delayer->channel_output_end);
	
	struct delayer_process* delayer_process = create_delayer_process(50, number_1_to_delayer->channel_input_end, delayer_to_console->channel_output_end);
	struct channel_end_generic_t* channel_end_generic_1 = (struct channel_end_generic_t*)malloc(sizeof(struct channel_end_generic_t));
	channel_end_generic_1->type = CHANNEL_INPUT_TYPE;
	channel_end_generic_1->channel_end = (void*)(delayer_to_console->channel_input_end);
	
	struct number_generator_process* number_generator_process_2 = create_number_generator_process(200, 400, 1, number_2_to_console->channel_output_end);
	struct channel_end_generic_t* channel_end_generic_2 = (struct channel_end_generic_t*)malloc(sizeof(struct channel_end_generic_t));
	channel_end_generic_2->type = CHANNEL_INPUT_TYPE;
	channel_end_generic_2->channel_end = (void*)(number_2_to_console->channel_input_end);
	
	// Creation of the process displaying what it receives
	char prefix[] = "Received from number generator process : \"";
	char suffix[] = "\".";
	struct multi_console_process* multi_console_process = create_multi_console_process(prefix, suffix, 2);
	add_to_alternative(channel_end_generic_1, multi_console_process->alternative);
	add_to_alternative(channel_end_generic_2, multi_console_process->alternative);
	
	struct processes_list_t* processes_list = create_processes_list(3);
	if(processes_list != NULL)
	{
		insert_processes_list(processes_list, number_generator_process_1, number_generator_process_1->run);
		insert_processes_list(processes_list, delayer_process, delayer_process->run);
		insert_processes_list(processes_list, number_generator_process_2, number_generator_process_2->run);
		insert_processes_list(processes_list, multi_console_process, multi_console_process->run);
	
		run_in_parallel(processes_list);
	}
	else
	{
		free(number_generator_process_1);
		number_generator_process_1 = NULL;
		
		free(number_generator_process_2);
		number_generator_process_2 = NULL;
		
		free(multi_console_process);
		multi_console_process = NULL;
	}

	free(number_1_to_delayer); 
	number_1_to_delayer = NULL;
	
	free(delayer_to_console);
	delayer_to_console = NULL;
	
	free(number_2_to_console);
	number_2_to_console = NULL;
	
	return EXIT_SUCCESS;
}
