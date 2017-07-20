#include <iostream>

#include <cstdlib>  //for rand() and srand()
#include <ctime>    //for time()

#include "Sort.h"

#include <cmath>    //for power()
#include <list>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//Windows implementation of the gettimeofday() function just to help with development.
#include "GetTimeOfDay.h"
#else
#include <sys/time.h>
#endif

//#include "Sort.h"

using namespace std;

void randomize(int a[], unsigned int size = 0, int lowerBound = 0, int upperBound = 99);


int main()
{
    const int NUM_ARRAYS = 6;
    const int LOWER_BOUND = 0;// 100000000;
    const int UPPER_BOUND = 9;
    const int NUM_TESTS = 1;

    //Initialize rand() and declare all the variables.
    srand(time(NULL));
    //int a1[10], b1[100], c1[1000], d1[10000], e1[100000];
    //int a2[10], b2[100], c2[1000], d2[10000], e2[100000];
    //int aux1[10], aux2[100], aux3[1000], aux4[10000], aux5[100000];
    //int *list1[5] = {a1, b1, c1, d1, e1};
    //int *list2[5] = {a2, b2, c2, d2, e2};
    //int *aux[5] = {aux1, aux2, aux3, aux4, aux5};
    struct timeval before, after;
    //double timing;
    double timing[NUM_TESTS], best, worst, average = 0, total = 0;

    //for (int i = 0; i < NUM_ARRAYS; ++i)
    //    randomize(list1[i], pow(10, i + 1), LOWER_BOUND, UPPER_BOUND);


   // cout << NUM_TESTS << " tests on arrays containing random data between " 
       //  << LOWER_BOUND << " and " << UPPER_BOUND << endl;

    for (int array = 1; array <= NUM_ARRAYS; ++array)
    {
        unsigned long size = pow(10, array);
        int *a = new int[size], *b = new int[size], *aux = new int[size];
        if (!a || !b || !aux)
        {
            cout << "Memory Allocation Error";
            //memory allocation error
            exit(1);
        }

        randomize(a, size, LOWER_BOUND, UPPER_BOUND);

      // // cout << "quick sort 10^" << array << endl;
      //  for (int test = 0; test < NUM_TESTS; ++test)
      //  {
      //      //reset the current array prior to every test
      //      for (unsigned int element = 0; element < size; ++element)
      //          b[element] = a[element];

      //      gettimeofday(&before, 0);

      //      quickSort(b, 0, size - 1);
      //      
      //      gettimeofday(&after, 0);
      //      timing[test] = (double)((double)after.tv_sec +
      //                              (double)after.tv_usec / (1000 * 1000)) -
      //                              (double)((double)before.tv_sec +
      //                              (double)before.tv_usec / (1000 * 1000));
      //      
      //  }

      //  best = worst = average = timing[0];

      //  for (int test = 1; test < NUM_TESTS; ++test)
      //  {
      //      if (best>timing[test])
      //          best = timing[test];
      //      if (worst < timing[test])
      //          worst = timing[test];
      //      average += timing[test];
      //  }

      //  total = average;
      //  average /= NUM_TESTS;

      //  /*cout << "best:    " << best << endl;
      //  cout << "worst:   " << worst << endl;
      //  cout << "average: " << average << endl;
      //  cout << "total:   " << total << endl << endl;*/



      // // cout << "heap sort 10^" << array << endl;
      //  for (int test = 0; test < NUM_TESTS; ++test)
      //  {
      //      //reset the current array prior to every test
      //      for (unsigned int element = 0; element < size; ++element)
      //          b[element] = a[element];

      //      gettimeofday(&before, 0);

      //      heapSort(b, size);

      //      gettimeofday(&after, 0);
      //      timing[test] = (double)((double)after.tv_sec +
      //                              (double)after.tv_usec / (1000 * 1000)) -
      //                              (double)((double)before.tv_sec +
      //                              (double)before.tv_usec / (1000 * 1000));
      //  }

      //  best = worst = average = timing[0];

      //  for (int test = 1; test < NUM_TESTS; ++test)
      //  {
      //      if (best>timing[test])
      //          best = timing[test];
      //      if (worst < timing[test])
      //          worst = timing[test];
      //      average += timing[test];
      //  }

      //  total = average;
      //  average /= NUM_TESTS;

      //  /*cout << "best:    " << best << endl;
      //  cout << "worst:   " << worst << endl;
      //  cout << "average: " << average << endl;
      //  cout << "total:   " << total << endl << endl;*/




      ////  cout << "merge sort 10^" << array << endl;
        for (int test = 0; test < NUM_TESTS; ++test)
        {
            //reset the current array prior to every test
            for (unsigned int element = 0; element < size; ++element)
                b[element] = a[element];

            gettimeofday(&before, 0);

            mergeSort(b, 0, size - 1, aux);

            gettimeofday(&after, 0);
            timing[test] = (double)((double)after.tv_sec +
                                    (double)after.tv_usec / (1000 * 1000)) -
                                    (double)((double)before.tv_sec +
                                    (double)before.tv_usec / (1000 * 1000));
        }

        best = worst = average = timing[0];

        for (int test = 1; test < NUM_TESTS; ++test)
        {
            if (best>timing[test])
                best = timing[test];
            if (worst < timing[test])
                worst = timing[test];
            average += timing[test];
        }

        total = average;
        average /= NUM_TESTS;

        /*cout << "best:    " << best << endl;
        cout << "worst:   " << worst << endl;
        cout << "average: " << average << endl;
        cout << "total:   " << total << endl << endl;*/



       // if (array <= 3)
       // {
       //   //  cout << "binary insertion sort 10^" << array << endl;
       //     for (int test = 0; test < NUM_TESTS; ++test)
       //     {
       //         //reset the current array prior to every test
       //         for (unsigned int element = 0; element < size; ++element)
       //             b[element] = a[element];

       //         gettimeofday(&before, 0);

       //         binaryInsertionSort(b, size);

       //         gettimeofday(&after, 0);
       //         timing[test] = (double)((double)after.tv_sec +
       //                                 (double)after.tv_usec / (1000 * 1000)) -
       //                                 (double)((double)before.tv_sec +
       //                                 (double)before.tv_usec / (1000 * 1000));
       //     }

       //     best = worst = average = timing[0];

       //     for (int test = 1; test < NUM_TESTS; ++test)
       //     {
       //         if (best>timing[test])
       //             best = timing[test];
       //         if (worst < timing[test])
       //             worst = timing[test];
       //         average += timing[test];
       //     }

       //     total = average;
       //     average /= NUM_TESTS;

       //    /* cout << "best:    " << best << endl;
       //     cout << "worst:   " << worst << endl;
       //     cout << "average: " << average << endl;
       //     cout << "total:   " << total << endl << endl;*/
       // }



       // if (array <= 2)
       // {
       //   //  cout << "linear insertion sort 10^" << array << endl;
       //     for (int test = 0; test < NUM_TESTS; ++test)
       //     {
       //         //reset the current array prior to every test
       //         for (unsigned int element = 0; element < size; ++element)
       //             b[element] = a[element];

       //         gettimeofday(&before, 0);

       //         linearInsertionSort(b, size);

       //         gettimeofday(&after, 0);
       //         timing[test] = (double)((double)after.tv_sec +
       //                                 (double)after.tv_usec / (1000 * 1000)) -
       //                                 (double)((double)before.tv_sec +
       //                                 (double)before.tv_usec / (1000 * 1000));
       //     }

       //     best = worst = average = timing[0];

       //     for (int test = 1; test < NUM_TESTS; ++test)
       //     {
       //         if (best>timing[test])
       //             best = timing[test];
       //         if (worst < timing[test])
       //             worst = timing[test];
       //         average += timing[test];
       //     }

       //     total = average;
       //     average /= NUM_TESTS;

       //     /*cout << "best:    " << best << endl;
       //     cout << "worst:   " << worst << endl;
       //     cout << "average: " << average << endl;
       //     cout << "total:   " << total << endl << endl;*/
       // }



       //// cout << endl;



       // //cout << "my radix sort 10^" << array + 1 << endl;
       // //for (int test = 0; test < NUM_TESTS; ++test)
       // //{
       // //    //reset the current array prior to every test
       // //    for (unsigned int element = 0; element < size; ++element)
       // //        b[element] = a[element];

       // //    gettimeofday(&before, 0);

       // //    radixSort(b, size);

       // //    gettimeofday(&after, 0);
       // //    timing[test] = (double)((double)after.tv_sec +
       // //                            (double)after.tv_usec / (1000 * 1000)) -
       // //                            (double)((double)before.tv_sec +
       // //                            (double)before.tv_usec / (1000 * 1000));
       // //}

       // //best = worst = average = timing[0];

       // //for (int test = 1; test < NUM_TESTS; ++test)
       // //{
       // //    if (best>timing[test])
       // //        best = timing[test];
       // //    if (worst < timing[test])
       // //        worst = timing[test];
       // //    average += timing[test];
       // //}

       // //total = average;
       // //average /= NUM_TESTS;

       // //cout << "best:    " << best << endl;
       // //cout << "worst:   " << worst << endl;
       // //cout << "average: " << average << endl;
       // //cout << "total:   " << total << endl << endl;



       // //cout << "wp radix sort 10^" << array + 1 << endl;
       // //for (int test = 0; test < NUM_TESTS; ++test)
       // //{
       // //    //reset the current array prior to every test
       // //    for (unsigned int element = 0; element < size; ++element)
       // //        b[element] = a[element];

       // //    gettimeofday(&before, 0);

       // //    radix(b, size, UPPER_BOUND);

       // //    gettimeofday(&after, 0);
       // //    timing[test] = (double)((double)after.tv_sec +
       // //                            (double)after.tv_usec / (1000 * 1000)) -
       // //                            (double)((double)before.tv_sec +
       // //                            (double)before.tv_usec / (1000 * 1000));
       // //}

       // //best = worst = average = timing[0];

       // //for (int test = 1; test < NUM_TESTS; ++test)
       // //{
       // //    if (best>timing[test])
       // //        best = timing[test];
       // //    if (worst < timing[test])
       // //        worst = timing[test];
       // //    average += timing[test];
       // //}

       // //total = average;
       // //average /= NUM_TESTS;

       // //cout << "best:    " << best << endl;
       // //cout << "worst:   " << worst << endl;
       // //cout << "average: " << average << endl;
       // //cout << "total:   " << total << endl << endl;



        delete[] a, b, aux;
    }

    return 0;
}



//Fills <size> elements of <a> with random numbers in the range <lowerBound>
//to <upperBound>, both inclusive.
void randomize(int a[], unsigned int size, int lowerBound, int upperBound)
{
    if (!size)
        return;

    for (unsigned long i = 0; i < size; ++i)
        a[i] = rand() % (upperBound + lowerBound) - lowerBound;
}

