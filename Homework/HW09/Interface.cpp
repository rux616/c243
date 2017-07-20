/*
Name:			Dan Cassidy and Christine Jordan
Date:			2014-11-18
Homework #:		09
Source File:	Interface.cpp
Class:          C-243 TuTh 1700
Action:			Contains interface-related functions.
*/

#include <cstdlib>
#include <iostream>

#include "Interface.h"
#include "Sort.h"
#include "Utilities.h"

using namespace std;

/****************************** buildArray *********************************************************
Action:         Interacts with the user and builds an array of user-specified size with user-
                specified elements.

Parameters:
IN:             None
OUT:            int *&array, holds the pointer to the newly-created array.
OUT:            unsigned long &size, holds the size of the newly-created array.

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void buildArray(int *&array, unsigned long &size)
{
    //If an existing array is passed in, deallocate it and reset the size first.
    if (array != NULL)
    {
        delete[] array;
        size = 0;
    }
    
    //Get the size of the array.
    cout << "How many elements should the array have?\n";
    getInput(size);

    //Check the requested size of the array.
    if (!size)
    {
        cout << "\nThe number of elements must be greater than zero.\n";
        exit(1);
    }

    //Allocate a new array with the desired number of elements.
    array = new (nothrow) int[size];
    //Verify that the array was allocated successfully.
    testAllocation(array);
    //Fill the newly created array with zeroes.
    for (unsigned i = 0; i < size; ++i)
        array[i] = 0;

    //Get the elements of the array.
    cout << "\nType the elements separated by spaces or new lines.\n"
         << "Hit CTRL-Z (Windows) or CTRL-D (Linux) then enter when you are done.\n";
    for (unsigned i = 0; i != size && getInput(array[i]); ++i);
} //buildArray

/****************************** getInput ***********************************************************
Action:         Template function.  Utilizes 'cin >>' to get read user input into the passed in
                variable.  Will ask the user to reinput a detected bad input and returns true or
                false based on whether the input was good (true) or EOF (false).

Parameters:
IN:             None
OUT:            T &data, the variable holding the data that was input.

Returns:        bool, indicating whether the input operation was successful and data is valid (true)
                or whether EOF was encountered and data should not be used (false).

Precondition:   None
***************************************************************************************************/
template <class T> bool getInput(T &data)
{
    T temp;

    //Loop while EOF is not encountered.  This also protects against prior EOFs in the input stream.
    while (!cin.eof())
    {
        //Get the user input and store it in a temporary variable.
        cin >> temp;
        //Check to make sure the input was good.
        if (!cin.good())
        {
            //If the input was not good, determine why.
            if (cin.eof())
                //If due to EOF, return false to indicate as such.
                return false;
            else
            {
                //If due to bad input, say so, clear the flags in cin, and eat the rest of the line.
                cout << "Invalid input. Please try again.\n";
                cin.clear();
                while (cin.get() != '\n');
                //This will then loop so the user can try again.
            }
        }
        else
        {
            //The input was good, so store it in data and return true indicating valid data.
            data = temp;
            return true;
        }
    }

    //Default return to indicate invalid data.
    return false;
} //getInput

/****************************** printArray *********************************************************
Action:         Prints the array passed in to it.

Parameters:
IN:             int array[], holds a pointer to the beginning of an array.
IN:             unsigned long size, holds the number of elements in the array.
OUT:            None

Returns:        Nothing

Precondition:   The array <array> MUST be allocated and have at least <size> number of elements.
***************************************************************************************************/
void printArray(int array[], unsigned long size)
{
    //Print that array!
    for (unsigned long i = 0; i < size; ++i)
        cout << array[i] << ' ';
} //printArray

/****************************** printTiming ********************************************************
Action:         Prints the timing data contained within a variable of type timingSummary.

Parameters:
IN:             timingSummary timing, holds the timing data to be printed.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void printTiming(timingSummary timing)
{
    //Print that timing data!
    cout << "Best:     " << timing.best << "\n";
    cout << "Worst:    " << timing.worst << "\n";
    cout << "Average:  " << timing.average << "\n";
    cout << "Total:    " << timing.total << "\n";
} //printTiming
