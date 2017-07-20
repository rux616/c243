#include <iostream>
using namespace std;

#ifndef HARDENED_INPUT
#define HARDENED_INPUT

/****************************** hardenedInput ******************************************************
Action:         Hardened input function intended to be used for numeric entry that can handle bad
                user input including overflows, EOFs, and type mismatches.  Can be configured to
                output a a customizable error message on an invalid input, return false on an
                invalid input, or exit on an invalid input, depending on their respective flags.

Parameters:
IN:             bool messageOnError, determines whether the function will display an error message
                when invalid input is detected.  Default value is false.
IN:             char message[], holds the error message to display if the messageOnError flag is
                set.  Default message is is "Invalid input.\n"
IN:             bool failOnError, determines whether the function will simply return false if
                invalid input is detected.  Default value is false.
IN:             bool exitOnError, determines whether the function will simply exit the program when
                an invalid input is detected.  Takes precedence over the failOnError flag.  Default
                value is false.
IN:             int exitCode, holds the error code the system is given if the program is terminated
                because the exitOnError flag is set.
OUT:            variable TYPE &data, holding the data which was input.

Returns:        bool, showing whether the input was successful or not.  Will always be true unless
                the failOnError flag is set.

Precondition:   None
***************************************************************************************************/
template <class T>
bool hardenedInput(T &data, bool messageOnError = false, char message[] = "Invalid input.\n",
                   bool failOnError = false, bool exitOnError = false, int exitCode = 1)
{
    T inputData;

    //Saves the current state of C++/C stream synching then turns it off, otherwise in_avail()
    //always returns 0 in gcc/g++ due to their implementation of it.  Visual Studio is unaffected.
    bool priorSyncStatus = cin.sync_with_stdio();
    cin.sync_with_stdio(false);

    //Clear any pre-existing errors and the input buffer.
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
    }
        
    do
    {
        //Checks for any fail and bad flags on cin.
        if (cin.fail())
        {
            //Outputs an error message if the flag is set.
            if (messageOnError)
                cout << message;

            //Exits if the flag is set.
            if (exitOnError)
                exit(exitCode);
            
            //Clears the fail and bad flags on cin.
            cin.clear();

            //Clears anything and everything currently in the input buffer.
            cin.ignore(cin.rdbuf()->in_avail());

            //Return false to indicate an error has occured if the flag is set.
            if (failOnError)
                return false;
        }

        //Ask for input.
        cin >> inputData;
    } while (cin.fail());

    //If input is good, flush the remainder of the input buffer (except for the trailing \n).
    cin.ignore(cin.rdbuf()->in_avail() - 1);
    
    //Return stream synching to its prior state.
    cin.sync_with_stdio(priorSyncStatus);

    //Set the output variable to the new value
    data = inputData;

    //Return true to signify success.
    return true;
} //bool hardenedInput

#endif
