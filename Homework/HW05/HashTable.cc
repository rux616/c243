/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     HashTable.cc
  Last updated:  October 1, 2014
  Description:   Implementation of a hash table.
**********************************************************************/

#include <iostream>
#include <iomanip>
using namespace std;

#include "HashTable.h"

// Constructor with given initial capacity.
HashTable::HashTable(int size)
    : storage(size)
{
    capacity = size;
    numObjects = 0;
} // HashTable::HashTable()

// Destructor: call the function makeEmpty instead, then deallocate
// the memory inside the storage vector
HashTable::~HashTable()
{
    makeEmpty();
    storage.clear();
} // HashTable::~HashTable()

// Delete all the objects without deleting the storage.
void HashTable::makeEmpty()
{
    for(int i=0; i<capacity; i++)
        storage[i].clear(); // delete all the nodes in the list
    numObjects = 0;
} // HashTable::makeEmpty()

// Checks if the access to a given position is safe in the
// storage. This means if the position is smaller than the capacity
// and non negative.
bool HashTable::isValid(int position)
{
    return (position >= 0 && position < capacity);
}

// Prints all the objects in the table.
void HashTable::print()
{
    ListIter iter;
    int i;
    
    cout << "The table has " << numObjects << " objects." << endl;
    if (numObjects) 
    {
        cout << "Here are the objects:" << endl;
        for (i=0; i<capacity; i++)
            if (storage[i].size()) 
                // traverse the list at index i
                for (iter=storage[i].begin(); iter!=storage[i].end(); ++iter)
                    cout << *iter << endl;
    }
} // HashTable::print()

// Increases the capacity of the array up to the given position.
bool HashTable::increase(int position)
{
    if (position >= 0)
        if (position > capacity) 
        {
            ListWordC emptyList;
            for (int i=capacity; i<= position; i++)
                storage.push_back(emptyList);
            capacity = position + 1;
            return true;
        }
    return false; 
} // HashTable::increase()

// This private function inserts an object in the table when we
// already know what position we want it to be inserted at. It is
// declared as private because the user should not be able to call
// it. It is designed to be called internally by the public insertion
// function after getting the position from the hashing table.
bool HashTable::insert(WordCounter &wordc, int position)
{
    if (position >=0) 
    {
        //if (!isValid(position))
        //    increase(position);
        storage[position].push_back(wordc);
        numObjects++;
        return true;
    }
    else
        return false;
} // HashTable::insert()

// This private function removes the word counter object from the list
// at the given position. It is also intended to be called internally
// because public remove function should determine the position based
// on the hash function before it calls this one. They key is used to
// locate the object which is copied to the object wordc before it is
// erased.
bool HashTable::remove(WordCounter &wordc, string k, int position)
{
    if (!isValid(position))
        return false;
    else 
    {
        ListIter iter = storage[position].begin();
        while (iter != storage[position].end() && 
               key(*iter) != k)
            ++iter;
        if (iter == storage[position].end())
            return false;
        else 
        {
            wordc = *iter;
            storage[position].erase(iter);
            numObjects--;
            return true;
        }
    }
} // HashTable::remove()



//----------------------
// Begin HW05 Additions
// Author: Dan Cassidy
//----------------------

// Private functions to be implemented by the student.

/****************************** find ***************************************************************
Action:         Determines if the string k argument passed to it has been used as a key to insert an
                object in the list at the given position.  If yes, then it copies that object into
                the object wordc and returns true.

Parameters:
IN:             string k, holds the string that is being searched for.
IN:             int position, holds the element number in the storage vector where the list to be
                searched is located.
OUT:            WordCounter &wordc, holds a reference to the word counter which will hold the found
                object.  If an object is not found, this variable is unchanged.

Returns:        bool, representing whether string k has been found in this list position.

Precondition:   None
***************************************************************************************************/
bool HashTable::find(WordCounter &wordc, string k, int position)
{
    //Assume this method will not find anything.
    bool found = false;

    //Check to make sure that the position being passed into this method is valid.
    if (isValid(position))
    {
        //Set up a list iterator and point it at the beginning of the list contained in the position
        //element of the storage vector.
        ListIter iter = storage[position].begin();

        //Iterate through the list looking for the string k.
        while (iter != storage[position].end() && key(*iter) != k)
            //If the string is not found at that location, increment the iterator.
            ++iter;

        //Once the loop is done, test the iterator and check if it is at the end of the list.
        if (iter != storage[position].end())
        {
            //If the iterator is not at the end of the list, copy the word counter object that iter
            //references into wordc and set the found flag to true indicating that string k was
            //located.
            wordc = *iter;
            found = true;
        }
    }

    //Return the flag found, be it true or false.
    return found;
} // HashTable::find()

