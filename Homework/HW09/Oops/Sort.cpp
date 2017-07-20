#include "Sort.h"
#include "Utilities.h"
#include <list>
#include <cmath>
#include <iostream>

using namespace std;

void sortAnalyze(int a[], unsigned int size)
{
    if (!size)
        return;

    int min = a[0], max = a[0];

    for (unsigned int i = 1; i < size; ++i)
    {
        if (max < a[i])
            max = a[i];
        else if (min > a[i])
            min = a[i];
    }


}



void quickSort(int a[], int first, int last)
{
    if (last <= first)
        return;

    int pivot = a[first]; //should be changed
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

    //if (a[j] > pivot)
    //    --j;

    swap(a[j], a[first]);

    quickSort(a, first, j - 1);
    quickSort(a, j + 1, last);
    for (int i = 0; i < last; ++i)
        cout << a[i] << " ";
}



void mergeSort(int a[], int first, int last, int aux[])
{
    if (last <= first)
        return;

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
    for (int i = 0; i < clast; ++i)
        cout << c[i] << " ";
}



void binaryInsertionSort(int a[], int size)
{
    for (int p = 2; p <= size; ++p)
    {
        bool found;
        int k;
        binarySearch(a, a[p], 1, p - 1, found, k);
        a[0] = a[p];
        for (int i = p - 1; i >= k; --i)
            a[i + 1] = a[i];
        a[k] = a[0];
    }
}



void linearInsertionSort(int a[], int size)
{
    for (int p = 1; p<size; ++p)
    {
        int i = p - 1;
        while (i >= 0 && a[i] > a[i + 1])
        {
            swap(a[i], a[i + 1]);
            i--;
        }
    }
} // linearInsertionSort()




void bucketSort(int a[], int size, int range)
{
    int * buckets = new int[range], i, j, k;

    for (j = 0; j < range; j++)
        buckets[j] = 0;
    for (i = 0; i < size; i++)
        buckets[a[i]]++;
    for (i = 0, j = 0; j < range; j++)
        for (k = 0; k < buckets[j]; k++)
            a[i++] = j;

    delete[] buckets;
} // bucketSort()



void radixSort(int a[], int size)
{
    if (!size)
        return;

    const int NUM_BUCKETS = 10;
    list<int> buckets[NUM_BUCKETS];

    unsigned int largest = a[0];
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
        swap(a[0], a[j]);
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
