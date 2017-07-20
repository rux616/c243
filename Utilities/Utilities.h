#pragma once

//Binary search function written by Drs. Knight and Vrajitoru.  Searches through <a>
//in a binary way for <target>.
void binarySearch(const int a[], const int &target, int first,
                  int last, bool &found, int &subscript);

//Reads a character that is in the character array allowed.  If the input is not allowed, the user
//is asked to enter another character.  If allowed is "" (default), any character is accepted.
void getChar(char &c, const char allowed[] = "");

//Template function.  Utilizes 'cin >>' to get read user input into the passed in
//variable.  Will ask the user to reinput a detected bad input and returns true or
//false based on whether the input was good(true) or EOF(false).
template <class T> bool getInput(T &data);

//Tests an array to see if it is sorted sequentually from smallest to largest.
bool isSorted(int array[], unsigned long size, bool showOutput = true);

//Determines how many digits are in a number.
unsigned int numDigits(int a);

//A quick swap function specifically made for arrays.
void swap(int a[], int i, int j);

//Fills <size> elements of <a> with random numbers in the range <lowerBound>
//to <upperBound>, both inclusive.
void randomize(int a[], unsigned long size, int lowerBound = 0, int upperBound = 99);

//Dr.Knight's and Dr. Vrajitoru's testAllocation function.  Tests a pointer of any
//type and displays an error message then exits the program if the pointer is null.
void testAllocation(void *p);
