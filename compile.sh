#!/bin/bash

flags="-Wall -std=c99" #add -DNET2_DEBUG option to enable debug messages to be displayed

function thd_compile
{
	if [ $# -eq 0 ]; then
		echo "NO FILE TO COMPILE";
		exit 0;
	elif [ $# -eq 1 ]; then
		echo "1 FILE TO COMPILE";
	else 
		echo "$# FILES TO COMPILE";
	fi

	for i in $*; do
		if [ -e $i ]; then
			if [ `gcc -c $i $flags -o ${i%?}o 2>&1 > /dev/null | wc -l` -eq 0 ]; then
				echo -e "\e[32mSUCCESS\e[0m \c";
			else
				echo -e "\e[7m\e[31mFAILURE\e[0m \c";
			fi
			echo " \"$i\""
		else
			echo -e "\tFILE \"$i\" NOT FOUND."
		fi
	done
}

clear

files_to_compile=`find -name "*.c"`
thd_compile $files_to_compile
