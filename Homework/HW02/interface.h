/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     interface.h
  Last updated:  September 11, 2014
  Description:   The functions that handle the communication with the
                 user.
**********************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "List.h"

// The main loop of the program: print the menu, input the user's
// choice, perform some action based on it.
void runTheActionsLoop ();

// Prints all the options in the program's menu.
void printMenu ();

// Reads the user's choice and asks for it again if the choice is not
// a valid one.
int actionFromUser ();

// Execute the action determined by the user's entry.
bool carryOutTheAction (int choice, List & first, List & second);

// Concatenate the lists and write appropriate messages.
void concatenateAction(List & first, List & second);
// Replace action: destroy the first list and then copy the second one
// to it.
void replaceAction(List & first, List & second);
// Empty the first list action.
void destroyAction(List &first);
// Inputs a number and inserts it at the front of the given list.
void insertFrontAction(List &first);
// Inputs a number and inserts it at the back of the given list.
void insertBackAction(List &first);
// Checks if the list is ordered and prints the appropriate message.
void isOrderedAction(List &first);
// Compares the two lists and prints the appropriate message.
void compareAction(List & first, List & second);



//----------------------
// Begin HW02 Additions
// Author: Dan Cassidy
//----------------------

//Displays a specific element in the first list (Option 17).
void retrieveAction(List &list);

//Locate a given number in the first list and print its element location (Option 18).
void locateAction(List &list);

//Insert a number after a specific element in the first list (Option 19).
void insertSpecifiedAction(List &list);

//Find the minimum element in the list (Option 20).
void minimumAction(List &list);

//Generic function to get integer input.
int inputNumber();

#endif
