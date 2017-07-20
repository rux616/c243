#pragma once

//
struct timingSummary
{
    double best;
    double worst;
    double average;
    double total;
};

//
struct threadData
{
    int *array;
    int first;
    int last;
    int level;
    int *auxArray;
};



void metaSort(int a[], unsigned int size);

void basicQuickSort(int a[], int first, int last);
void quickSort(int a[], int first, int last);

void parallelQuickSort(int a[], int first, int last);
void *parallelQuickSortHelper(void *threadArguments);

void dualPivotQuickSort(int a[], int left, int right, int div);

void mergeSort(int a[], int first, int last, int aux[]);
void mergeArrays(int a[], int afirst, int alast,
                 int b[], int bfirst, int blast,
                 int c[], int cfirst, int clast);

void parallelMergeSort(int a[], int first, int last, int aux[]);
void *parallelMergeSortHelper(void *threadArguments);

void binaryInsertionSort(int a[], int size);

void linearInsertionSort(int a[], int size);

void bucketSort(int a[], int size, int range);

void radixSort(int a[], unsigned long size);

void radix(int* nums, int length, int max);

//Heap Sort
void heapSort(int a[], int size);
inline int leftChild(int i);
inline int rightChild(int i);
inline int parent(int i);
void percolateDown(int a[], int i, int n);


void testSortBQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortPQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortDPQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortHS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortMS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing, int aux[]);
void testSortPMS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing, int aux[]);
void testSortBIS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortLIS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
void testSortMeta(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing);
