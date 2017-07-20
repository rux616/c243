/********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
  Class:         C243, Spring 2012
  File name:     MyArray.h
  Last updated:  August 29, 2012.
  Description:   Definition of a class that implements a safe array.
*********************************************************************/

#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <iostream>
using namespace std;

class MyArray 
{
protected:
    int size;
    int *array;

public:
    // Constructor with given size, can be used as default constructor.
    MyArray(int theSize = 0);

    // Destructor. If the array is not empty, it must be deallocated.
    ~MyArray();

    // Copy constructor
    MyArray(MyArray &data);

    // Assignment operator
    MyArray &operator=(MyArray &data);

    // Deletes the array and sets the size to 0.
    void empty();

    // Resize the array.
    void resize(int theSize = 0);

    // Access an element of the array. If the index (subscript) is out
    // of the array, it prints an error message and exits the program.
    int &operator[](int index);

    // Returns the size of the array.
    int getSize();

    // Output the elements of the array.
    void output();

    // We define this so that we can write "cout << a;" if a is an
    // object of this class.
    friend ostream &operator<<(ostream &out, MyArray &data);
    //friend istream &operator>>(istream &in, MyArray &data);



    //----------------------
    // Begin HW01 Additions
    // Author: Dan Cassidy
    //----------------------

    //Input the elements of the array from the console.
    void input();

    //Initialize the elements of the array with random values between 0 and a given maximum limit.
    void randomize(int limit = 100, int theSize = 0);

    //Compare the elements of the array with another one and decide if they are equal.
    bool operator==(MyArray &data) const;

    //Compare the elements of the array with another one and decide if they are not equal.
    bool operator!=(MyArray &data) const;

    //Compute the Hamming distance between two arrays.
    int hamming(MyArray &data) const;
}; // class MyArray

#endif
