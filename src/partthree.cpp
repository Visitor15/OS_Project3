/*
 * CS 3242 Operating Systems
 * Fall 2013
 * Project 3:  Forks and Threads
 * Nick Champagne and Hue Moua
 * Date:  9/23/2013
 * File:  partthree.cpp
 */

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

void partitionMasterList();
void *performSortOnThread(void *complex_thread);

std::vector<long> masterList;

int m_numOfRunningThreads = 0;

struct ComplexThread {
	long m_threadID;
	pthread_t m_thread;
	std::vector<long> unsorted_list;

	ComplexThread() {
		m_threadID = -1;
		m_thread = NULL;
	}

	ComplexThread(const std::vector<long> list) :
			unsorted_list(list) {
		m_thread = NULL;
		m_threadID = -1;
	}

	void createAndExecuteThread() {
		m_threadID = pthread_create(&m_thread, NULL, &performSortOnThread,
				this);

		if (m_threadID == 0) {
			++m_numOfRunningThreads;
			m_threadID = pthread_self();
		}
	}
};

int main() {
	partitionMasterList();

	return 0;
}

/*
 * Method used to evenly distribute elements from the masterList to
 * an arbitrary number of threads.
 */
void partitionMasterList() {
	std::ifstream inStream("numbers.txt");
	std::string m_line;
	std::stringstream stringStream;
	while (std::getline(inStream, m_line)) {

		//Converting from a string to a long.
		long number = strtol((char*) m_line.c_str(), NULL, 0);

		masterList.push_back(number);
	}

	int numOfThreads;
	std::cout << "Number of threads: ";
	std::cin >> numOfThreads;

	int range = masterList.size() / numOfThreads;
	long offset = 0;
	long lastIndex = range;
	for (int i = 0; i < numOfThreads; i++) {
		std::vector<long> partitionedData;
		std::vector<long>::iterator it;

		for (int j = offset; j < lastIndex; j++) {
			partitionedData.push_back(masterList.at(j));
		}

		offset += range;
		lastIndex += range;

		ComplexThread* thread = new ComplexThread(partitionedData);

		/*
		 * Only for testing purposes
		 */
//		if (i == 0) {
//			for (int p = 0; p < thread->unsorted_list.size(); p++) {
//				if (p < 25) {
//					std::cout << "DATA IS: " << thread->unsorted_list.at(p)
//							<< std::endl;
//				}
//			}
//		}
		std::cout << "Partitioned data size is: "
				<< thread->unsorted_list.size() << std::endl;
	}
}
