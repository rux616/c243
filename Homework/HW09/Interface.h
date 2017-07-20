/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Interface.h
Class:          C-243 TuTh 1700
Action:			Contains interface-related function prototypes.
*/

#pragma once

#include "Sort.h" //For timingSummary struct.

//Interacts with the user and builds an array of user-specified size with user-
//specified elements.
void buildArray(int *&array, unsigned long &size);

//Template function.  Utilizes 'cin >>' to get read user input into the passed in
//variable.  Will ask the user to reinput a detected bad input and returns true or
//false based on whether the input was good(true) or EOF(false).
template <class T> bool getInput(T &data);

//Prints the array passed in to it.
void printArray(int array[], unsigned long size);

//Prints the timing data contained within a variable of type timingSummary.
void printTiming(timingSummary timing);
