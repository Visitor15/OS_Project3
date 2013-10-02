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
#include <iterator>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdexcept>

enum SORT_TYPE {
	QUICK_SORT = 0,
	QUICK_SORT3 = 1,
	SELECTION_SORT = 2,
	MERGE_SORT = 3,
	BUBBLE_SORT = 4,
	SHELL_SORT = 5,
	INSERTION_SORT = 6,
	HEAP_SORT = 7,
	BUCKET_SORT = 8,
	COMB_SORT = 9
};

std::vector<long> masterUnsortedList;
std::vector<long> masterSortedList;

int m_numOfRunningThreads = 0;

std::string printENUM(SORT_TYPE type);

void doQuickSort(std::vector<long> &unsortedList, long begin, long end);
void doSelectionSort(std::vector<long> &unsortedList);
void doMergeSort(std::vector<long> &unsortedList);
void doBubbleSort(std::vector<long> &unsortedList);
void doShellSort(std::vector<long> &unsortedList);
void doInsertionSort(std::vector<long> &unsortedList);
void writeContentsToFile(std::vector<long> &values, std::string fileName);
std::vector<long> mergeSortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList);

void *performSortOnThread(void *complex_thread);

struct ComplexThread {
	long m_threadID;
	pthread_t m_thread;
	SORT_TYPE m_sort_type;
	std::vector<long> data_list;

	ComplexThread() {
		m_threadID = -1;
		m_thread = NULL;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list) :
			data_list(list) {
		m_thread = NULL;
		m_threadID = -1;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list, SORT_TYPE sort_type) :
			data_list(list), m_sort_type(sort_type) {
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

void onThreadFinished(ComplexThread* thread);
std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads);

void trial1();
void trial2();

int main() {

	trial1();

	/* Sleeping the main thread for 1 second while background
	 * threads are still processing.
	 */
	while (m_numOfRunningThreads > 0) {
		sleep(1);
	}

	std::cout << "All threads finished!" << std::endl;
//	for(int i = 0; i < masterSortedList.size(); i++) {
//		std::cout << masterSortedList.at(i) << std::endl;
//	}

	writeContentsToFile(masterSortedList, "testFile.txt");

	return 0;
}

std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads) {
	std::vector<ComplexThread*> threadList;

	std::ifstream inStream("numbers.txt");
	std::string m_line;
	while (std::getline(inStream, m_line)) {

		//Converting from a string to a long.
		long number = strtol((char*) m_line.c_str(), NULL, 0);

		masterUnsortedList.push_back(number);
	}

	/*
	 * Sanity check to ensure we actually have a useful number of theads.
	 * We'll purposely ask the user if we initially do not.
	 */
	if (numOfThreads == NULL || numOfThreads < 1) {
		std::cout << "Number of threads: ";
		std::cin >> numOfThreads;
	}

	int range = masterUnsortedList.size() / numOfThreads;
	long offset = 0;
	long lastIndex = range;
	for (int i = 0; i < numOfThreads; i++) {
		std::vector<long> partitionedData;
		std::vector<long>::iterator it;

		for (int j = offset; j < lastIndex; j++) {
			partitionedData.push_back(masterUnsortedList.at(j));
		}

		offset += range;
		lastIndex += range;

		ComplexThread* thread = new ComplexThread(partitionedData);
		std::cout << "Partitioned data size is: " << thread->data_list.size()
				<< std::endl;

		threadList.push_back(thread);
	}

	inStream.close();

	return threadList;
}

/*
 * Main execution thread used by a ComplexThread when pthread_create() is called.
 */
void *performSortOnThread(void* complex_thread) {
	ComplexThread* working_thread = static_cast<ComplexThread*>(complex_thread);

	switch (working_thread->m_sort_type) {
	case QUICK_SORT: {
		std::cout << "QuickSort" << std::endl;
		doQuickSort(working_thread->data_list, 0,
				(working_thread->data_list.size() - 1));
		break;
	}
	case SELECTION_SORT: {
		std::cout << "SelectionSort" << std::endl;
		doSelectionSort(working_thread->data_list);
		break;
	}
	case MERGE_SORT: {
		std::cout << "MergeSort" << std::endl;
		break;
	}
	case BUBBLE_SORT: {
		std::cout << "BubbleSort" << std::endl;
		break;
	}
	case SHELL_SORT: {
		std::cout << "ShellSort" << std::endl;
		break;
	}
	case INSERTION_SORT: {
		std::cout << "InsertionSort" << std::endl;
		break;
	}
	}

	onThreadFinished(working_thread);
	return working_thread;
}

void onThreadFinished(ComplexThread* thread) {
	pthread_join(thread->m_thread, (void **) &thread);

	masterSortedList = mergeSortedLists(thread->data_list, masterSortedList);

	--m_numOfRunningThreads;

	/*
	 * Synchronizing the message output so we will not have the full message interrupted
	 * by another thread attempting to post a message at the same time.
	 */
//	basic_mutex.lock();
//	std::cout << "Thread: " << thread->m_thread << " finished!" << std::endl;
//	basic_mutex.unlock();
}

