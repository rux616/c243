/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Sort.cpp
Class:          C-243 TuTh 1700
Action:			Contains sorting-related functions.
*/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>

#include "Sort.h"
#include "Utilities.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define WINDOWS
    #include "GetTimeOfDay.h" //Windows implementation of the gettimeofday() function.
#else
    #include <sys/time.h>
#endif

//Arrays with this number of elements or less will use a linear insertion sort.
#define LIS_THRESHOLD 500

using namespace std;

/****************************** testSort ***********************************************************
Action:         Applies a small testing framework for testing the desired sort, in this case the
                dual pivot quicksort.

Parameters:
IN:             int rawArray[], holds pointer to array with the raw data to sort.
IN:             int sortedArray[], holds pointer to array that will be sorted.
IN:             unsigned long size, holds the number of elements of the arrays.
IN:             unsigned tests, holds the number of tests to be run on the data.
OUT:            int sortedArray[], holds pointer to the array with the sorted data.
OUT:            timingSummary &timing, holds the timing summary of the tests.

Returns:        Nothing

Precondition:   Arrays should be allocated, and size should be the number of elements in each.
                The number of tests desired should be more than zero.
***************************************************************************************************/
void testSort(int rawArray[], int sortedArray[], unsigned long size,
              unsigned tests, timingSummary &timing)
{
    //Declare necessary variables and check the allocation as needed.
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    //Start the testing loop.
    cout << "\nDual Pivot Quick Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //Reset the sorted array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            sortedArray[element] = rawArray[element];

        //Get the current time.
        gettimeofday(&before, 0);

        //Sort that array!
        dualPivotQuickSort(sortedArray, 0, size - 1, 3);

        //Get the elapsed time.
        gettimeofday(&after, 0);
        
        //Store the elapsed time after mathing it.
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    //Set the default values of best, worst, and average.
    timing.best = timing.worst = timing.average = rawTiming[0];

    //Run through the array of the timings and figure out which one was the best and
    //which one was the worst.
    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    //Get the total time and the average time.
    timing.total = timing.average;
    timing.average /= tests;
} //testSort

/****************************** dualPivotQuickSort *************************************************
Action:         Dual pivot quicksort by Vladimir Yaroslavskiy.

Parameters:
IN:             int a[], holds pointer to the array of data to be sorted.
IN:             int left, holds the left most element number this instance will work on.
IN:             int right, holds the right most element number this instance will work on.
IN:             int div, holds the number of sections the array will be divided into.
OUT:            int a[], holds pointer to the sorted array.

Returns:        Nothing

Precondition:   <a> should be an allocated array, and left and right should be elements within that
                array.
***************************************************************************************************/
void dualPivotQuickSort(int a[], int left, int right, int div)
{
    int len = right - left;

    if (len < LIS_THRESHOLD)
    { // insertion sort for small arrays
        for (int i = left + 1; i <= right; i++)
            for (int j = i; j > left && a[j] < a[j - 1]; j--)
                swap(a, j, j - 1);
        return;
    }
    int third = len / div;

    // "medians"
    int m1 = left + third;
    int m2 = right - third;

    if (m1 <= left)
        m1 = left + 1;
    if (m2 >= right)
        m2 = right - 1;
    if (a[m1] < a[m2])
    {
        swap(a, m1, left);
        swap(a, m2, right);
    }
    else
    {
        swap(a, m1, right);
        swap(a, m2, left);
    }

    // pivots
    int pivot1 = a[left];
    int pivot2 = a[right];

    // pointers
    int less = left + 1;
    int great = right - 1;

    // sorting
    for (int k = less; k <= great; k++)
    {
        if (a[k] < pivot1)
            swap(a, k, less++);
        else if (a[k] > pivot2)
        {
            while (k < great && a[great] > pivot2)
                great--;

            swap(a, k, great--);

            if (a[k] < pivot1)
                swap(a, k, less++);
        }
    }
    // swaps
    int dist = great - less;

    if (dist < 13)
        div++;
    swap(a, less - 1, left);
    swap(a, great + 1, right);

    // subarrays
    dualPivotQuickSort(a, left, less - 2, div);
    dualPivotQuickSort(a, great + 2, right, div);

    // equal elements
    if (dist > len - 13 && pivot1 != pivot2)
    {
        for (int k = less; k <= great; k++)
        {
            if (a[k] == pivot1)
                swap(a, k, less++);
            else if (a[k] == pivot2)
            {
                swap(a, k, great--);

                if (a[k] == pivot1)
                    swap(a, k, less++);
            }
        }
    }
    // subarray
    if (pivot1 < pivot2)
        dualPivotQuickSort(a, less, great, div);
} //dualPivotQuickSort



//**************************************************************************************************
// Code below this point is not actually used in the program, but is included as bonus content.
// Basically, Christine and I are damn proud that we actually got this working and want to show
// the code off like a couple of little kids. :)
//**************************************************************************************************
#ifndef WINDOWS
    #include <pthread.h>
#endif

//The parallel functions will create 2^PARALLEL_THREADS threads.
#define PARALLEL_THREADS 1

void parallelQuickSort(int a[], int first, int last)
{
    //Makes sure this compiles under Windows.
    #ifndef WINDOWS
    {
        pthread_t thread;
        void *status;

        threadData td;
        td.array = a;
        td.first = first;
        td.last = last;
        td.level = PARALLEL_THREADS;
        td.auxArray = NULL;

        pthread_create(&thread, NULL, parallelQuickSortHelper, (void *)&td);
        pthread_join(thread, &status);
    }
    #else
        return;
    #endif
} //parallelQuickSort

