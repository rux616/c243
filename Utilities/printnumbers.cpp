#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage: pn <mode> <quantity> <lowerbound> <upperbound>\n"
             << "  <mode> can be 1 for random, 2 for sequential, 3 for constant,\n"
             << "  or 4 for sorted.  If mode is 3, <lowerbound> is used as the\n"
             << "  constant\n";
        exit(1);
    }
    
    int mode = atoi(argv[1]);
    unsigned long size = atoi(argv[2]);
    int lowerBound = atoi(argv[3]);
    int upperBound = atoi(argv[4]);
    
	srand(time(NULL));
    
	cout << size << '\n';
	
    vector<int> myVector;
    
    for (unsigned long i = 0; i < size; ++i)
    {
		if (mode == 1)
			cout << rand() % (abs(upperBound - lowerBound) + 1) - lowerBound << ' ';
		else if (mode == 2)
			cout << i << ' ';
        else if (mode == 3)
            cout << lowerBound << ' ';
        else
            myVector.push_back(rand() % (abs(upperBound - lowerBound) + 1) - lowerBound);
    }
        
    if (mode == 4)
    {
        sort(myVector.begin(), myVector.end());
        for (unsigned long i = 0; i < size; ++i)
            cout << myVector[i] << ' ';
    }
            
    return 0;
}