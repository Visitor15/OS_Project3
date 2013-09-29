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
#include <unistd.h>
#include <mutex>

/*
 * Used to post a thread message update so another
 * thread cannot come along and post a message
 * midway of another message getting posted.
 */
std::mutex basic_mutex;

double calculatePi(long totalDarts, long dartsHit);
void prepareThreads();
void *performWork(void *complex_thread);

int m_numOfRunningThreads = 0;

/*
 * struct used to encapsulate a pthread along with its
 * specific data.
 */
struct ComplexThread {
	double m_runTime;
	long m_totalDarts;
	long m_dartsHit;
	long m_threadID;

	pthread_t m_thread;

	ComplexThread() {
		m_runTime = 0.0;
		m_totalDarts = 0;
		m_dartsHit = 0;
		m_threadID = -1;
		m_thread = NULL;
	}

	ComplexThread(const int numOfDarts) :
			m_totalDarts(numOfDarts), m_dartsHit(0), m_runTime(0.0) {
		m_thread = NULL;
		m_threadID = -1;
	}

	void createAndExecuteThread() {
		m_threadID = pthread_create(&m_thread, NULL, &performWork, this);

		if (m_threadID == 0) {
			++m_numOfRunningThreads;
			m_threadID = pthread_self();
		}
	}
};

void onThreadFinished(ComplexThread* thread);

int m_numOfThreads = 0;
long m_numOfDarts = 0;
long m_numOfDartsHit = 0;

const double AREA_OF_SQUARE = 2 * 2;
const double RADIUS = 1;

int main() {

	clock_t startTime = clock();

	prepareThreads();

	/* Sleeping the main thread for 1 second while background
	 * threads are still processing.
	 */
	while (m_numOfRunningThreads > 0) {
		sleep(1);
	}

	std::cout << "\nThreads finished, calculating PI..." << std::endl;

	double PI = calculatePi(m_numOfDarts, m_numOfDartsHit);

	double executionTime = double(startTime - clock()) / (double) CLOCKS_PER_SEC;

	std::cout << "===========================" << std::endl;
	std::cout << "RESULTS" << std::endl;
	std::cout << "===========================" << std::endl;
	std::cout << "Number of threads:\t" << m_numOfThreads << std::endl;
	std::cout << "Number of darts:\t" << m_numOfDarts << std::endl;
	std::cout << "Number of hits:\t\t" << m_numOfDartsHit << std::endl;
	std::cout << "Number of misses:\t" << (m_numOfDarts - m_numOfDartsHit)
			<< std::endl;
	std::cout << "Execution time:\t\t" << executionTime << std::endl;
	std::cout << "PI:\t\t\t" << PI << std::endl;

	return 0;
}

void prepareThreads() {
	std::cout << "Enter number of threads: ";
	std::cin >> m_numOfThreads;

	std::cout << "\nEnter number of darts to throw: ";
	std::cin >> m_numOfDarts;

	for (int i = 0; i < m_numOfThreads; i++) {
		ComplexThread* thread = new ComplexThread(
				m_numOfDarts / m_numOfThreads);

		thread->createAndExecuteThread();
	}
}

/*
 * Main execution thread used by a ComplexThread when pthread_create() is called.
 */
void *performWork(void* complex_thread) {
	ComplexThread* working_thread = static_cast<ComplexThread*>(complex_thread);

	/*
	 * Synchronizing the message output so we will not have the full message interrupted
	 * by another thread attempting to post a message at the same time.
	 */
	basic_mutex.lock();
	std::cout << "Thread: " << working_thread->m_thread
			<< " is performing work on " << working_thread->m_totalDarts
			<< " darts." << std::endl;
	basic_mutex.unlock();

	float pointX;
	float pointY;
	float calculatedPoint;

	for (long i = 0; i < working_thread->m_totalDarts; i++) {
		pointX = (float) rand() / RAND_MAX;
		pointY = (float) rand() / RAND_MAX;
		calculatedPoint = (pointX * pointX) + (pointY * pointY);

		if (calculatedPoint < (RADIUS * RADIUS)) {
			working_thread->m_dartsHit++;
		}
	}
	onThreadFinished(working_thread);
	return working_thread;
}

/*
 * Callback method used by a thread that has finished its execution.
 * Allows itself to join the main thread and pass its processed data to it.
 */
void onThreadFinished(ComplexThread* thread) {
	pthread_join(thread->m_thread, (void **) &thread);

	m_numOfDartsHit += thread->m_dartsHit;
	--m_numOfRunningThreads;

	/*
	 * Synchronizing the message output so we will not have the full message interrupted
	 * by another thread attempting to post a message at the same time.
	 */
	basic_mutex.lock();
	std::cout << "Thread: " << thread->m_thread << " finished!" << std::endl;
	basic_mutex.unlock();
}

double calculatePi(long totalDarts, long dartsHit) {
	double pi = 0.0;

	pi = ((4.0 * (double) dartsHit) / (double) totalDarts);

	return pi;
}
