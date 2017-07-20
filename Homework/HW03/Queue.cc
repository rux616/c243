/*********************************************************************
  Author:        Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     Queue.cc
  Last updated:  September 2014
  Description:   Implementation of a class queue derived from a list.
  Note:          This code is copied and pasted from Stack.cc and
                 the appropriate functions and comments have been
                 altered.
**********************************************************************/

#include <iostream>
#include <cstdlib>
using namespace std;

#include "Queue.h"

// Default constructor: create an empty queue.
Queue::Queue() : List() 
{
} // Queue::Queue()

// Insert an object into the queue.
void Queue::enqueue(int data)
{
    insertBack(data);
} // Queue::enqueue()

// Remove the front object and return its value.
int Queue::dequeue()
{
    int result = front();
    removeFront();
    return result;
} // Queue::dequeue() 

// Inspect the value of the front object.
int Queue::front()
{ 
    if (!isEmpty()) 
        return *begin();
    else 
    {
        cerr << "Error: Attempt to access the front of an empty queue.\n";
        exit(1);
    }  
} // Queue::front()

// Delete all the objects in the queue.
void Queue::makeEmpty()
{ 
    List::makeEmpty(); 
} // Queue::makeEmpty()

// Test if the queue is empty.
bool Queue::isEmpty()
{ 
    return List::isEmpty();
} // Queue::isEmpty()

// Concatenate two queues.
void Queue::concatenate(Queue &other)
{
    List::concatenate(other);
} // Queue::concatenate()
