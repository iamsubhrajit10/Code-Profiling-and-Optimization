CC = g++
CFLAGS = -g -Wall -Werror -std=c++17

all: clean create_tests_all compile test

create_test: create_testcases.cpp
	$(CC) $(CFLAGS) -o create_testcases create_testcases.cpp
	./create_testcases 20241202 0
	
create_tests_all: create_testcases.cpp
	$(CC) $(CFLAGS) -o create_testcases create_testcases.cpp
	./create_testcases 20241202 0
	./create_testcases 20241202 1
	./create_testcases 20241202 2

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
	rm -f *.txt
