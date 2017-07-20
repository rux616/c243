#include <cstdlib>
#include <iostream>

#include "Utilities.h"

using namespace std;

/****************************** binarySearch *******************************************************
Action:         Binary search function written by Drs. Knight and Vrajitoru.  Searches through <a>
                in a binary way for <target>.
                

Parameters:
IN:             const int a[], 
IN:             const int &target, 
IN:             int first, 
IN:             int last, 
OUT:            bool &found, 
OUT:            int &subscript, 

Returns:        Nothing

Precondition:   
***************************************************************************************************/
void binarySearch(const int a[], const int &target, int first,
                  int last, bool &found, int &subscript)
{
    int mid;
    found = false; // The target hasn't been found.
    while (first <= last && !found) // The value parameters "first"
    {                               // and "last" are modified
        mid = (first + last) / 2;   // during loop execution.
        if (target < a[mid])
            last = mid - 1;
        else if (a[mid] < target)
            first = mid + 1;
        else // only remaining possibility: a[mid] matches target
            found = true;
    }
    if (found)
        subscript = mid; // The location of "target".
    else
        subscript = first;          // This is the appropriate subscript to
} //binarySearch                    // return if "target" is not present.

/****************************** getChar ************************************************************
Action:         Reads a character input by the user and checks it against the contents of the
                character array 'allowed'.  If the input is not allowed, the user is asked to enter
                another character.  If 'allowed' is empty (default), any character is accepted.

Parameters:
IN:             const char allowed[], holds all of the allowed characters.  Defaults to "".
OUT:            char &c, contains a reference to the variable that will hold the user input.

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void getChar(char &c, const char allowed[])
{
    bool isValid = true;

    do
    {
        //If this is the first time through the loop, isValid will be true.  If it is false, meaning
        //this is NOT the first time through the loop, display the following prompt for the user,
        //letting them know what inputs are acceptable.
        if (!isValid)
        {
            cout << "Please enter one of the following characters: ";
            for (int i = 0; allowed[i]; ++i)
            {
                if (i)
                    cout << ", ";
                cout << allowed[i];
            }
            cout << "\nEnter your choice: ";
        }

        //Ask the user for input.
        cin >> c;

        //Assume input is invalid pending tests.
        isValid = false;

        //If the allowed set of characters is not empty, input is restricted.  Proceed to test the
        //validity of the user input.
        if (allowed != "")
        {
            for (int i = 0; allowed[i]; ++i)
                if (c == allowed[i])
                    isValid = true;
        }

        //If the allowed set of characters is empty, any input is acceptable.
        else
            isValid = true;

        //Empty the rest of the line.
        while (cin.get() != '\n');

    } while (!isValid);
} //getChar

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

/****************************** isSorted ***********************************************************
Action:         Tests an array to see if it is sorted sequentually from smallest to largest.
                Returns true if yes, false if no.

Parameters:
IN:             int array[], holds a pointer to the start of the array to test.
IN:             unsigned long size, holds the number of elements in array.
IN:             bool showOutput, 
OUT:            None

Returns:        bool,

Precondition:   array must be allocated.  size must be the number of elements in array.
***************************************************************************************************/
bool isSorted(int array[], unsigned long size, bool showOutput)
{
    bool isSorted = true;
    int temp = array[0];

    for (unsigned long i = 0; !(i >= size) || !isSorted; ++i)
    {
        if (array[i] < temp)
        {
            isSorted = false;
            if (showOutput)
                cout << "Warning!  Element " << i - 1 << " (" << temp 
                     << ") is greater than element " << i << " (" << array[i] << ").\n";
        }
        temp = array[i];
    }

    return isSorted;
} //isSorted

/****************************** numDigits **********************************************************
Action:         Determines how many digits are in a number.

Parameters:
IN:             int number, holds the number in question.
OUT:            None

Returns:        unsigned int holding the number of digits.

Precondition:   None
***************************************************************************************************/
unsigned int numDigits(int number)
{
    //Initializes count to 1 if number is 0, or 0 if number is something else.
    unsigned char count = !number;

    //If the number is negative, make it positive.
    if (number < 0)
        number *= -1;

    //Loop while number is not zero, checking if it is smaller than certain numbers.
    //If it is, then increment count based on the number, then divide the entire number
    //by 10000 and start the cycle over again.  This helps to minimize the number of
    //division operations.
    while (number)
    {
        if (number < 10)
            ++count;
        else if (number < 100)
            count += 2;
        else if (number < 1000)
            count += 3;
        else
            count += 4;
        number /= 10000;
    }

    return count;
} //numDigits

/****************************** randomize **********************************************************
Action:         Randomizes the content of an array.

Parameters:
IN:             int a[], holds a pointer to the array to be randomized.
IN:             unsigned long size, holds the number of elements in the array to be randomized.
IN:             int lowerBound, holds the inclusive lower bound of the numbers to be generated.
                Defaults to 0.
IN:             int upperBound, holds the inclusive upper bound of the numbers to be generated.
                Defaults to 99.
OUT:            int a[], holds a pointer to the array that has been randomized.

Returns:        Nothing

Precondition:   Array <a> MUST be allocated and have at least <size> number of elements.
***************************************************************************************************/
void randomize(int a[], unsigned long size, int lowerBound, int upperBound)
{
    //Verify that the size is valid and that the lower bound is not greater than the upper bound.
    if (!size || upperBound < lowerBound)
        return;

    //Determine the distance between upperBound and lowerBound.
    unsigned distance = abs(upperBound - lowerBound);

    //Randomize the elements in the array.
    for (unsigned i = 0; i < size; ++i)
        a[i] = rand() % (distance + 1) + lowerBound;
} //randomize

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
