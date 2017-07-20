/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     interface.cc
  Last updated:  September 11, 2014
  Description:   The functions that handle the communication with the
                 user.
**********************************************************************/

#include <iostream>
using namespace std;

#include "interface.h"

//Added 5 to this constant to facilitate the extra menu options in homework 02.
const int NUMBER_OF_MENU_ITEMS = 21;

/*******************  R U N   T H E   A C T I O N S   L O O P  *************

This function creates two empty linked lists that can be manipulated by the
interactive user of this program.  It then goes into a loop that repeatedly
(1) prints a menu of list actions for the user to choose from,
(2) reads the number of the menu item (action) selected by the user,
(3) attempts to carry out the action selected.
Documented by W. Knight.  Coded by W. Knight, modified by Dr. Vrajitoru.  */

void runTheActionsLoop ()
{
    List     first, second;  // The linked lists in the program.
    int      actionNumber;
    bool     timeToQuit;   // Will be set to true when the user wants to quit.
  
    cout << "\nThe program has now created two empty lists.\n"
         << "Press the Enter key to see a menu of operations you can"
         << " perform: ";

    while (cin.get() != '\n') ;   // Clear the user's line.
    cout << endl;                 // Echo their end of line character.

    do // main loop of the program
    {
        printMenu ();
      
        actionNumber = actionFromUser ();
      
        timeToQuit = carryOutTheAction (actionNumber, first, second);

    }  while (! timeToQuit);
} // runTheActionsLoop ()

/**************************  P R I N T   M E N U  **************************

This function prints a menu of possible actions that the interactive
user can choose to perform on the linked list in the program.
Documented by W. Knight.  Coded by W. Knight, modified by
Dr. Vrajitoru.  */

void printMenu ()
{
    cout << " 0) EXIT FROM THE PROGRAM.\n";
    cout << " 1) Combine the lists by attaching the second to the first.\n";
    cout << " 2) Copy the second list to the first after destroying first.\n";
    cout << " 3) Count the number of negative values in the first list.\n";
    cout << " 4) Destroy the first list (i.e., cause it to become empty).\n";
    cout << " 5) Insert a number at the front of the first list.\n";
    cout << " 6) Insert a number at the front of the second list.\n";
    cout << " 7) Insert a number at the back of the first list.\n";
    cout << " 8) Insert a number at the back of the second list.\n";
    cout << " 9) Print all the data in the first list.\n";
    cout << "10) Print all the data in the second list.\n";
    cout << "11) Print the last number in the first list.\n";
    cout << "12) Test whether the 1st list is ordered.\n";
    cout << "13) Test whether the two lists are alike.\n";
    cout << "14) Sort the first list.\n";
    cout << "15) Remove the first node from the first list.\n";
    //Added text for options 16-20 per homework 02 guidelines.
    cout << "16) Compute the sum of all the numbers in the first list.\n";
    cout << "17) Display a specific element in the first list.\n";
    cout << "18) Locate a given number in the first list.\n";
    cout << "19) Insert a number after a specific element in the first list.\n";
    cout << "20) Find the minimum number in the first list.\n";
} // printMenu()

/*********************  A C T I O N   F R O M   U S E R  ******************/
// Reads the user's choice and asks for it again if the choice is not
// a valid one.
// Coded by W. Knight.

int actionFromUser ()
{
    int choice;

    cout << "\nType a choice from the menu (it will be echoed): ";
    cin  >> choice;
    cout << choice << endl;  // Echo the user's choice.

    while ( !cin.good() || choice < 0 || choice >= NUMBER_OF_MENU_ITEMS )
    {
        cout << "\nYou must enter a number from the menu.\n"
             << "Please try again.\n\n";
      
        cin.clear();                 // Reset the "good" bit.
        while (cin.get() != '\n') ;  // Clear line entered by the user.

        cout << "\nYour choice from the menu: ";
        cin  >> choice;
        cout << choice << endl;  // Echo the user's choice.
    }

    while (cin.get() != '\n') ; // Clear rest of line after the user's
    // choice
    return choice;
} // actionFromUser()

/****************  C A R R Y   O U T   T H E   A C T I O N  ****************

This function attempts to carry out an action on a linked list pointed
to by the parameter "front".  The action is indicated by the parameter
"choice".  The function consists almost entirely of a lengthy switch
statement that calls the appropriate list function and then reports
the results of the call.  Documented by W. Knight.  Coded by
W. Knight, modified by Dr. Vrajitoru.  */
 
