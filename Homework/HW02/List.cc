/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     List.cc
  Last updated:  September 11, 2014
  Description:   Implementation of a container class for a list.

**********************************************************************/

#include <iostream>
using namespace std;

#include "general.h"
#include "List.h"

// Default constructor.
List::List()
    : head(NULL), tail(NULL), size(0) // empty list
{
} // List::List()

// Copy constructor. The target object should be assigned a hard copy
// of the list contained in the data object.
List::List(const List &data)
    : head(NULL), tail(NULL), size(0) // start with an empty list
{
    *this = data; // then use the operator= form this class
} // List::List()

// Destructor.
List::~List()
{
    // call the function makeEmpty instead so that we only have to
    // change things in one place
    makeEmpty();
} // List::~List()

// Assignment operator. It must also make a hard copy of the
// list. Make sure that the tail of the target object is also set
// correctly.
List &List::operator=(const List &data)
{
    if (this != &data) // check that we're not assigning the object to itself
    {
        size = data.size;
        head = data.head->copy(); // make a copy of the list instead of 
                                  // of assigning just the pointer 
        tail = head->lastNode();  // now we have to find the last node
                                  // because we've copied the list
    }

    return *this; 
} // List::operator=()

// Returns the size of the list.
int List::getSize()
{
    return size;
} // List::getSize()

// Inserts a new node with the specified content at the front of the
// list. It must make sure to update both the head and the tail of the
// list if need be.
void List::insertFront(int number)
{
    // create a node with the data and pointing towards the head
    ListNodePtr newNode = new ListNode(number, head);
    testAllocation(newNode);

    size++; // we're adding a node

    head = newNode; // this one becomes the head of the list

    if (size == 1)      // if it was the first one
        tail = newNode; // then it's also the tail
} // List::insertFront()

// Removes the node in the list after the one pointed to by the
// current in the iterator. If the iterator contains a NULL pointer,
// it removes the front node. It returns false if the list was empty
// and no node was removed.
bool List::removeNodeAfter(ListIterator iter)
{
    if (!iter) // current in iter is NULL
        return removeFront();
    else 
    {
        if (!iter.current->next) // iter is the last node
            return false; // nothing to remove in this case
        else
        {
            ListNodePtr temp = iter.current->next; // the node to delete
            iter.current->next = iter.current->next->next; // adjust links
            delete temp;
            size--;
            if (!iter.current->next) // if the node we deleted was the last one
                tail = iter.current; // then we have a new tail.
            return true;
        }
    }
} // List::removeNodeAfter()

// Checks if the list is empty.
bool List::isEmpty()
{
    return (size == 0); // we can check the size or the pointers
} // List::isEmpty()

// Converts to boolean. It must return true if the list is not empty,
// and false if it is.
List::operator bool()
{
    return !isEmpty(); // return true if the list is not empty
} // List::operator bool()

// Deletes the entire list and reset all the data.
void List::makeEmpty()
{
    if (size) // if the list is not empty
    {
        deleteList(head); // call the function from ListNode.cc
        head = NULL; // then reset the data
        tail = NULL;
        size = 0;
    }
} // List::makeEmpty()

// Difference operator - it must compare the content of the two lists
// and not just the pointers to them.
bool List::operator!=(const List &data)
{
    return !(*this == data); // call the operator== on the object,
                             // not on the pointer
} // List::operator!=()

// Concatenates the second list to the first and empties the second
// list.
void List::concatenate(List &data)
{
    if (head != data.head) // if the two lists are not the same
    {
        if (head) // if the first list is not empty
        {
            // call the ListNode function to add the second list 
            // to the tail of the first
            tail->concatenate(data.head); 
            size += data.size; // we also have to update the size
            if (data.tail)     // and the tail
                tail = data.tail;
        }
        else // if the first list is empty
        {
            // move the list from the second one into it
            head = data.head;
            size = data.size;
            tail = data.tail;
        }
        if (data) // if the second list was not empty, unlink/empty it
        {
            data.size = 0;
            data.head=NULL;
            data.tail=NULL;
        }
    }
    else // error
        cout << "Attepmt to concatenate a list to itself. "
             << "Operation ignored." << endl;
} // List::concatenate()
  
