/********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
  Class:         C243, Spring 2012
  File name:     main.cc
  Last updated:  August 26, 2013.
  Description:   Testing the class MyArray.
*********************************************************************/

#include "MyArray.h"
#include <iostream>
using namespace std;

// main function
int main()
{
    //Original code, commenting out per HW01 instructions.
    /*
    // create an object of type MyArray with a capacity of 10
    MyArray a(10);

    // initialize all its elements with the index
    for (int i=0; i<a.getSize(); i++)
        a[i] = i;

    // testing the output operator for the entire array
    cout << a;

    // testing the access to an index outside of the array range
    cout << a[15] << endl;
    */



    //----------------------
    // Begin HW01 Additions
    // Author: Dan Cassidy
    //----------------------

    const int RAND_LIMIT = 10;
    MyArray a, b, c;

    //Testing new MyArray functions as specified.
    a.input();
    b.randomize(RAND_LIMIT, a.getSize());
    c.randomize(RAND_LIMIT, a.getSize());
    cout << "a == b: " << (a == b ? "true" : "false") << "\n";
    cout << "a != b: " << (a != b ? "true" : "false") << "\n";
    cout << "The Hamming distance between MyArray 'b' and MyArray 'c' is " << b.hamming(c) << ".\n";

    return 0;
} // main()

/*************** Example of the execution ***************************
The array of size 10contains the elements:
0 1 2 3 4 5 6 7 8 9 
Illegal access to an element of the array.
The size of the array was 10 and the index was 15
*********************************************************************/
