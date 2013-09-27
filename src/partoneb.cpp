/*
 * CS 3242 Operating Systems
 * Fall 2013
 * Project 3:  Forks and Threads
 * Nick Champagne and Hue Moua
 * Date:  9/23/2013
 * File:  partoneb.cpp
 */

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void simulateBusyWork(char, pid_t);
pid_t performFork();
const int NUM_OF_CHILDREN = 4;

int main()
{
    performFork();
}

pid_t performFork()
{
    pid_t pid;

	// fork a process
	pid = fork();

	// determine if this process is the parent or the child
	if (pid < 0)
	{
		// error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	}
	else if (pid == 0)
	{
		// child process
		int i = 0;

		// Create 4 more child processes
		while (i < NUM_OF_CHILDREN && 0 == pid)
		{
			// When forking, the parent contains the child's PID, which is > 0, and exits the loop immediately;
			// 				 the child continues on to fork more children.
			pid = fork();

			i++;

		}

		// The parents created from the while-loop above do busy work on their children's behalf.
		if (pid > 0)
		{
			cout << "A child process with PID " << pid << " was created.\n";

			simulateBusyWork('C', pid);
		}
	}
	else
 	{
		// parent process

		// do parent's busy work.
		simulateBusyWork('P', pid);
	}
	return pid;
}

void simulateBusyWork(char ch, pid_t pid)
{
	srand(pid);
    for (int i = 0; i < 5; i++)
    {
    	int max = rand() % 9900000 + 100000;
        for (int j = 0; j < max; j++)
            ; // does nothing but waste time
        cout << ch << pid << ": " << i << endl;
    }
    cout << "Process " << ch << pid << " finished its work." << endl;
}
