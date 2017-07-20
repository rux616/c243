/********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
  Class:         C243, Spring 2012
  File name:     MyArray.cc
  Last updated:  August 29, 2012.
  Description:   Implementation of a class that implements a safe array.
*********************************************************************/

#include "MyArray.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "HardenedInput.h"
using namespace std;

// Constructor with given size, can be used as default constructor.
MyArray::MyArray(int theSize)
{
    // initialize the object as empty
    array = NULL;
    size = 0;
    // then use the resize function to allocate the memory
    resize(theSize);
} // MyArray::MyArray()

// Destructor. If the array is not empty, it must be deallocated.
MyArray::~MyArray()
{
    // call the function empty instead, so we only need to modify in 
    // one place if anything changes
    empty();
} // MyArray::~MyArray()

// Copy constructor: initializes the target object with the size of
// the object data and copies the content of the object data into the
// target object.
MyArray::MyArray(MyArray &data)
    : size(data.size) // initialize the size
{
    // allocate the array
    array = new int[size];

    // copy all the values from data
    for (int i=0; i<size; i++)
        array[i] = data.array[i];
} // MyArray::MyArray()

// Assignment operator: if the object data is different from the
// target object, initializes the target object with the size of the
// object data and copies the content of the object data.
MyArray &MyArray::operator=(MyArray &data)
{
    // check that we're not trying to copy an object onto itself
    if (this != &data) // different address in the memory
    {
        resize(data.size);
        for (int i=0; i<size; i++)
            array[i] = data.array[i];
    }
    else 
        cout << "Attempt to copy an object on itself. " 
             << "Operation ignored." << endl;
    return *this;
} // MyArray::operator=()

// Deallocate the array and reset the size to 0
void MyArray::empty()
{
    // make sure we have an array to delete
    if (size != 0 && array != NULL) 
    {
        size = 0;
        delete [] array;
        array = NULL; // make the pointer NULL after deallocating 
    }
} // MyArray::empty()

// Resize the array with the new size. If the object contains an
// array, it is deallocated first.
void MyArray::resize(int theSize)
{
    if (theSize >= 0) // check that the new size is valid
    {
        if (size > 0) // if the array is not empty, deallocate first
            empty();
        if (theSize != 0) // allocate again if the size is not 0
        {
            size = theSize;
            array = new int[size];
        }
    }
    else // negative size, give an error message
        cout << "Resize attepmted with a negative size. "
             << "Operation ignored." << endl;
} // MyArray::resize()

// Access an element of the array. If the index (subscript) is out
// of the array, it prints an error message and exits the program.
int &MyArray::operator[](int index)
{
    // check that the index is in the right range
    if (index >= 0 && index < size) 
        return array[index]; 
    else // error
    {
        cerr << "Illegal access to an element of the array." << endl
             << "The size of the array was " << size 
             << " and the index was " << index << endl;
        exit(1);
    }
} // MyArray::operator[]()

// Returns the size of the array.
int MyArray::getSize()
{
    return size;
} // MyArray::getSize()

// Output the elements of the array.
void MyArray::output()
{
    // output the size
    cout << "The array of size " << size 
         << " contains the elements:" << endl;
    // then all the elements
    for (int i=0; i<size; i++)
        cout << array[i] << ' ';
    cout << endl;
} // MyArray::output()

// We define this so that we can write "cout << a;" if a is an object
// of this class. This is not a class method.
ostream &operator<<(ostream &out, MyArray &data)
{
    // output the size
    out << "The array of size " << data.size 
        << " contains the elements:" << endl;
    // then all the elements
    for (int i=0; i<data.size; i++)
        out << data.array[i] << ' ';
    out << endl;
    return out;
} // operator<<()



//----------------------
// Begin HW01 Additions
// Author: Dan Cassidy
//----------------------