/****************************** hashing ************************************************************
Action:         This function takes a string (a key) and determines the index in the storage array
                an object with that key should go.

Parameters:
IN:             string s, holds the string that is going to be hashed and returned.
OUT:            None

Returns:        int, holding the index in the hash table where the hashed key will be located.

Precondition:   None
***************************************************************************************************/
int HashTable::hashing(string s)
{
    ////HASHING FUNCTION - ADDITION
    //unsigned int hashVal = 0;
    //
    //for (int i = 0; s[i]; ++i)
    //    hashVal += int(s[i]);
    //
    //return hashVal % capacity;


    
    ////HASHING FUNCTION - POLYNOMIAL
    //const int HASH_PRIME = 37;
    //int hashVal = 0;
    //
    //for (int i = 0; s[i]; ++i)
    //    hashVal = (hashVal * HASH_PRIME + int(s[i])) % capacity;
    //
    //return hashVal;



    //HASHING FUNCTION - XOR BIT SHIFT
    unsigned int hashVal = 0;
    
    for (int i = 0; s[i]; ++i)
    {
        unsigned int temp = (hashVal << 5) ^ int(s[i]);
        hashVal = temp ^ hashVal;
    }
    
    return hashVal % capacity;



    /***********************************************************************************************
    Why I Chose What I Chose
    ------------------------
    I used 3 hashing algorithms (as seen above) for the extra credit on this assignment.  I tested
    out a simple addition type, a polynomial type, and an XOR bit shift type.  The numbers I
    report are the total number of collisions for the entire vector, and the maximum number of
    collisions in any given element of the vector.  For a small list (the stopWords file) the
    numbers broke down as follows:
    
          In all instances, there were 319 objects put into 200 elements.
    
                          Total Collisions        Max Collisions
          Addition              182                     6
          Polynomial            157                     5
          XOR Bit Shift         150                     5
    
    For a VERY large list (a list of 319378 words) the numbers broke down as follows:
    
          In all instances, there were 195318 objects put into 200 elements.
    
                          Total Collisions        Max Collisions
          Addition             195118                  1485
          Polynomial           195118                  1081
          XOR Bit Shift        195118                  1066
    
    The difference between the polynomial hashing algorithm and the XOR bit shift algorithm was
    not that substantial, even in the large test set, but the XOR bit shift WAS slightly more
    effective than the polynomial, which is why I chose to use it.
    ***********************************************************************************************/

} // HashTable::hashing()

/****************************** resize *************************************************************
Action:         Checks the load factor of the table and resizes the table if needed.

Parameters:
IN:             None
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/

void HashTable::resize()
{
    unsigned int newSize = 0;

    //Calculates the load factor.
    float loadFactor = float(numObjects) / float(capacity);

    //Checks the load factor against the thresholds for increasing or decreasing the size of the
    //vector, then calculates what the new size should be.
    if (loadFactor >= LF_INCREASE_THRESHOLD)
        newSize = capacity * CHANGE_FACTOR;
    else if (loadFactor <= LF_DECREASE_THRESHOLD)
        newSize = capacity / CHANGE_FACTOR;
    else
        return;

    //Checks to make sure the new size is not less than the default capacity, and not larger than
    //whatever the maximum possible size could be.
    if (newSize > DEF_CAPACITY && newSize < storage.max_size())
    {
        //Create a new temporary vector and copies the old vector into it.
        vector<ListWordC> oldStorage = storage;
        //Copies the old capacity for use with rehashing.
        unsigned int oldCapacity = capacity;
        
        //Clears the storage vector.
        storage.clear();
        //Resizes the storage vector with the new calculated size.
        storage.resize(newSize);
        //Sets the capacity of the hash table to the new size.
        capacity = newSize;
        //Resets the number of objects stored in the hash table.
        numObjects = 0;

        //Loops through the oldStorage vector.
        for (unsigned int i = 0; i < oldCapacity; ++i)
        {
            //Loops through each element (list) of the oldStorage vector.
            ListIter iter = oldStorage[i].begin();
            while (iter != oldStorage[i].end())
            {
                //Takes the object at iter, rehashes it, and reinserts it into the storage vector.
                WordCounter wordc = *iter;
                insert(wordc, hashing(key(wordc)));
                ++iter;
            }
        }
    }
}

// Public functions to be implemented by the student.

/****************************** insert *************************************************************
Action:         Attempts to insert the passed WordCounter argument into the table. Gets the hash
                index for the key of the passed object, then determines if the key has been inserted
                in the list at that index.  If not, the object is inserted in that list and true is
                returned.  If the key has been used in the table, false is returned.

Parameters:
IN:             WordCounter &wordc, contains a reference to the object that this method will attempt
                to insert into the table.
OUT:            None

Returns:        bool, representing whether the insertion was successful or not.

Precondition:   None
***************************************************************************************************/
bool HashTable::insert(WordCounter &wordc)
{
    //Start with the assumption that the string in wordc is not already in the hash table.
    bool inserted = true;

    //Resizes the storage vector if needed due to high or low load factor.
    //resize();

    //Compute the hash of the string in wordc and store it for later use.
    int hashVal = hashing(key(wordc));

    //Check if the word in wordc is already in the table.
    if (find(wordc, key(wordc), hashVal))
        //If it is, then set the inserted flag to false.
        inserted = false;
    else
        //If it is not, insert the wordc object in the list at the element specified by the hash
        //method.
        inserted = insert(wordc, hashVal);

    //Return whether the insertion operation was successful or not.
    return inserted;
} // HashTable::insert()

