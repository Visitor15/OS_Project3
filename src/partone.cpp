/*
 * CS 3242 Operating Systems
 * Fall 2013
 * Project 3:  Forks and Threads
 * Nick Champagne and Hue Moua
 * Date:  9/23/2013
 * File:  partone.cpp
 */

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;
void simulateBusyWork(char);
pid_t performFork();
int main()
{
    performFork();
}
pid_t performFork()
{
    pid_t pid;
    // fork a process
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
        simulateBusyWork('C');
    }
    else
    {
        // parent process
    }
    return pid;
}
void simulateBusyWork(char ch)
{
    int i, j;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 1000000; j++)
            ; // does nothing but waste time
        cout << i << endl;
    }
    cout << "Process finished its work." << endl;
}
