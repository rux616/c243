/*************************************************************
  Author:       Dana Vrajitoru, IUSB, CS
                Dan Cassidy, IUSB, Student
  Class:        C243 Data Structures
  Date:         October 1, 2014
  File:         main.cc                             
  Description:  A test program for the Porter transformation. 
                It reads one word at a time from the console 
                and outputs the transformed word until the 
                input ends by an eof - a CTRL-D under a Linux 
                terminal or the end of the file in case or 
                redirection.
***************************************************************/

#include <fstream>
#include <iostream>
using namespace std;
#include <cstdlib>
#include "porter.h"
#include "HashTable.h"

void readStopList(HashTable &stopList);
void indexWords(HashTable &stopList, HashTable &indexing);
void increment(HashTable &indexing, string word);

// Main function: create the stopList, call the function reading the
// words and indexing them, and then printing out the result.
int main()
{
    HashTable stopList, indexing;
    readStopList(stopList);
    stopList.statistic();
    indexWords(stopList, indexing);
    indexing.statistic();
    cout << "Here is the indexing of your text" << endl;
    //indexing.print();
    return 0;
} // main()

// Inputs the stopList and stores it in the table.
void readStopList(HashTable &stopList)
{
    char word[KEYWORDSIZE];
    ifstream fin("stopWords");
    if (!fin.good())
    {
        cout << "error reading the stop list file" << endl;
        exit(1);
    }
    WordCounter wordc;
    do 
    {
        fin >> word;
        if (!fin.eof()) 
        {
            wordc.setWord(word);
            stopList.insert(wordc);
        }
    } while (!fin.eof());
    fin.close();
} // readStopList()

/****************************** indexWords *********************************************************
Action:         Reads a word at a time from the console, verifies that it's not in the stopList, and
                if it's not, it applies the Porter transform to it, then it indexes it in the table
                indexing.

Parameters:
IN:             HashTable &stopList, holds a reference to the stopList table, which contains a list
                of words that will not be added to the indexing table.
OUT:            HashTable &indexing, holds a reference to the main table used to index any entered
                words.

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void indexWords(HashTable &stopList, HashTable &indexing)
{
    char word[KEYWORDSIZE];
    WordCounter wordc;
    cout << "Enter words separated by spaces or new lines.\n"
         << "The program will output the Porter transform of the words.\n"
         << "End the input with Ctrl-D." << endl;
    while (!cin.eof()) 
    {
        cin >> word;
        if (!cin.eof()) 
        {
            toLowerCase(word);
            clean(word);
            //Check the table stopList to make sure the word is not there before applying the Porter
            //transformation and adding it to the the indexing hash table.
            if (!stopList.access(wordc, word))
            {
                stripAffixes(word);
                increment(indexing, word);
            }
        }
    }
    cout << endl;
} // indexWords()

/****************************** increment **********************************************************
Action:         Increments the indexing of the given word in the table.  This means that if the word
                can't be found in the table, then it will be inserted with a count of 1.  Otherwise
                the word will be removed from the table, the count will be incremented, and the word
                will be reinserted with the new count.

Parameters:
IN/OUT:         HashTable &indexing, holds a reference to the hash table which will be looked at to
                find the word.  Regardless of whether the word is found, the table will be updated.
IN:             string word, contains the word that will be searched for and incremented if found or
                inserted with a count of 1 if not.

Returns:        Nothing

Precondition:   None
***************************************************************************************************/
void increment(HashTable &indexing, string word)
{
    //Declare a new WordCounter and set it equal to the passed in word.
    WordCounter wordc = word;

    //Attempt remove word from the table.
    if (indexing.remove(wordc, word))
        //If word was found in the table, increment its count of the word.
        wordc.increment();

    //Insert wordc into the hash table.  If it was found previously, it will have a count > 1, if
    //not, it will be inserted for the first time with a count of 1.
    indexing.insert(wordc);
} // increment()
