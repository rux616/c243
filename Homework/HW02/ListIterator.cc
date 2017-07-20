/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     ListIterator.cc
  Last updated:  September 11, 2014
  Description:   Definition of the list iterator class.
**********************************************************************/

#include "ListIterator.h"
#include <cstdlib>
#include <iostream>
using namespace std;

// Default constructor. We need to make sure the pointer is initialized
// to NULL.
ListIterator::ListIterator()  
    : current(NULL)
{
} // ListIterator::ListIterator()

// Constructor from a ListNode pointer.
ListIterator::ListIterator(ListNode *link)
{
    current = link; // just store the pointer in the target object
} // ListIterator::ListIterator()

// Assignment operator with a ListNode pointer
ListNode *ListIterator::operator=(ListNode *link)
{
    current = link; // store the pointer in the target object
    return current; // then return it
} // ListIterator::operator=()

// Operator to advance the pointer.
ListIterator &ListIterator::operator++()
{
    if (current)                 // if the pointer is not null
        current = current->next; // move it to the next node
    return *this;
} // ListIterator::operator++()

// Operator to access the content of the node. If the pointer is null, 
// it exits the program with an error message.
int &ListIterator::operator*()
{
    if (current)               // if the pointer is not null
        return current->datum; // return the datum
    else  // error
    {
        cerr << "Attempting to dereference an empty iterator" << endl;
        exit(1);
    }
} // ListIterator::operator*()

// Operator to check if the pointer is not null.
ListIterator::operator bool()
{
    return (current != NULL); // return true if the pointer is not null
} // ListIterator::operator bool()

// Operator to convert to a ListNodePtr.
ListIterator::operator ListNodePtr()
{
    return current; 
} // ListIterator::operator ListNodePtr()

// Swaps the content of the target object with the other iterator if
// they are both not null. If one of them is null the function returns
// false, otherwise true.
bool ListIterator::swap(ListIterator &other)
{
    if (current && other) // if neither node is NULL
    {
        // swap the datums
        int temp = current->datum;
        current->datum = *other;
        *other = temp;

        return true;  // success
    }
    else
        return false; // failure
} // ListIterator::swap()

// Comparison operator.
bool ListIterator::operator==(ListIterator &data)
{
    // just compare the pointers
    return (current == data.current);
} // ListIterator::operator==()



//----------------------
// Begin HW02 Additions
// Author: Dan Cassidy
//----------------------

/****************************** ListIterator::min **************************************************
Action:         Locates the node containing the minimum number in the list starting from the target
                object (*this).  Returns a null iterator if the list is empty.  This is an accessor
                function and does not modify the target object.

Parameters:
IN:             None
OUT:            None

Returns:        ListIterator, pointing to the node which contains the minimum value in the list.

Precondition:   None
***************************************************************************************************/
ListIterator ListIterator::min()
{
    ListIterator smallestFound = *this, currentTarget = *this;
    //ListNode *smallestFound = current, *currentTarget = current;

    //Cycle through the list until the end (NULL) is found.
    while (currentTarget)
    {
        //Compare datums and if currentTarget is smaller, set smallestFound equal to it.
        if (*currentTarget < *smallestFound)
        //if (currentTarget->getDatum() < smallestFound->getDatum())
            smallestFound = currentTarget;

        //Move on to the next Node.
        ++currentTarget;
        //currentTarget = currentTarget->getNext();
    }

    //Return a ListIterator containing the address of the smallest found number in the list.
    return ListIterator(smallestFound);
} // ListIterator::min()

/****************************** ListIterator::moveForward ******************************************
Action:         Moves the target object (*this) ListIterator forward by the desired number of nodes.
                Is set to NULL if the number of steps would push the ListIterator beyond the end of
                the list.  This is a mutator function and changes the target object.

Parameters:
IN:             int steps, containing the number of desired nodes to move forward by.
OUT:            None

Returns:        ListIterator &, containing a reference to the target object (*this) that has been
                moved forward.

Precondition:   None
***************************************************************************************************/
ListIterator &ListIterator::moveForward(int steps)
{
    //Repeatedly advance this iterator to the next node in the list until the requested number of
    //moves has been achieved or the end of the list has been reached.
    for (; steps > 0 && current; --steps)
        current = current->next;

    return *this;
} // ListIterator::moveForward()
