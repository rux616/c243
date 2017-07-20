#pragma once

//Utilizes 'cin >>' to get read user input into the passed in variable.  Will ask the
//user to reinput a detected bad input and returns true or false based on whether the
//input was good(true) or EOF(false).
bool getInput(int &data);

//Dr.Knight's and Dr. Vrajitoru's testAllocation function.  Tests a pointer of any
//type and displays an error message then exits the program if the pointer is null.
void testAllocation(void *p);