/****************************** access *************************************************************
Action:         Attempts to get the requested string from the hash table. Gets the hash index for
                the string then finds out if the string has been used as a key to insert an object
                in the list at that index.  If yes, the list object is copied into the wordc object
                and true is returned.  If not, false is returned.

Parameters:
IN:             string s, the string that is requested from the hash table.
OUT:            WordCounter &wordc, will hold the found object (if any).

Returns:        bool, representing whether the requested string was found (true) or not (false).

Precondition:   None
***************************************************************************************************/
bool HashTable::access(WordCounter &wordc, string s)
{
    //Determine whether the string has been used as a key and if it has, find() copies the found
    //object into wordc and returns true, otherwise it returns false.  The find() returned value is
    //passed on as this function's return value as well.
    return find(wordc, s, hashing(s));
} // HashTable::access()

/****************************** remove *************************************************************
Action:         Attempts to remove an object from the hash table.  Gets the index of the string,
                finds out if the string has been used as a key to insert an object into the list at
                the specified index.  If yes, that object is copied into the wordc object, then it
                is removed from the list and true is returned.  If the string is not found in the
                list, false is returned.

Parameters:
IN:             string s, contains the string that will be removed from the table.
OUT:            WordCounter &wordc, contains a reference to the word counter that will be used to
                store the object that is removed.

Returns:        bool, representing whether the word was removed from the list (true) or not (false).

Precondition:   None
***************************************************************************************************/
bool HashTable::remove(WordCounter &wordc, string s)
{
    //Attempt to remove the passed string s from the table using the private remove() method.
    return remove(wordc, s, hashing(s));
} // HashTable::remove()

/****************************** statistic **********************************************************
Action:         Prints the statistic of usage of each index in the table along with some other
                useful information such as the number of objects stored, the capacity, the total
                number of collisions, and max number of collisions for any one hash.

Parameters:
IN:             None
OUT:            None

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void HashTable::statistic()
{
    int CollisionsTotal = 0, CollisionsMax = 0;

    //Show the total number of unique objects and the max capacity of the table.
    cout << "There are a total of " << numObjects
         << " unique objects stored in " << capacity
         << " elements.\n\n";
    
    //Print a list of all the different elements in the hash table and how many objects they each
    //have in them.
    cout << "The distribution of the items in the table are as follows:\n\n";
    for (int i = 0; i < capacity; ++i)
    {
        //Determines the number of local collisions at a given element in the storage vector by
        //getting the size of the list stored there minus 1 (because there has only been a collision
        //if the number of items in a list is 2 or greater).  Also stores the number of localised
        //collisions because it's used multiple times.
        int LocalCollisions = storage[i].size() - 1;

        //Check to see if there were actually any collisions for a particular hash.
        if (LocalCollisions > 0)
        {
            //If there are, bump the total number of collisions up.
            CollisionsTotal += LocalCollisions;

            //Check if this local collision is larger than any others.
            if (LocalCollisions > CollisionsMax)
                //If so, make a record of it.
                CollisionsMax = LocalCollisions;
        }
        //Simply prints out the number of items in a list at a given location.
        cout << LocalCollisions + 1 << ' ';
    }

    //Print out the results of the previous loop: the total number of collisions that happened, and
    //the max number of collisions that happened at any one index.
    cout << "\n\nTotal Number of Collisions: " << CollisionsTotal
         << "\nMax Number of Collisions: " << CollisionsMax
         << "\n\n";
} // HashTable::statistic()
