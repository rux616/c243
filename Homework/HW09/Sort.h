/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Sort.h
Class:          C-243 TuTh 1700
Action:			Contains sorting-related function prototypes.
*/

#pragma once

//Struct to hold the timing data of the test sorting.
struct timingSummary
{
    double best;
    double worst;
    double average;
    double total;
};

//Applies a small testing framework for testing the desired sort,
//in this case the dual pivot quicksort.
void testSort(int rawArray[], int sortedArray[], unsigned long size,
              unsigned tests, timingSummary &timing);

//Dual pivot quicksort by Vladimir Yaroslavskiy.
void dualPivotQuickSort(int a[], int left, int right, int div);



//Struct used to pass function arguments into threaded functions.
struct threadData
{
    int *array;
    int first;
    int last;
    int level;
    int *auxArray;
};

//Parallel quicksort functions, helpers and serial versions.
void parallelQuickSort(int a[], int first, int last);
void *parallelQuickSortHelper(void *threadArguments);
void quickSort(int a[], int first, int last);

//Parallel merge sort functions, helpers, and serial versions.
void parallelMergeSort(int a[], int first, int last, int aux[]);
void *parallelMergeSortHelper(void *threadArguments);
void mergeSort(int a[], int first, int last, int aux[]);
void mergeArrays(int a[], int afirst, int alast,
                 int b[], int bfirst, int blast,
                 int c[], int cfirst, int clast);

//Standard linear insertion sort.
void linearInsertionSort(int a[], int size);
