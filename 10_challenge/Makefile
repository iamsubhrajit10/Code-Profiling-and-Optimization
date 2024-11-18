CC = g++
CFLAGS = -g -Wall -Werror -std=c++17

all: clean create_tests compile test

create_tests: create_testcases.cpp
	$(CC) $(CFLAGS) -o create_testcases create_testcases.cpp
	./create_testcases

test: compile testcase.txt
	./find_14_chars

compile: find_14_chars.cpp
	$(CC) $(CFLAGS) -o find_14_chars find_14_chars.cpp

#
# Clean the src dirctory
#
clean:
	rm -f create_testcases
	rm -f find_14_chars
	rm -f testcase.txt