void *parallelQuickSortHelper(void *threadArguments)
{
    //Makes sure this compiles under Windows.
    #ifndef WINDOWS
    {
        struct threadData *myData;
        myData = (struct threadData *) threadArguments;

        if (myData->last - myData->first < LIS_THRESHOLD)
        {
            linearInsertionSort(&myData->array[myData->first], myData->last - myData->first + 1);
            pthread_exit(NULL);
        }

        if (!myData->level)
        {
            quickSort(myData->array, myData->first, myData->last);
            pthread_exit(NULL);
        }

        swap(myData->array, myData->first, (myData->first + myData->last) / 2);
        int pivot = myData->array[myData->first];
        int i = myData->first + 1, j = myData->last;

        while (i < j)
        {
            while (myData->array[i] < pivot && i < j)
                ++i;
            while (myData->array[j] > pivot)
                --j;
            if (i < j)
                swap(myData->array, i++, j--);
        }

        if (myData->array[j] > pivot) j--;

        swap(myData->array, j, myData->first);

        threadData td1, td2;
        td1.array = myData->array;
        td1.first = myData->first;
        td1.last = j - 1;
        td1.level = myData->level - 1;
        td1.auxArray = NULL;

        td2.array = myData->array;
        td2.first = j + 1;
        td2.last = myData->last;
        td2.level = myData->level - 1;
        td2.auxArray = NULL;

        pthread_t thread1, thread2;
        void *status;

        pthread_create(&thread1, NULL, parallelQuickSortHelper, (void *)&td1);
        pthread_create(&thread2, NULL, parallelQuickSortHelper, (void *)&td2);
        pthread_join(thread1, &status);
        pthread_join(thread2, &status);

        pthread_exit(NULL);
    }
    #else
        return NULL;
    #endif
} //parallelQuickSortHelper

void quickSort(int a[], int first, int last)
{
    if (last - first < LIS_THRESHOLD)
    {
        linearInsertionSort(&a[first], last - first + 1);
        return;
    }

    swap(a, first, (first + last) / 2);
    int pivot = a[first];

    int i = first + 1, j = last;

    while (i < j)
    {
        while (a[i] < pivot && i < j)
            ++i;
        while (a[j] > pivot)
            --j;
        if (i < j)
            swap(a, i++, j--);
    }

    if (a[j] > pivot) j--;

    swap(a, j, first);

    quickSort(a, first, j - 1);
    quickSort(a, j + 1, last);
} //quickSort

void parallelMergeSort(int a[], int first, int last, int aux[])
{
    //Makes sure this compiles under Windows.
    #ifndef WINDOWS
    {
        threadData td;
        td.array = a;
        td.first = first;
        td.last = last;
        td.level = PARALLEL_THREADS;
        td.auxArray = aux;

        pthread_t thread;

        pthread_create(&thread, NULL, parallelMergeSortHelper, (void *)&td);
        pthread_join(thread, NULL);
    }
    #else
        return;
    #endif
} //parallelMergeSort

void *parallelMergeSortHelper(void *threadArguments)
{
    //Makes sure this compiles under Windows.
    #ifndef WINDOWS
    {
        struct threadData *args;
        args = (struct threadData *) threadArguments;

        if (args->last - args->first < LIS_THRESHOLD)
        {
            linearInsertionSort(&args->array[args->first], args->last - args->first + 1);
            pthread_exit(NULL);
        }

        if (!args->level)
        {
            mergeSort(args->array, args->first, args->last, args->auxArray);
            pthread_exit(NULL);
        }

        int mid = (args->first + args->last) / 2;

        threadData td1, td2;
        td1.array = args->array;
        td1.first = args->first;
        td1.last = mid;
        td1.level = args->level - 1;
        td1.auxArray = args->auxArray;

        td2.array = args->array;
        td2.first = mid + 1;
        td2.last = args->last;
        td2.level = args->level - 1;
        td2.auxArray = args->auxArray;

        pthread_t thread1, thread2;

        pthread_create(&thread1, NULL, parallelMergeSortHelper, (void *)&td1);
        pthread_create(&thread2, NULL, parallelMergeSortHelper, (void *)&td2);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        mergeArrays(args->array, args->first, mid,
                    args->array, mid + 1, args->last,
                    args->auxArray, args->first, args->last);

        for (int i = args->first; i <= args->last; ++i)
            args->array[i] = args->auxArray[i];

        pthread_exit(NULL);
    }
    #else
        return NULL;
    #endif
} //parallelMergeSortHelper

void mergeSort(int a[], int first, int last, int aux[])
{
    if (last - first < LIS_THRESHOLD)
    {
        linearInsertionSort(&a[first], last - first + 1);
        return;
    }

    int mid = (first + last) / 2;

    mergeSort(a, first, mid, aux);
    mergeSort(a, mid + 1, last, aux);
    mergeArrays(a, first, mid, a, mid + 1, last, aux, first, last);

    for (int i = first; i <= last; ++i)
        a[i] = aux[i];
} //mergeSort

void mergeArrays(int a[], int afirst, int alast,
                 int b[], int bfirst, int blast,
                 int c[], int cfirst, int clast)
{
    int i = afirst, j = bfirst, k = cfirst;
    while (i <= alast && j <= blast)
    {
        if (a[i] <= b[j])
            c[k++] = a[i++];
        else
            c[k++] = b[j++];
    }

    while (i <= alast)
        c[k++] = a[i++];

    while (j <= blast)
        c[k++] = b[j++];
} //mergeArrays

void linearInsertionSort(int a[], int size)
{
    for (int p = 1; p<size; ++p)
    {
        int i = p - 1;
        while (i >= 0 && a[i] > a[i + 1])
        {
            swap(a, i, i + 1);
            i--;
        }
    }
} //linearInsertionSort
