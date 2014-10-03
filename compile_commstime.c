#!/bin/bash

gcc -c run_commstime.c\
	channel.c\
	processes_list.c\
	prefix_process.c\
	successor_process.c\
	copy_process.c\
	console_process_int.c\
	-Wall -std=c99
