/*
 * CS 3242 Operating Systems
 * Fall 2013
 * Project 3:  Forks and Threads
 * Nick Champagne and Hue Moua
 * Date:  9/23/2013
 * File:  parttwo.cpp
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

void prepareThreads();
void *performWork(void *complex_thread);

struct ComplexThread {
	double m_runTime;
	int m_totalDarts;
	int m_dartsHit;
	int m_threadID;

	pthread_t m_thread;

	ComplexThread() {
		m_runTime = 0.0;
		m_totalDarts = 0;
		m_dartsHit = 0;
		m_threadID = -1;
		m_thread = NULL;
	}

	ComplexThread(const int numOfDarts) : m_totalDarts(numOfDarts), m_dartsHit(0), m_runTime(0.0) {
		m_thread = NULL;
		m_threadID = -1;
	}

	void createAndExecuteThread() {
		m_threadID = pthread_create(&m_thread, NULL, &performWork, this);
	}
};

void onThreadFinished(ComplexThread* thread);

// Variable declaration here
int m_numOfThreads = 0;
int m_numOfDarts = 0;

std::vector<pthread_t> threadList;

int main() {

	prepareThreads();


	return 0;
}

void prepareThreads() {
	std::cout << "Enter number of threads: ";
	std::cin >> m_numOfThreads;

	std::cout << "\nEnter number of darts to throw: ";
	std::cin >> m_numOfDarts;

	std::cout << "NUM OF THREADS: " << m_numOfThreads << " AND NUM OF DARTS: " << m_numOfDarts << "\n";

	for(int i = 0; i < m_numOfThreads; i++) {
		std::cout << "Creating new thread: " << i << "\n";
		ComplexThread* thread = new ComplexThread(m_numOfDarts/4);
		thread->createAndExecuteThread();
	}
}

void *performWork(void* complex_thread)  {
	ComplexThread* working_thread = static_cast<ComplexThread*>(complex_thread);

	std::cout << "Thread: " << working_thread->m_thread << " is performing work on " << working_thread->m_totalDarts << " darts.\n";

	for(int i = 0; i < 25; i++) {
		std::cout << "\tT-" << working_thread->m_thread << " : " << i << "\n";
	}

	onThreadFinished(working_thread);

	return working_thread;
}

void onThreadFinished(ComplexThread* thread) {
	ComplexThread tmp_thread;

	pthread_exit((void*) thread);
	pthread_join(thread->m_thread, (void **)&tmp_thread);

	std::cout << "Thread: " << thread->m_thread << " finished!\n";
}

//void performWork(std::vector<pthread_t>* list) {
//
//}
