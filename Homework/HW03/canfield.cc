/*********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
                 Dan Cassidy, IUSB, Student
  Class:         C243 Data Structures
  File name:     canfield.cc
  Last updated:  September 2014
  Description:   Implementation of the card game of Canfield using 
                 stacks and queues.
**********************************************************************/

#include "Queue.h"
#include "Stack.h"
#include "canfield.h"
#include "general.h"
#include <iostream>
#include <iomanip>
using namespace std;
#include <ctime>
#include <cstdlib>

// It's easier to keep the game data global.
Queue deck;
Stack reserve;
Stack foundation[NUM_SUITS], tableau[NUM_SUITS];
int waste;
bool quit = false, playerHasWon = false;
char from, to;
int fromId, toId;
int score = 0;

// The main loop of the game.
void canfield()
{
    char answer;
    srand(time(NULL));
    newGame();
    printGame();
    while (!quit) 
    {
        printMenu();
        cin >> answer;
        performAction(answer);
    }
} // canfield()

// Swap two integers
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
} // swap

// Starting a new game. It must reset all the data structures,
// generate a new set of 52 random cards and place them in the
// appropriate places in the game.
void newGame()
{
    int i, card;
    int cardsLeft = NUM_CARDS;
    int newDeck[NUM_CARDS];

    // Empty all the data structures
    emptyAll();

    //Reset the score.
    score = 0;

    //Reset the flag that declares whether the player has won the game or not.
    playerHasWon = false;

    // Create a new deck and shuffle it
    newShuffledPack(newDeck);

    // Now place the cards in the data structures
    for (i=0; i<NUM_RANKS; i++)      // fill in the reserve with 13 cards
    {
        card = newDeck[--cardsLeft]; // use a card and decrease the total count
        reserve.push(card);
    }
    for (i=0; i<NUM_SUITS; i++)      // draw a card for each tableau
    {
        card = newDeck[--cardsLeft]; // use a card and decrease the
                                     // total count
        tableau[i].push(card);
    }
    for (i=0; i<NUM_SUITS; i++)      // Initialize the foundations with 0
        foundation[i].push(0);
    waste = newDeck[--cardsLeft];    // the card to play
    while (cardsLeft > 0)            // the remaining cards go into the deck
    {
        card = newDeck[--cardsLeft];
        deck.enqueue(card);
    }
} // newGame()

// Create a new pack of cards and shuffles them
void newShuffledPack(int *newDeck)
{
    int i, j;
    for (i=0; i<NUM_CARDS; i++)
        newDeck[i] = 1+i % NUM_RANKS; // add the cards in order
    for (i=0; i<NUM_CARDS-1; i++)     // then shuffle them
    {
        j = i+rand() % (NUM_CARDS-i); // random other card
        swap(newDeck[i], newDeck[j]); // swap them
    }
} // newShuffledPack()

// Empty all the data structures
void emptyAll()
{
    int i;
    deck.makeEmpty();
    reserve.makeEmpty();
    for (i=0; i<NUM_SUITS; i++) 
    {
        foundation[i].makeEmpty();
        tableau[i].makeEmpty();
    }
} // emptyAll()

// Prints the choices of actions for the player.
void printMenu()
{
    cout << "[q] quit [N] new game";
    if (!playerHasWon)
        cout << " [m] move [d] deal a card [r] repeat previous action";
    cout << "\nEnter your choice: ";
} // printMenu()

// Performs an action based on the user's choice.
void performAction(char answer)
{
    switch (answer) 
    {
        case 'q':
        case 'Q':
            quit = true;
            break;
            //case 'n': this causes problems because it's next to m
        case 'N':
            newGame();
            break;
        case 'm':
        case 'M':
            if (!playerHasWon)
                move();
            break;
        case 'd':
        case 'D':
            if (!playerHasWon)
                deal();
            break;
        case 'r':
        case 'R':
            //Repeat the last move the user made, if possible.
            if (!playerHasWon)
                move(true);
            break;
    }
    if (!quit)
        printGame();
} // performAction()

// Displays the entire content of the game.
void printGame()
{
    clearScreen();
    cout << "Deck:    ";
    if (waste != -1)
        cout << waste;
    cout << endl << endl
         << "Reserve: ";
    if (!reserve.isEmpty())
        cout << reserve.top();
    cout << endl << endl;
    for (int i=0; i<NUM_SUITS; i++) 
    {
        cout << "F" << i << "      ";
        if (foundation[i].top() > 0)
            cout << setw(2) << foundation[i].top() << "   ";
        else
            cout << "     ";
        cout <<"T" << i << ' ';
        tableau[i].print();
        cout << endl << endl;
    }

    //Display the current score.
    cout << "Score: " << score << "\n\n";

    if (playerHasWon)
        cout << "Congratulations!  You have won the game!\n\n";
} //  printGame()

// Deals one card from the deck.
void deal()
{
    if (!deck.isEmpty()) 
    {
        deck.enqueue(waste);
        waste = deck.dequeue();
    }
} // deal()

