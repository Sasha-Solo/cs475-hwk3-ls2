#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>



/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {

	stack_t *s2 = initstack(); //initialize stack
	int counter = 0;
	int counter2 = 0;

	if (argc == 1 || argc > 3){ //re-prompt if user does not type in more than the req args
		printf("Usage: ./ls2 <path> [optional-file]\n");
	}
	else if (argc == 2){ //if user wants to do Mode 1
		printf("Mode 1 for: %s\n", argv[1]);
		modeOne(argv[1], counter); //call modeOne
	}
	else if (argc == 3){ //if user wants to do Mode 2
		printf("Mode 2 for: %s %s\n", argv[1], argv[2]);
		modeTwo(argv[1], argv[2], s2, counter2); //call modeTwo
		printstack(s2);
	}

	freestack(s2);
	//stack stuff here

	// stack_t *s = initstack();

	// push(s, "Hello1");
	// push(s, "Hello2");
	// push(s, "Hello3");

	// // print stack
	// printstack(s);

	// // free up stack
	// freestack(s);


	return 0;
}
