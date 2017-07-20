/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
  Class:         C243, Spring 2004
  File name:     vector_test.cc
  Last updated:  March 3, 2004.
  Description:   A little program to give an example of how the vector
                 class functions.
**********************************************************************/

#include <iostream>
using namespace std;
#include <vector>

int main()
{
  vector<int> v(21);
  
  v[0]=1;
  v[5]=2;
  v[20]=3;
  v.push_back(4);
  v.push_back(5);
  v.clear();
  cout << v.size() << endl;
  for (int i=0; i<v.size(); i++) 
    cout << v[i] << ' ';
  cout << endl;
}

/********************** Program output *******************************
---- Output without the instruction v.clear() ----
23
1 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 4 5 
**********************************************************************/
