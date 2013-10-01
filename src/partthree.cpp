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

enum SORT_TYPE {
	QUICK_SORT,
	QUICK_SORT3,
	SELECTION_SORT,
	MERGE_SORT,
	BUBBLE_SORT,
	SHELL_SORT,
	INSERTION_SORT,
	HEAP_SORT,
	BUCKET_SORT,
	COMB_SORT
};

std::vector<long> masterList;

int m_numOfRunningThreads = 0;

void *performSortOnThread(void *complex_thread);

struct ComplexThread {
	long m_threadID;
	pthread_t m_thread;
	SORT_TYPE m_sort_type;
	std::vector<long> unsorted_list;

	ComplexThread() {
		m_threadID = -1;
		m_thread = NULL;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list) : unsorted_list(list) {
		m_thread = NULL;
		m_threadID = -1;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list, SORT_TYPE sort_type) :
			unsorted_list(list), m_sort_type(sort_type) {
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

void partitionMasterListForArbitraryNumOfThreads();
std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads);

void trial1();
void trial2();

int main() {
//	partitionMasterListForArbitraryNumOfThreads();

	trial1();

	return 0;
}

/*
 * Method used to evenly distribute elements from the masterList to
 * an arbitrary number of threads.
 */
void partitionMasterListForArbitraryNumOfThreads() {
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
		std::cout << "Partitioned data size is: "
				<< thread->unsorted_list.size() << std::endl;
	}
}

std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads) {
	std::vector<ComplexThread*> threadList;

	std::ifstream inStream("numbers.txt");
	std::string m_line;
	std::stringstream stringStream;
	while (std::getline(inStream, m_line)) {

		//Converting from a string to a long.
		long number = strtol((char*) m_line.c_str(), NULL, 0);

		masterList.push_back(number);
	}

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
		std::cout << "Partitioned data size is: "
				<< thread->unsorted_list.size() << std::endl;

		threadList.push_back(thread);
	}

	return threadList;
}

void trial1() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(5);

	for (int i = 0; i < threadList.size(); i++) {
		std::cout << "THREAD DATA COUNT: "
				<< threadList.at(i)->unsorted_list.size() << std::endl;
	}

}

void trial2() {

}