// Prints the last node in the list: the tail.
void List::printLast()
{
    if (tail) // check that we have a last node
        cout << "The last node in the list contains " 
             << tail->datum << endl;
    else
        cout << "The list is empty, it has no last node" << endl;
} // List::printLast()

/*******************************************************************/
// Functions using list iterators.

// Begining of the list returned as a list iterator.
ListIterator List::begin()
{
    return ListIterator(head); 
} // 

// End of the list returned as a list iterator.
ListIterator List::end()
{
    return ListIterator(tail);
} // List::end()

// Prints the list. We also want to know how many node are there in
// the list and if the list is empty.  I have rewriten this function
// using list iterators.
void List::print()
{
    // print the count first
    cout << "The list has " << size << " nodes." << endl;

    if (size)   // if the list is not empty
    {
        // print all the nodes using an iterator
        cout << "Here are the nodes of the list:" << endl;
        ListIterator iter(head); //start from the head
        while (iter) // while we haven't reached the end
        {
            cout << *iter << ' '; // print the datum
            ++iter; // move to the next node
        }
        cout << endl;
    }
} // List::print()

// Finds the given number in the list and return a list iterator
// containing a pointer to the node containing the number in
// question. If the number is not in the list, it returns an iterator   
// where the pointer current is NULL.
ListIterator List::locate(int number)
{
    ListIterator locator = begin(); // start form the head
    while (locator && *locator != number) // while we haven't reached the end
                                          // nor found the target
        ++locator; // move to the next node
    return locator; // return the iterator containing the target
                    // or containing NULL if it wasn't there
} // List::locate()

// The bubble sort using list iterators.
void List::bubbleSort()
{
    if (head) // if the list is not empty
    {
        bool ordered = false;
        ListIterator iter1, iter2; // need two iterators
        while (!ordered) // while the list is not sorted
        {
            ordered = true; // we'll assume it is sorted
            iter1 = head;   // start form the head with one
            iter2 = head->next; // and the next with the other
            while (iter2) 
            {
                if (*iter1 > *iter2) // if two nodes are out of order
                {
                    iter1.swap(iter2); // swap their datums
                    ordered = false;   // and set the flag
                }
                ++iter1; // move both iterators at the same time
                ++iter2; // so they will allways be adjacent nodes
            }
        }
    }
} // List::bubbleSort()



//----------------------
// Begin HW02 Additions
// Author: Dan Cassidy
//----------------------

/****************************** operator== *********************************************************
Action:         Compares the content of two lists.  Does short-circuit evaluation by comparing the
                sizes of the two lists and the address of the head nodes and if the sizes are not
                equal or the addresses of the head nodes are the same, this function does not waste
                time comparing the contents of the lists.

Parameters:
IN:             const List &data, holds the second list to compare.
OUT:            None

Returns:        bool describing whether the two compared lists are equal or not.

Precondition:   None
***************************************************************************************************/
bool List::operator==(const List &data)
{
    //Assume the lists are equal.
    bool listsAreEqual = true;

    //If the lists have unequal size, they are not equal, and we have no need to compare.
    if (size != data.size)
        listsAreEqual = false;

    //If both lists start at the same node, then they are equal, and we have no need to compare.
    else if (head == data.head);

    //The head nodes are not the same, but the sizes are equal.  Compare the datum of the lists.
    else
    {
        ListIterator currentNode1 = head, currentNode2 = data.head;

        //Iterate through the lists and check the datums.
        while (listsAreEqual && currentNode1)
        {
            //Compare the datums.
            if (*currentNode1 != *currentNode2)
                listsAreEqual = false;

            //Go to the next node.
            ++currentNode1;
            ++currentNode2;
        }
    }

    //Return the result of the function.
    return listsAreEqual;
} // List::operator==()