bool carryOutTheAction (int choice, List & first, List & second)
{
    cout << "\n\n\n\n" << endl;

    switch (choice) 
    {
    case 0:  
        cout << endl << endl;
        return true;  // It's time to quit the program.
    case 1:  
        concatenateAction(first, second);    
        break;
    case 2:  
        replaceAction(first, second);
        break;
    case 3:  
        cout << "The first list contains " << first.countNegative()
             << " negative numbers." << endl;
        break;
    case 4:  
        destroyAction(first); 
        break;
    case 5:  
        insertFrontAction(first);
        break;
    case 6:  
        insertFrontAction(second);
        break;
    case 7:  
        insertBackAction(first);
        break;
    case 8:  
        insertBackAction(second);
        break;
    case 9:  
        cout << "Here is the content of the first list:\n";
        first.print();
        break;
    case 10:  
        cout << "Here is the content of the second list:\n";
        second.print();
        break;
    case 11:  
        first.printLast();
        break;
    case 12: 
        isOrderedAction(first);
        break;
    case 13: 
        compareAction(first, second);
        break;
    case 14:
        //Altered to use the selection sort per homework 02 guidelines.
        first.selectionSort();
        cout << "Here is the content of the sorted list:\n";
        first.print();
        break;
    case 15:
        first.removeFront();
        cout << "Here is the content of the list after removing "
             << "the first number:\n";
        first.print();
        break;
    //Cases 16-20 are added per homework 02 guidelines.
    case 16:
        cout << "The sum of all the numbers in the first list is "
             << first.sum() << ".\n";
        break;
    case 17:
        retrieveAction(first);
        break;
    case 18:
        locateAction(first);
        break;
    case 19:
        insertSpecifiedAction(first);
        break;
    case 20:
        minimumAction(first);
        break;
    } // end of switch statement
  
    cout << "\nPress the Enter key to continue. ";
    while (cin.get() != '\n') ; // Clear the line
    cout << endl;                 // Echo their end of line character.

    return false;  // Do not quit the program.
} // carryOutTheAction ()

// Concatenate the lists and write appropriate messages.
void concatenateAction(List & first, List & second)
{
    first.concatenate(second);
    cout << "The second list has been attached to the first.\n";
    if (!second)
        cout << "The second list is now empty.\n";
    else
        cout << "Second list should now be empty, but it's not.\n";
} // concatenateAction()

// Replace action: destroy the first list and then copy the second one
// to it.
void replaceAction(List & first, List & second)
{
    first.makeEmpty();
    first = second;
    cout << "The two lists should now be identical.\n";
} // replaceAction()

// Empty the first list action.
void destroyAction(List &first)
{
    first.makeEmpty();
    if (!first)
        cout << "The first list is now empty.\n";
    else
        cout << "The first list should be empty, but it's not.\n";
} // destroyAction()

// Inputs a number and inserts it at the front of the given list.
void insertFrontAction(List &first)
{
    int number;
    cout << "\nEnter the number to be added: ";
    //Used the new getInput function here for code reusability.
    number = inputNumber();

    first.insertFront (number);
} // insertFrontAction()

// Inputs a number and inserts it at the back of the given list.
void insertBackAction(List &first)
{
    int number;
    cout << "\nEnter the number to be added: ";
    //Used the new getInput function here for code reusability.
    number = inputNumber();

    first.insertBack (number);
} // insertBackAction()

// Checks if the list is ordered and prints the appropriate message.
void isOrderedAction(List &first)
{
    if (first.isOrdered())
        cout << "The list is ordered." << endl;
    else
        cout << "The list is not ordered." << endl;
} // isOrderedAction()

// Compares the two lists and prints the appropriate message.
void compareAction(List & first, List & second)
{
    if (first == second)
        cout << "The lists contain the numbers in the same order.\n";
    else
        cout << "The lists are not alike.\n";
} // compareAction()



//----------------------
// Begin HW02 Additions
// Author: Dan Cassidy
//----------------------

