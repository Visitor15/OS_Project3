//============================================================================
// Name        : Project3_A.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <stdexcept>

pid_t processList[5];

static std::vector<pid_t> processIDList;

int num_of_forks = 4;

void simulateBusyWork(char);
pid_t performFork();

int main(void) {

	processIDList.push_back(getpid());




	if(num_of_forks > -1 && num_of_forks < 4) {
		performFork();
	}

	return EXIT_SUCCESS;
}

pid_t performFork() {
	pid_t pid;

	// Forking
	pid = fork();

	if (pid < 0) {
		// Error
		std::fprintf(stderr, "Fork Failed.\n");

		return -1;
	} else if (pid == 0) {
		// Child process
		num_of_forks = -1;

		std::cout << "A child process with PID: " << getpid() << " was created."
				<< std::endl;

		processIDList.push_back(getpid());

		simulateBusyWork('C');
	} else {
		simulateBusyWork('P');

		--num_of_forks;
		
		// This is just for testing

		waitpid(getpid(), NULL, 0);

	}

	return pid;
}

void simulateBusyWork(char ch) {
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 10000000; j++) {
			; // Just wasting time

		}

		std::cout << ch << getpid() << ": " << i << std::endl;
	}

	std::cout << "Process " << ch << getpid() << " finished its work."
			<< std::endl;
}
