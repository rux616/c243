/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Utilities.h
Class:          C-243 TuTh 1700
Action:			Contains function prototypes for general utility functions.
*/

#pragma once

//A quick swap function specifically made for arrays.
void swap(int a[], int i, int j);

//Dr.Knight's and Dr. Vrajitoru's testAllocation function.  Tests a pointer of any
//type and displays an error message then exits the program if the pointer is null.
void testAllocation(void *p);
