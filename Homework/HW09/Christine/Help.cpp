#include <iostream>
#include <pthread.h>
#define PARALEL_THREADS 1
#define LIS_THRESHOLD 500


using namespace std;
void linearInsertionSort(int a[], int size);
void mergeSort(int a[], int first, int last, int aux[]);
void *mergeSort(void *ai);
void mergeArrays(int a[], int afirst, int alast,
                 int b[], int bfirst, int blast,
                 int c[], int cfirst, int clast);

 struct ArrayIndex
{
    int level;
    int last;
    int first;
    int *array;
    int *temp;
};


int main()
{
    int a[10] = {9, 5, 8, 6, 4, 3, 2, 1, 0, 7};
    int aux[10] = {0};



    struct ArrayIndex ai;

    ai.level = PARALEL_THREADS;
    ai.first = 0;
    ai.last =  9;//sizeof(a)-1;
    ai.array = a;
    ai.temp = aux;

    cout << ai.first << " " << ai.last << "  \n";
    pthread_t thread;

    pthread_create(&thread, NULL, mergeSort, (void*)&ai); //call mergeSort;
    pthread_join(thread, NULL);

     for (int i = 0; i < 10; ++i)
        cout << a[i] << " ";
}


struct aIndex
{
    int last;
    int first;
};

void *mergeSort(void *ai)
{

    struct ArrayIndex *args;
    args = (struct ArrayIndex *)ai;

    if (args ->first >= args -> last)
        pthread_exit(NULL);

    if (!args -> level)
    {
        mergeSort(args -> array, args->first, args->last, args -> temp);
        pthread_exit(NULL);
    }

    int mid = (args->first + args->last) / 2;


    pthread_t thread1, thread2;

    ArrayIndex array1, array2;

    array1.level = PARALEL_THREADS-1;
    array1.last = mid;
    array1.first = args->first;
    array1.array = args->array;
    array1.temp = args->temp;

    array2.level = PARALEL_THREADS-1;
    array2.last = args->last;
    array2.first = mid + 1;
    array2.array = args->array;
    array2.temp = args->temp;



    if (args->first >= args->last)
        pthread_exit(NULL);



    //for(int i = 0; i < 2; i++)
        pthread_create(&thread1, NULL, mergeSort, (void*)&array1);
        pthread_create(&thread2, NULL, mergeSort, (void*)&array2);
    //for(int i = 0; i < 2; i++)
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    mergeArrays(args -> array, array1.first, array1.last, args -> array, array2.first,
               array2.last, args->temp, args->first, args->last);

    for (int i = args -> first; i <= args->last; ++i)
        args -> array[i] = args -> temp[i];
    pthread_exit(NULL);
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