/****************************** List::insertBack ***************************************************
Action:         Inserts a new node with the specified content at the back of the list.  Simply calls
                the insertAfter function using the tail of the list as the node to insert after.

Parameters:
IN:             int number, contains the number to be added to the list.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void List::insertBack(int number)
{
    //Make things simple and call insertAfter using the tail as the base node.
    insertAfter(tail, number);
} // List::insertBack()

/****************************** List::insertAfter **************************************************
Action:         Inserts a new node after the specified node.  If the chosen node is NULL, then it
                inserts the node at the front of the list and calls the insertFront function to do
                so.  Updates the head, tail, and size attributes appropriately, as needed.

Parameters:
IN:             ListIterator chosenNode, contains the location of the node that the new node will be
                inserted after.
IN:             int number, contains the number which will be added as a new node.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void List::insertAfter(ListIterator chosenNode, int number)
{
    //If the iterator is NULL, insert at the head of the list.
    if (!chosenNode)
        insertFront(number);
    
    //If the iterator is not null, insert where it points.
    else
    {
        //Allocate memory for a new List Node and check it for validity.
        ListNode *newNode = new ListNode(number);
        testAllocation(newNode);

        //Shuffle pointers around to insert the new node into the list.
        newNode->next = chosenNode.current->next;
        chosenNode.current->next = newNode;

        //Check if the new node is pointing to NULL and if it is, make it the new tail.
        if (!newNode->next)
            tail = newNode;
        
        //Increment size.
        ++size;
    }
} // List::insertAfter()

/****************************** List::isOrdered ****************************************************
Action:         Checks if the list is in ascending order.  If the list is empty or contains only a
                single node, this function will always be true.

Parameters:
IN:             None
OUT:            None

Returns:        bool, detailing whether the list is ordered (true) or not (false).

Precondition:   None
***************************************************************************************************/
bool List::isOrdered()
{
    ListIterator currentNode = head;
    int lastValue = 0;
    bool isOrdered = true;

    //If the list is not empty, set the starting value to the value of the head node.
    if (size)
        lastValue = *currentNode;

    //Advance to the next node in the list.
    ++currentNode;

    //Iterate through the list while isOrdered is true and iter1 isn't NULL.
    while (isOrdered && currentNode)
    {
        //Check the value of the current node and compare it to the last value.
        if (*currentNode < lastValue)
            isOrdered = false;

        //Update the last value.
        lastValue = *currentNode;

        //Advance to the next node.
        ++currentNode;
    }

    return isOrdered;
} // List::isOrdered()

/****************************** List::countNegative ************************************************
Action:         Counts the number of negative values in the list.  If the list is empty then there
                can not possibly any negative values in it, so this function simply returns 0 in
                that case.

Parameters:
IN:             None
OUT:            None

Returns:        int, containing the number of negative values in the list.

Precondition:   None
***************************************************************************************************/
int List::countNegative()
{
    ListIterator currentNode = head;
    int numNegatives = 0;

    //Iterate through the list until the end (NULL) is reached.
    while (currentNode)
    {
        //Compare the value in the current node and see if it is less than 0.
        if (*currentNode < 0)
            //If it is, increment the counter.
            ++numNegatives;

        //Advance to the next node in the list.
        ++currentNode;
    }

    return numNegatives;
} // List::countNegative()