/****************************** input **************************************************************
Action:         Inputs the elements of the array from the console.  This function first asks the
                user for the desired size of the array, then asks the user to input the elements.

Parameters:
IN:             None
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void MyArray::input()
{
    //Some variables to facilitate using the hardenedInput function.
    int tempSize = 0, tempElement = 0;
    bool messageOnError = true;
    char message[] = "Invalid input.  Please try again: ";

    //Get the desired size of the array.
    cout << "Please enter the desired size of the array: ";
    hardenedInput(tempSize, messageOnError, message);
    resize(tempSize);

    //Cycle through the array, asking for the elements.
    for (int i = 0; i < size; ++i)
    {
        cout << "Please enter element " << i + 1 << " of " << size << ": ";
        hardenedInput(tempElement, messageOnError, message);
        array[i] = tempElement;
    }
} //void MyArray::input()

/****************************** randomize **********************************************************
Action:         Initializes the elements of the array with random values between 0 (inclusive) and a
                given maximum limit (also inclusive).  The intended size of the array is provided by
                the second parameter, theSize.  If this is 0 (default), the array will not be
                resized and will just have its elements randomized.

Parameters:
IN:             int limit, holds the maximum number that will be generated to fill the array.
                Default value is 100.
IN:             int theSize, holds the size of the array to be generated.  Default value is 0.
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void MyArray::randomize(int limit, int theSize)
{
    //Use a static variable to keep track of whether rand() has already been seeded
    static bool randInit = false;
    if (!randInit)
    {
        //If rand() has not been seeded, do so using the current time.
        srand(time(NULL));
        randInit = true;
    }

    //If theSize is not 0, call the resize() method to resize the array.
    if (theSize)
        resize(theSize);

    //Cycle through the array and randomize the elements from 0 (inclusive) to limit (inclusive).
    for (int i = 0; i < size; ++i)
        array[i] = rand() % (limit + 1);
} //void MyArray::randomize(int limit, int theSize)

/****************************** operator== *********************************************************
Action:         Compares one MyArray object with another and decide if they are equal. Objects of
                differing sizes are always considered not equal.

Parameters:
IN:             MyArray &data, contains a reference to the MyArray object being compared against.
OUT:            None

Returns:        bool, representing whether the calling object and the other object are equal.

Precondition:   None
***************************************************************************************************/
bool MyArray::operator==(MyArray &data) const
{
    //Declare a flag to keep track of whether the arrays match.  Assume that they do.
    bool arraysMatch = true;
    
    //Check to make sure the sizes match.  If they do not, set the arraysMatch flag to false.  This
    //will prevent the following for loop from triggering.
    if (size != data.size)
        arraysMatch = false;

    //Cycle through the elements of the arrays and test if they match.  If they do not, set the
    //arraysMatch flag to false, which will exit the for loop.
    for (int i = 0; i < size && arraysMatch; ++i)
        if (array[i] != data.array[i])
            arraysMatch = false;

    //Return the arraysMatch flag
    return arraysMatch;
} //bool MyArray::operator==(MyArray &data) const

/****************************** operator!= *********************************************************
Action:         Compares one MyArray object with another and decide if they are not equal. Objects
                of differing sizes are always considered not equal.

Parameters:
IN:             MyArray &data, contains a reference to the MyArray object being compared against.
OUT:            None

Returns:        bool, representing whether the calling object and the other object are not equal.

Precondition:   None
***************************************************************************************************/
bool MyArray::operator!=(MyArray &data) const
{
    //Returns the opposite of the operator== method.
    return !(*this == data);
} //bool MyArray::operator!=(MyArray &data) const

/****************************** hamming ************************************************************
Action:         Computes the Hamming distance between two MyArray objects.

Parameters:
IN:             MyArray &data, contains a reference to the MyArray object for which the Hamming
                distance is being computed against.
OUT:            None

Returns:        int representing the computed Hamming distance.

Precondition:   None
***************************************************************************************************/
int MyArray::hamming(MyArray &data) const
{
    //Declare a variable to hold the Hamming distance counter and set it to 0.
    int hammingDistance = 0;

    //Declare variables to hold object-independent sizes.
    int smallerArraySize = (size < data.size ? size : data.size);
    int largerArraySize = (size > data.size ? size : data.size);

    //Cycle through the arrays and increment the Hamming distance counter when appropriate.
    for (int i = 0; i < smallerArraySize; ++i)
        if (array[i] != data.array[i])
            ++hammingDistance;

    //Return the Hamming distance counter plus the difference in array sizes.
    return hammingDistance + largerArraySize - smallerArraySize;
} //int MyArray::hamming(MyArray &data) const
