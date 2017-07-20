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
#include <list>
#include <vector>

int main()
{
  list<int> l;
  vector<list<int> > v;
  list<int>::iterator iter;
  int i, j;
  
  for (i=0; i<5; i++)
    v.push_back(l);
  for (i=0; i<5; i++)
    v[i].push_back(i);
  for (i=5; i<10; i++)
    l.push_back(i);
  for (i=10; i<15; i++)
    l.push_front(i);

  iter=l.begin();
  ++iter; 
  *iter=20;
  ++iter;
  --iter;
  l.erase(iter);

  for (int i=0; i<v.size(); i++) {
    for (iter=v[i].begin(); iter != v[i].end(); ++iter)
      cout << *iter << ' ';
    cout << endl;
  }
  for (iter=l.begin(); iter!=l.end(); iter++)
    cout << *iter << ' ';
  cout << endl;
}

/********************** Program output *******************************
0 
1 
2 
3 
4 
14 12 11 10 5 6 7 8 9
**********************************************************************/
