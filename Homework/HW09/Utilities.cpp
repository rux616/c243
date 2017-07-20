/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Utilities.cpp
Class:          C-243 TuTh 1700
Action:			Contains general utility functions.
*/

#include <cstdlib>
#include <iostream>

#include "Utilities.h"

using namespace std;

/****************************** swap ***************************************************************
Action:         A quick swap function specifically made for arrays.  Part of the dual pivot
                quicksort by Vladimir Yaroslavskiy.

Parameters:
IN:             int a[], holds a pointer to the start of the array a.
IN:             int i, is the element number of <a> that will be swapped with element j.
IN:             int j, is the element number of <a> that will be swapped with element i.
OUT:            None

Returns:        Nothing

Precondition:   array must be allocated, and i and j must be valid elements of the array.
***************************************************************************************************/
void swap(int a[], int i, int j)
{
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
} //swap

/****************************** testAllocation *****************************************************
Action:         Dr. Knight's and Dr. Vrajitoru's testAllocation function.  Tests a pointer of any
                type and displays an error message then exits the program if the pointer is null.

Parameters:
IN:             void *p, holds a raw pointer to somewhere in memory.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void testAllocation(void *p)
{
    if (!p)
    {
        cout << "Dynamic allocation failed. Program being terminated.\n";
        exit(1);
    }
} //testAllocation
