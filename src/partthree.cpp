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

std::vector<long> masterList;

int m_numOfRunningThreads = 0;

std::string printENUM(SORT_TYPE type);

void doQuickSort(std::vector<long> &unsortedList, long begin, long end);
void doSelectionSort(std::vector<long> &unsortedList, long begin, long end);
void doMergeSort(std::vector<long> &unsortedList);
void doBubbleSort(std::vector<long> &unsortedList);
void doShellSort(std::vector<long> &unsortedList);
void doInsertionSort(std::vector<long> &unsortedList);

void swap(long &x, long &y);

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

	ComplexThread(const std::vector<long> list) :
			unsorted_list(list) {
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

std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads);

void trial1();
void trial2();

int main() {
//	partitionMasterListForArbitraryNumOfThreads();

	trial1();

	return 0;
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

	/*
	 * Sanity check to ensure we actually have a useful number of theads.
	 * We'll purposely ask the user if we initially do not.
	 */
	if (numOfThreads == NULL || numOfThreads < 1) {
		std::cout << "Number of threads: ";
		std::cin >> numOfThreads;
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

/*
 * Trial 1 is only designed for 2 threads.
 */
void trial1() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(2);

	for (int i = 0; i < threadList.size(); i++) {
		std::cout << "THREAD DATA COUNT: "
				<< threadList.at(i)->unsorted_list.size() << std::endl;

		if (i == 0) {
			// Using QUICK SORT
			threadList.at(i)->m_sort_type = QUICK_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;

			std::cout << "First item is: "
					<< threadList.at(i)->unsorted_list.at(0) << std::endl;

			doQuickSort(threadList.at(i)->unsorted_list, 0L,
					(long) threadList.at(i)->unsorted_list.size() - 1);

			std::cout << "\nNew first item is: "
					<< threadList.at(i)->unsorted_list.at(0) << std::endl;

			std::vector<long> tempList = threadList.at(i)->unsorted_list;

			for(int k = 0; k < tempList.size(); k++) {
				std::cout << tempList.at(k) << "\n";
			}

		}
		if (i == 1) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = SELECTION_SORT;

			std::cout << "SORT TYPE SET TO: "
					<< printENUM(threadList.at(i)->m_sort_type) << std::endl;

			std::cout << "First item is: "
					  << threadList.at(i)->unsorted_list.at(0) << std::endl;

			doSelectionSort(threadList.at(i)->unsorted_list, 0L,
					(long) threadList.at(i)->unsorted_list.size() - 1);

			std::cout << "\nNew first item is: "
					<< threadList.at(i)->unsorted_list.at(0) << std::endl;

			std::vector<long> tempList = threadList.at(i)->unsorted_list;

			for(int k = 0; k < tempList.size(); k++) {
				std::cout << tempList.at(k) << "\n";
			}
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
		std::cout << "THREAD DATA COUNT: "
				<< threadList.at(i)->unsorted_list.size() << std::endl;

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

//	std::cout << "Pivot pos: " << pivotVal << ", ";

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
		if (j % 50 == 0) {
			std::cout << "\n";
		}
		std::cout << ".";
		doQuickSort(unsortedList, leftMost, j);
	}
	if (i < rightMost) {
		if (i % 50 == 0) {
			std::cout << "\n";
		}
		std::cout << ".";
		doQuickSort(unsortedList, i, rightMost);
	}

//	std::cout << "FINISHED! " << std::endl;

//	for (int p = 0; p < unsortedList.size(); p++) {
//		if (p < 25) {
//			std::cout << unsortedList.at(p) << std::endl;
//		}
//	}
}

void doSelectionSort(std::vector<long> &unsortedList, long leftMost,
		long rightMost) {

	long i, j;
	long iMin;
	long n = rightMost;

	for (j = leftMost; j < n-1; j++) {
	    /* find the min element in the unsorted a[j .. n-1] */

	    /* assume the min is the first element */
	    iMin = j;
	    /* test against elements after j to find the smallest */
	    for ( i = j+1; i < n; i++) {
	        /* if this element is less, then it is the new minimum */
	        if (unsortedList.at(i) < unsortedList.at(iMin)) {
	            /* found new minimum; remember its index */
	            iMin = i;
	        }
	    }

	    /* iMin is the index of the minimum element. Swap it with the current position */
	    if ( iMin != j ) {
	        swap(unsortedList.at(j), unsortedList.at(iMin));
	    }

	    std::cout << j << "\t" << unsortedList.at(j) << std::endl;
	}
}

void swap(long &x, long &y)
{
	long temp = x;
	x = y;
	y = temp;
}

void doMergeSort(std::vector<long> &unsortedList) {

}

void doBubbleSort(std::vector<long> &unsortedList) {

}

void doShellSort(std::vector<long> &unsortedList) {

}

void doInsertionSort(std::vector<long> &unsortedList) {

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
