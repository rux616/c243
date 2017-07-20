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
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#endif

//The parallel functions will create 2^PARALLEL_THREADS threads.
#define PARALLEL_THREADS 1
//Arrays with this number of elements or less will use the linear insertion sort.
#define LIS_THRESHOLD 500
//Arrays with this number of elements or less (bounded by LIS_THRESHOLD) will use the
//dual pivot quicksort.
#define PMS_THRESHOLD 100000
//Arrays that have at least this distance between the largest and smallest elements and
//have more elements than specified by PMS_THRESHOLD will use the parallel merge sort.
#define DISTANCE_THRESHOLD 10000



using namespace std;

void metaSort(int a[], unsigned int size)
{
    //Test to see if we can use the linear insertion sort.
    if (size <= LIS_THRESHOLD)
    {
        linearInsertionSort(a, size);
        return;
    }
    //Test to see if we can use the dual pivot quicksort
    else if (size <= PMS_THRESHOLD)
    {
        dualPivotQuickSort(a, 0, size - 1, 3);
        return;
    }

    //The number of elements is over PMS_THRESHOLD, so do a quick examination of the array
    //to see if it warrants using the parallel merge sort or whether the dual pivot quicksort
    //will still work.
    int min = a[0], max = a[0];
    for (unsigned int i = 1; i < size; ++i)
    {
        if (max < a[i])
            max = a[i];
        else if (min > a[i])
            min = a[i];
    }

    //
    if (max - min > DISTANCE_THRESHOLD)
    {
        int *aux = new int[size];
        testAllocation(aux);
        parallelMergeSort(a, 0, size - 1, aux);
        delete[] aux;
        return;
    }
    else
    {
        dualPivotQuickSort(a, 0, size - 1, 3);
        return;
    }
}


void basicQuickSort(int a[], int first, int last)
{
    if (last <= first)
        return;

    int pivot = a[first];
    int i = first + 1, j = last;

    while (i < j)
    {
        while (a[i] < pivot && i < j)
            ++i;
        while (a[j] > pivot)
            --j;
        if (i < j)
            swap(a[i++], a[j--]);
    }

    if (a[j] > pivot)
        j--;

    swap(a[j], a[first]);

    quickSort(a, first, j - 1);
    quickSort(a, j + 1, last);
}

void quickSort(int a[], int first, int last)
{
    if (last - first < LIS_THRESHOLD)
    {
        linearInsertionSort(&a[first], last - first + 1);
        return;
    }
    //if (last <= first)
    //    return;

    //swap(a[first], a[determinePivot(first, last)]);
    swap(a, first, (first + last) / 2);
    int pivot = a[first]; //should be changed

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

    //a[first] is simply the pivot location
    swap(a, j, first);

    quickSort(a, first, j - 1);
    quickSort(a, j + 1, last);
}



void parallelQuickSort(int a[], int first, int last)
{
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
}

void *parallelQuickSortHelper(void *threadArguments)
{
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
}


void dualPivotQuickSort(int a[], int left, int right, int div)
{
    int len = right - left;

    if (len < LIS_THRESHOLD)
    { // insertion sort for tiny array
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
}



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
}



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
}



void parallelMergeSort(int a[], int first, int last, int aux[])
{
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
}

void *parallelMergeSortHelper(void *threadArguments)
{
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
}

void binaryInsertionSort(int a[], int size)
{
    for (int p = 1; p<size; ++p)
    {
        bool found;
        int k, temp;
        binarySearch(a, a[p], 0, p - 1, found, k);
        temp = a[p];
        for (int i = p - 1; i >= k; --i)
            a[i + 1] = a[i];
        a[k] = temp;
    }
} // binaryInsertionSort()



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
} // linearInsertionSort()



void bucketSort(int a[], int size, int range)
{
    int * buckets = new (nothrow) int[range], i, j, k;

    for (j = 0; j < range; j++)
        buckets[j] = 0;
    for (i = 0; i < size; i++)
        buckets[a[i]]++;
    for (i = 0, j = 0; j < range; j++)
        for (k = 0; k < buckets[j]; k++)
            a[i++] = j;

    delete[] buckets;
} // bucketSort()