/****************************** List::removeFront **************************************************
Action:         Removes the front node from the list and then delets said node.  Returns true if the
                operation was successful, and false if it was not (e.g. the list was already empty).
                This function also updates the head, tail, and size attributes as necessary.

Parameters:
IN:             None
OUT:            None

Returns:        bool, indicating whether the operation was successful or not.

Precondition:   None
***************************************************************************************************/
bool List::removeFront()
{
    bool operationSuccessful = false;

    //Make sure the list is not empty.
    if (size)
    {
        //Store the current head of the list so it doesn't get lost.
        ListNode *temp = head;

        //Point the head of the list at the next node.
        head = head->next;

        //Delete the former head node of the list.
        delete temp;

        //Decrement the size.
        --size;

        //If the new size is 0, then we just deleted the last node in the list and need to update
        //tail to reflect that fact.
        if (!size)
            tail = NULL;

        //Operation was successful.
        operationSuccessful = true;
    }

    return operationSuccessful;
} // List::removeFront()

/****************************** List::sum **********************************************************
Action:         Computes the sum of all the numbers stored in the lists.  Returns 0 if the list is
                empty.

Parameters:
IN:             None
OUT:            None

Returns:        int, containing the computed sum.

Precondition:   None
***************************************************************************************************/
int List::sum()
{
    ListIterator currentNode = head;
    int sum = 0;

    //While we have not reached the end of the list, add the value of the
    //current node to sum and cycle to the next node.
    while (currentNode)
    {
        sum += *currentNode;
        ++currentNode;
    }

    return sum;
} // List::sum()

/****************************** List::nodeAtPosition ***********************************************
Action:         Locates the requested zero-based element number of the list and returns a
                ListIterator pointing to such.  If the list doesn't have the requested number of
                nodes (+1), then it will return a NULL ListIterator.  Utilizes the
                ListIterator::moveForward function.

Parameters:
IN:             int location, contains the zero-based element number that is requested.
OUT:            None

Returns:        ListIterator, pointing to the requested zero-based list element.

Precondition:   None
***************************************************************************************************/
ListIterator List::nodeAtPosition(int location)
{
    //Start at the head of the list.
    ListIterator currentNode = head;

    //Move forward however many nodes desired.
    currentNode.moveForward(location);

    return currentNode;
} // List::nodeAtPosition()

/****************************** List::selectionSort ************************************************
Action:         Implements a selection sort for the list.

Parameters:
IN:             None
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void List::selectionSort()
{
    //Only bother sorting if list has at least 2 nodes.
    if (size >= 2)
    {
        //Set the starting point for the sort.
        ListIterator currentNode = head, minimumNode = head;

        //Repeatedly interate through the list until the tail is reached.
        while (!(currentNode == tail))
        {
            //Find the minimum element in the remainder of the list.
            minimumNode = currentNode.min();

            //Check if the found minimum is different than the current node, and swap it if it is.
            if (!(currentNode == minimumNode))
                currentNode.swap(minimumNode);
            
            //Advance to the next node in the list.
            ++currentNode;
        }
    }
} // List::selectionSort()

/****************************** List::positionAtNode ***********************************************
Action:         Searches for a given ListIterator in a list.  Returns -1 if the ListIterator is not
                in the list for any reason, including if the list is empty, the ListIterator being
                searched for is NULL, or it simply isn't in the list.

Parameters:
IN:             ListIterator target, contains the node being looked for by this function.
OUT:            None

Returns:        int, containing the zero-based element number of found node.  If not found, will be
                -1.

Precondition:   None
***************************************************************************************************/
int List::positionAtNode(ListIterator target)
{
    ListIterator currentNode = head;
    int currentPosition = 0, foundPosition = -1;

    //Verify that the target is not NULL and the list is not empty because if either is then there
    //is no need to search.
    if (target && currentNode)
    {
        //Iterate through the list in search of the target.
        while (foundPosition == -1 && currentNode)
        {
            //Check to see if the current node is the target node.
            if (currentNode == target)
                foundPosition = currentPosition;

            //If it is not, move to the next node and increment the position counter.
            else
            {
                ++currentPosition;
                ++currentNode;
            }
        }
    }

    return foundPosition;
} // List::positionAtNode()
