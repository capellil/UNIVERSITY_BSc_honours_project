#!/bin/bash

flags="-Wall -std=c99 -pthread"

function thd_LINK
{
	if [ $# -eq 0 ]; then
		echo "NO FILE TO LINK";
		exit 0;
	elif [ $# -eq 1 ]; then
		echo "1 FILE TO LINK";
	else 
		echo "$# FILES TO LINK";
	fi

	for i in $*; do
		if [ -e $i ]; then
			if [ `gcc -o ${i%??} $i net2/net2*.o processes/process*.o $flags 2>&1 > /dev/null | wc -l` -eq 0 ]; then
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

files_to_LINK=`find -maxdepth 2 -name "test*.o"`
thd_LINK $files_to_LINK