void radixSort(int a[], unsigned long size)
{
    if (!size)
        return;

    const int NUM_BUCKETS = 10;
    list<int> buckets[NUM_BUCKETS];

    int largest = a[0];
    for (unsigned long i = 1; i < size; ++i)
        if (largest < a[i])
            largest = a[i];

    unsigned int maxLength = numDigits(largest);

    for (unsigned int i = 0; i < maxLength; ++i)
    {
        for (unsigned int j = 0; j < size; ++j)
        {
            unsigned int position = a[j] / int(pow(10, i)) % 10;
            buckets[position].push_back(a[j]);
        }
        int k = 0;
        for (int j = 0; j < NUM_BUCKETS; ++j)
        {
            while (buckets[j].size())
            {
                a[k++] = buckets[j].front();
                buckets[j].pop_front();
            }
        }
    }
}



#define BASE 10
void radix(int* nums, int length, int max)
{
    list<int> bucket[BASE];
    int i;

    // iterate through each radix until n>max
    for (int n = 1; max >= n; n *= BASE)
    {
        // sort list of numbers into buckets
        for (i = 0; i<length; i++)
            bucket[(nums[i] / n) % BASE].push_back(nums[i]);

        // merge buckets back to list
        for (int k = i = 0; i<BASE; bucket[i++].clear())
            for (list<int>::iterator j = bucket[i].begin(); j != bucket[i].end(); nums[k++] = *(j++));
    }
}



void heapSort(int a[], int size)
{
    for (int i = (size - 2) / 2; i >= 0; i--)
        percolateDown(a, i, size);
    for (int j = size - 1; j>0; j--)
    {
        swap(a, 0, j);
        percolateDown(a, 0, j);
    }
} // heapsort()



inline int leftChild(int i)
{
    return 2 * i + 1;
}



inline int rightChild(int i)
{
    return 2 * i + 2;
}



inline int parent(int i)
{
    return (i - 1) / 2;
}



void percolateDown(int a[], int i, int n)
{
    int child, temp;
    for (temp = a[i]; leftChild(i)<n; i = child)
    {
        child = leftChild(i);
        if (child != n - 1 &&
            a[child] < a[child + 1])
            child++;
        if (temp < a[child])
            a[i] = a[child];
        else
            break;
    }
    a[i] = temp;
}


void testSortBQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nBasic Quick Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        basicQuickSort(b, 0, size - 1);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nQuick Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        quickSort(b, 0, size - 1);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortPQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nParallel Quick Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        parallelQuickSort(b, 0, size - 1);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortMeta(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nMeta Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        metaSort(b, size);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortDPQS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nDual Pivot Quick Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        dualPivotQuickSort(b, 0, size - 1, 3);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortHS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nHeap Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        heapSort(b, size);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortMS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing, int aux[])
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nMerge Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        mergeSort(b, 0, size - 1, aux);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortPMS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing, int aux[])
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nParallel Merge Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        parallelMergeSort(b, 0, size - 1, aux);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortLIS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nLinear Insertion Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        linearInsertionSort(b, size);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}

void testSortBIS(int a[], int b[], unsigned long size, unsigned tests, timingSummary &timing)
{
    timeval before, after;
    double *rawTiming = new (nothrow) double[tests];
    testAllocation(rawTiming);

    cout << "\nBinary Insertion Sort - " << size << " Elements - " << tests << " Tests\n";
    for (unsigned test = 0; test < tests; ++test)
    {
        //reset the current array prior to every test
        for (unsigned long element = 0; element < size; ++element)
            b[element] = a[element];

        gettimeofday(&before, 0);

        binaryInsertionSort(b, size);

        gettimeofday(&after, 0);
        rawTiming[test] = (double)((double)after.tv_sec +
                                   (double)after.tv_usec / (1000 * 1000)) -
                                   (double)((double)before.tv_sec +
                                   (double)before.tv_usec / (1000 * 1000));
    }

    timing.best = timing.worst = timing.average = rawTiming[0];

    for (unsigned test = 1; test < tests; ++test)
    {
        if (timing.best>rawTiming[test])
            timing.best = rawTiming[test];
        if (timing.worst < rawTiming[test])
            timing.worst = rawTiming[test];
        timing.average += rawTiming[test];
    }

    timing.total = timing.average;
    timing.average /= tests;
}