// Performs one move.
void move(bool repeatLastMove)
{
    int card;

    //Add a conditional that lets us repeat a move if desired by bypassing the user input.
    if (!repeatLastMove)
    {
        cout << "From:" << endl
            << "[t] top of the tableau [T] whole tableau [d] deck [r] reserve : ";
        cin >> from;
        if (from == 't' || from == 'T')
        {
            cout << "number : ";
            getNumber(fromId, NUM_SUITS);
        }
        cout << "To: [t] tableau [f] foundation : ";
        cin >> to;
        cout << "number : ";
        getNumber(toId, NUM_SUITS);
    }
    card = getCard();
    cout << "card to move: " << card << endl;
    if (card != -1 && checkMove(card)) 
    {
        do
        {
            removeCard();
            addCard(card);

            //Check if the score needs to be updated and do so.
            updateScore();
            
            //Update 'card' on every loop iteration if a whole tableau is being moved to a
            //foundation.
            if ((to == 'f' || to == 'F') && from == 'T')
                card = getCard();

        //Only loop if the player is attempting to move an entire tableau to a foundation, and if
        //the next move is valid.
        } while ((to == 'f' || to == 'F') && from == 'T' && checkMove(card));

        //If a card was moved from a tableau, and that tableau is now empty, automatically move a
        //card into that tableau from the reserve if there are any cards left in it.
        if ((from == 't' || from == 'T') && tableau[fromId].isEmpty() && !reserve.isEmpty())
            tableau[fromId].push(reserve.pop());
    }
    else 
    {
        cout << "Movement not permitted. Press <Enter> to continue."; 
        cin.get();
        while (cin.get() != '\n') ; // Clear the line
    }
} // move()

// Checks if the proposed movement is possible.
bool checkMove(int card)
{
    if (to == 't' || to == 'T') 
        if (tableau[toId].isEmpty()) // one can always move anything to an
                                     // empty tableau
            return true;
        else
            return (tableau[toId].top() == card+1 || 
                    tableau[toId].top() == 1 && card == NUM_RANKS) ;
	    // One can place a card on the top of the tableau if it's one
	    // card smaller than the top of that tableau. One can also
	    // place a 13 over a 1.
    else if (to == 'f' || to == 'F') 
    {
        return (foundation[toId].top() == card-1);
    }
    return false; // Anything else is not permitted.
} // checkMove()

// Removes the card to be moved.
void removeCard()
{
    int error=0;
    switch (from) 
    {
        case 'd':
        case 'D':
            if (!deck.isEmpty())
                waste = deck.dequeue();
            else
                waste = -1;
            break;
        case 'r':
        case 'R':
            if (!reserve.isEmpty())
                reserve.pop();
            else
                error = 1;
            break;
        case 't':
            if (!tableau[fromId].isEmpty())
                tableau[fromId].pop();
            else
                error = 2;
            break;
        case 'T':
            if (!tableau[fromId].isEmpty())
            {
                //If the user is moving an entire tableau to a foundation, then we pop the top card.
                if (to == 'f' || to == 'F')
                    tableau[fromId].pop();
            }
            else
                error = 3;
            break;
    }
    if (error) 
    {
        cout << "Error in the code: " << error << endl;
        exit(1);
    }
} // removeCard()

// Adds the card or the entire tableau to its new destination.
void addCard(int card)
{
    if (from == 'T' && (to == 't' || to == 'T')) 
    {
        // add the stack onto which we move at the end of the one we move.
        tableau[fromId].concatenate(tableau[toId]);
        // move that stack back into the destination stack.
        tableau[toId].concatenate(tableau[fromId]);
    }
    else 
        if (to == 't' || to == 'T') 
            tableau[toId].push(card);
        else
            foundation[toId].push(card);
} // addCard()

// Determines what card is to be moved.
int getCard()
{
    switch (from)
    {
        case 'd':
        case 'D':
            return waste;
        case 'r':
        case 'R':
            if (!reserve.isEmpty())
                return reserve.top();
            else
                return -1;
        case 't':
            if (!tableau[fromId].isEmpty())
                return tableau[fromId].top();
            else
                return -1;
        case 'T':
            if (!tableau[fromId].isEmpty())
                //Check if the user is attempting to move an entire tableau onto a foundation.
                if (to == 'f' || to == 'F')
                    //If the user wants to move to a foundation, return the top card.
                    return tableau[fromId].top();
                else
                    //Otherwise, return the bottom card.
                    return tableau[fromId].bottom();
            else
                return -1;
        default:
            return -1;
    }
} // getCard()

// Clears the terminal screen.
void clearScreen()
{
    for (int i=0; i<24; i++)
        cout << endl;
} // clearScreen()



//---------------------------------
// Begin HW03 Additional Functions
// Author: Dan Cassidy
//---------------------------------

/****************************** updateScore ********************************************************
Action:         Small utility function to check if the score needs to be updated and then does so.
                Also checks to see if the player has won the game if all the cards have been moved
                to the foundations.

Parameters:
IN:             None
OUT:            None

Returns:        Nothing

Precondition:   This function should only be called after a valid move.
***************************************************************************************************/
void updateScore()
{
    //Only add to the score if a card was moved to a foundation.
    if (to == 'f' || to == 'F')
        //Preincrement score then check to see if it is equal to the number of cards the game was
        //played with.  If it is, then the player has won and the flag is set indicating as much.
        if (++score == NUM_CARDS)
            playerHasWon = true;
}