/****************************** retreiveAction *****************************************************
Action:         Displays a specific element in the first list (Option 17).  The element is zero-
                based, meaning that the first element is element 0, the second element is element 1,
                and so on.

Parameters:
IN:             List &list, contains a reference to the list from which the node is to be retrieved.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void retrieveAction(List &list)
{
    int number;

    //Get the element number to retrieve.
    cout << "Enter the zero-based position of the element you would like to retrieve: ";
    number = inputNumber();

    //Retrieve the desired node.
    ListIterator retreivedNode = list.nodeAtPosition(number);

    //If the node exists, print off the number it contains.
    if (retreivedNode)
        cout << "\nThe node at position " << number << " contains the number "
             << *retreivedNode << ".\n";

    //If the node does not exist, state as much.
    else
        cout << "\nThe node at position " << number << " does not exist.\n";
} // retrieveAction()

/****************************** locateAction *******************************************************
Action:         Locates a given number in the first list and prints its element location (Option
                18).  The printed element is zero-based, so element 0 is the first (head) node of
                the list, element 1 is the second node, and so on.

Parameters:
IN:             List &list, contains a reference to the list which will be searched.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void locateAction(List &list)
{
    int number = 0, location = -1;

    //Get the number being searched for.
    cout << "Enter the number you would like to search for in the first list: ";
    number = inputNumber();

    //Attempt the locate the number entered, then get the position of the containing node.
    location = list.positionAtNode(list.locate(number));

    //If the number was not found state as much.
    if (location == -1)
        cout << "\nThe number " << number << " was not found in the first list.\n";

    //If the number was found, display the element location of said number.
    else
        cout << "\nThe number " << number << " was found at element " << location << ".\n";
} // locateAction()

/****************************** insertSpecifiedAction **********************************************
Action:         Inserts a number after a specific element in the first list (Option 19).  This
                function uses a zero-based element number, so the first node is element 0, the
                second node is element 1, and so on.  To insert a number at the head of the list,
                use element -1.

Parameters:
IN:             List &list, contains a reference to the list that is going to have a node inserted.
OUT:            List &list, contains a reference to the list that has had a node inserted.

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void insertSpecifiedAction(List &list)
{
    int number = 0, location = -1;
    ListIterator insertionPoint = NULL;

    //Get the number to add to the list.
    cout << "Enter a number to add to the list: ";
    number = inputNumber();

    //Get the element where the user would like to add the previously entered number.
    cout << "\nEnter the zero-based element you would like to add the number after, -1 to\n"
         << "add to the front of the list: ";
    location = inputNumber();

    //Check to make sure the node the user is trying to insert after actually exists.
    if (location != -1)
        insertionPoint = list.nodeAtPosition(location);
    
    //If the node doesn't exist, display an error.
    if (location != -1 && !insertionPoint)
        cout << "\nError, that element does not exist in this list.\n";

    //If the node exists, call the insertAfter function and let it do its thing.
    else
        list.insertAfter(insertionPoint, number);
} // insertSpecifiedAction()

/****************************** minimumAction ******************************************************
Action:         Finds the minimum value of the first list (Option 20).  Will print an error message
                if the list is empty.

Parameters:
IN:             List &list, contains a reference to the list being searched.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void minimumAction(List &list)
{
    //Declare a ListIterator and use it to store the location of the smallest element in the list.
    ListIterator minNode = list.begin().min();

    //Verify the node exists and if it does, print the value it contains.
    if (minNode)
        cout << "The minimum number in the first list is "
             << *minNode << ".\n";

    //If it doesn't exist, print an error message.
    else
        cout << "Error, the list is empty.\n";
} // minimumAction()

/****************************** inputNumber ********************************************************
Action:         Generic function to get integer input.  *NOTE* This code was already here and was
                simply moved into its own function to facilitate ease of (re-)use.

Parameters:
IN:             None
OUT:            None

Returns:        int, containing the number given as input.

Precondition:   None
***************************************************************************************************/
int inputNumber()
{
    int number;

    //Start pre-existing code.
    cin >> number;
    while (!cin.good())
    {
        cout << "\nAn integer number is required.\n"
             << "Please try again.\n\n";
        cin.clear();                 // Reset the "good" bit.
        while (cin.get() != '\n');   // Clear line entered by user.
    }
    cout << number << endl;          // Echo the number.

    while (cin.get() != '\n');       // Clear the line
    //End pre-existing code.

    return number;
} // inputNumber()
