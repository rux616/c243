#include <cstdlib>
#include <iostream>

#include "Utilities.h"

using namespace std;

/****************************** getInput ***********************************************************
Action:         Utilizes 'cin >>' to get read user input into the passed in variable.  Will ask the
                user to reinput a detected bad input and returns true or false based on whether the
                input was good (true) or EOF (false).

Parameters:
IN:             None
OUT:            T &data, the variable holding the data that was input.

Returns:        bool, indicating whether the input operation was successful and data is valid (true)
                or whether EOF was encountered and data should not be used (false).

Precondition:   None
***************************************************************************************************/
bool getInput(int &data)
{
    int temp;

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