/*
 * Trial 1 is only designed for 2 threads.
 */
void trial1() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(2);

	for (int i = 0; i < threadList.size(); i++) {
//		std::cout << "THREAD DATA COUNT: " << threadList.at(i)->data_list.size()
//				<< std::endl;

		if (i == 0) {
			// Using QUICK SORT
			threadList.at(i)->m_sort_type = QUICK_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;

			threadList.at(i)->createAndExecuteThread();

		}
		if (i == 1) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = SELECTION_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;

			threadList.at(i)->createAndExecuteThread();
		}
	}
}

/*
 * Trial 2 is only designed for 4 threads.
 */
void trial2() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(4);

	for (int i = 0; i < threadList.size(); i++) {
		std::cout << "THREAD DATA COUNT: " << threadList.at(i)->data_list.size()
				<< std::endl;

		if (i == 0) {
			// Using QUICK SORT
			threadList.at(i)->m_sort_type = MERGE_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;
		}
		if (i == 1) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = BUBBLE_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;
		}

		if (i == 2) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = SHELL_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;
		}

		if (i == 3) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = INSERTION_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;
		}
	}
}

void doQuickSort(std::vector<long> &unsortedList, long leftMost,
		long rightMost) {

	long tempVal;
	long i = leftMost;
	long j = rightMost;
	long pivotVal = unsortedList.at((leftMost + rightMost) / 2);

	while (i <= j) {
		while (unsortedList.at(i) < pivotVal)
			i++;
		while (unsortedList.at(j) > pivotVal)
			j--;
		if (i <= j) {
			tempVal = unsortedList.at(i);
			unsortedList.at(i) = unsortedList.at(j);
			unsortedList.at(j) = tempVal;
			i++;
			j--;
		}
	}

	if (leftMost < j) {
		doQuickSort(unsortedList, leftMost, j);
	}
	if (i < rightMost) {
		doQuickSort(unsortedList, i, rightMost);
	}
}

void doSelectionSort(std::vector<long> &unsortedList) {
	long minIndex;
	long tempVal;
	long i;
	long j;

	for (i = 0; i < unsortedList.size() - 1; i++) {
		minIndex = i;

		for (j = i + 1; j < unsortedList.size(); j++) {

			if (unsortedList.at(j) < unsortedList.at(minIndex)) {
				minIndex = j;
			}
		}

		if (minIndex != i) {
			tempVal = unsortedList.at(i);
			unsortedList.at(i) = unsortedList.at(minIndex);
			unsortedList.at(minIndex) = tempVal;
		}
	}
}

void doMergeSort(std::vector<long> &unsortedList) {

}

void doBubbleSort(std::vector<long> &unsortedList) {

}

void doShellSort(std::vector<long> &unsortedList) {

}

void doInsertionSort(std::vector<long> &unsortedList) {

}

std::vector<long> mergeSortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList) {
	long firstListSize = firstList.size();
	long secondListSize = secondList.size();

	long totalSize = firstListSize + secondListSize;

	std::vector<long> finalSortedList(totalSize);

	long i = 0;
	long j = 0;
	long k = 0;

	while (i < firstListSize && j < secondListSize) {
		if (firstList.at(i) <= secondList.at(j)) {
			finalSortedList.at(k) = firstList.at(i);
			i++;
		} else {
			finalSortedList.at(k) = secondList.at(j);
			j++;
		}
		k++;
	}
	if (i < firstListSize) {
		for (int p = i; p < firstListSize; p++) {
			finalSortedList.at(k) = firstList.at(p);
			k++;
		}
	} else {
		for (int p = j; p < secondListSize; p++) {
			finalSortedList.at(k) = secondList.at(p);
			k++;
		}
	}

	return finalSortedList;
}

void writeContentsToFile(std::vector<long> &values, std::string fileName) {
	std::cout << "Writing contents to file: " << fileName << std::endl;

	std::ofstream outFile((char*) fileName.c_str());
	std::ostream_iterator<long> out_iterator(outFile, "\n");
	std::copy(values.begin(), values.end(), out_iterator);

	outFile.close();

	std::cout << "Data written to " << fileName << std::endl;
}

/*
 * Simply for testing purposes.
 */
std::string printENUM(SORT_TYPE type) {
	switch (type) {
	case QUICK_SORT: {
		return "QuickSort";
		break;
	}
	case SELECTION_SORT: {
		return "SelectionSort";
		break;
	}
	case MERGE_SORT: {
		return "MergeSort";
		break;
	}
	case BUBBLE_SORT: {
		return "BubbleSort";
		break;
	}
	case SHELL_SORT: {
		return "ShellSort";
		break;
	}
	case INSERTION_SORT: {
		return "InsertionSort";
		break;
	}
	}
}
