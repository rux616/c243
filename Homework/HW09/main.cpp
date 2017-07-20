/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	main.cpp
Class:          C-243 TuTh 1700
Action:			Contains main function for this project.
*/

#include <cstdlib>  //for srand()
#include <ctime>    //for time()
#include <iostream>

#include "Interface.h"
#include "Sort.h"
#include "Utilities.h"

//Define the number of tests to run.
#define NUMBER_OF_TESTS 10

using namespace std;

int main()
{
    int *rawArray = NULL, *sortedArray = NULL;
    unsigned long size = 0;
    struct timingSummary timing;

    //Get the size and elements of rawArray from user input.
    buildArray(rawArray, size);

    //Allocate a new array of same size as rawArray and copy rawArray into it.
    //This preserves rawArray as a master copy of original input.
    sortedArray = new (nothrow) int[size];
    testAllocation(sortedArray);
    for (unsigned i = 0; i < size; ++i)
        sortedArray[i] = rawArray[i];

    //Use the dual pivot quicksort to sort the array.
    testSort(rawArray, sortedArray, size, NUMBER_OF_TESTS, timing);

    //Print the sorted array.
    cout << "\nThe sorted array:\n";
    printArray(sortedArray, size);

    //Print the timing data.
    cout << "\nTiming statistics for " << NUMBER_OF_TESTS << " tests:\n";
    printTiming(timing);
    
    return 0;
} //main
