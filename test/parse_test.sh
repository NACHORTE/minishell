#!/bin/bash

if [ $# -ge 1 ]; then
	if [ $1 = "-err" ]; then
		echo -e "\033[0;36mExecuting parse_test with -err flag\033[0m"
		flag=$1
	fi
	if [ $1 = "-all" ]; then
		echo -e "\033[0;36mExecuting parse_test with -all flag\033[0m"
		flag=$1
	fi
fi

make -C libft/ >/dev/null
if [ $? -ne 0 ]; then
	echo -e "\033[0;31mError: libft compilation failed\033[0m"
	exit 1
fi
gcc -fsanitize=leak \
	-Iinclude -Ilibft/include \
	src/parse/*.c src/variables/*.c test/test_parse.c \
	libft/libft.a \
	-o parse_test
if [ $? -ne 0 ]; then
	echo -e "\033[0;31mError: parse_test compilation failed\033[0m"
	exit 1
fi
./parse_test $flag
rm -rf parse_test