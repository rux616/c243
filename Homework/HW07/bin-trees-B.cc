
/*** M A N I P U L A T E   B I N A R Y   T R E E S   R E C U R S I V E L Y ***

FILE NAME      :  bin-trees-B.cc

FIRST COMPLETED:  by W. Knight, July 5, 1993
MODIFIED       :  by W. Knight, Jan. 17, 1994 (converted it from Pascal to C++)
                  by W. Knight, Sept. 1, 1997 (revised documentation)
                  by W. Knight, Oct. 7, 1998  (adapted it for C243)
                  by W. Knight, Aug. 20, 2000 (added a function)
                  by D. Vrajitoru and C. George (added a print function)
                                                Nov 3, 2004
                  by D. Vrajitoru, Oct. 21, 2010
                  by D. Vrajitoru, March 5, 2012
                  by D. Vrajitoru, October 24, 2012
                  by D. Vrajitoru, October 22, 2013
                  
LANGUAGE :  GNU C++ for Linux

ACTION   :  The program allows an interactive user to build binary
            trees with integer labels on the nodes, to get information
            about these trees, and to modify or destroy them.

INPUT    :  All input consists of commands entered by an interactive user.

OUTPUT   :  All output is sent to the screen, including the prompts
            and menus provided to the interactive user and the results
            of the commands entered by the user.  The commands entered
            by the user are echoed by the program to the screen.

COMMENTS :  This program is intended to be given to C243 students as
            a programming assignment.  They are to supply missing code.

******************************************************************************/

#include <iostream>
using namespace std;
#include <cstdlib>

#include <vector>
#include <string>
#include <sstream>

#define A(x)   cout << x << endl;       // Debugging macro
#define B(x,y) cout << x << y << endl;  // Debugging macro


/**************************  D A T A   T Y P E S  ****************************/

struct node
{
    node *left;
    int  datum;
    node *right;
};
typedef node * nodePtr;      // Define a new pointer data type.


/*****************  F U N C T I O N   P R O T O T Y P E S  *******************/

void  buildABinaryTree (nodePtr &rootp);
void  prettyPrint (nodePtr rootp, int margin = 0);
void  graphPrint(nodePtr rootp);
int   graphPrintWalk(nodePtr rootp, int &pos, int level, vector<string>& output);
ostream &operator<<(ostream &out, nodePtr rootp);

void  printMenuAndGetAction (char & response);
void  printExplanatoryMessage();
void  readAndEchoCharResponse (char & response);
void  readAndEchoIntResponse (int & response);
void  testDynamicAllocation (void * pointerValue);

void  destroyABinaryTree(nodePtr &rootp);  // Copy this function from the previous homework.
bool  isHeightBalanced(nodePtr rootp);
int   numberOfSiblingPairs(nodePtr rootp);
int   sumOfLeafData(nodePtr rootp);
void  growNewLeaves(nodePtr &rootp);

bool  testHeightBalanced(nodePtr rootp, int &height);


/************************  M A I N   F U N C T I O N  **********************/

int main()
{
    nodePtr  treep  = NULL;     // "treep" stands for "tree pointer";  will be
    char      response;          // used to point to the root of a binary tree
    int       dataValue;
    
    void  printExplanatoryMessage();
  
    do  // until interactive user signals to quit
    {
        printMenuAndGetAction (response);  // Insists on a valid response
        switch (response)  // Response is necessarily one of the following.
        {
        case 'B' :
        case 'b' : buildABinaryTree (treep);
        break;
        case 'C' :
        case 'c' : cout << "\nThe tree contains " 
                        << numberOfSiblingPairs (treep)
                        << " sibling pairs." << endl;
        break;
        case 'D' :
        case 'd' : if ( isHeightBalanced (treep) )
            cout << "\nThe tree is height balanced." << endl;
        else
            cout << "\nThe tree is not height balanced." << endl;
        break;
        
        case 'G' :
        case 'g' : growNewLeaves (treep);
        cout << "\nThe tree should now have new leaves.\n";
        break;
        
      case 'P' : 
      case 'p' : if (!treep)
          cout << "\nThe tree is empty." << endl;
        else
          graphPrint (treep);
      break;
        case 'Q' :
        case 'q' : break; // Do nothing.  The program will quit.
        case 'S' :
        case 's' : cout << "\nThe sum of the data in the tree leaves is "
                        << sumOfLeafData (treep) << endl;
        break;
        }
    } while (response != 'Q' && response != 'q');
    
    cout << "\n\n";
    return 0; // Indicate normal termination.
} // main()


/*************  P R I N T   E X P L A N A T O R Y   M E S S A G E  ***********

This function prints a message to the interactive user explaining briefly
what the program is going to do.  Documented and coded by W. Knight. */

void  printExplanatoryMessage()
{
    cout << "\n\nThis program allows an interactive user to create, modify,\n";
    cout << "display, and destroy binary trees.  The program is menu driven.\n";
    cout << "Press the Enter key to continue.";
    while (cin.get() != '\n');  // Clear the input stream up through newline.
} // printExplanatoryMessage()     // Note the empty loop body.


/************  R E A D   A N D   E C H O   C H A R   R E S P O N S E  *********

This utility function assumes that an interactive user has been prompted
to enter a one-character response, followed by a newline character.
It reads the first non-white-space response character and then clears all 
other characters from the input stream, up through the first newline character.
It also echoes the character back to the user.  Documented and coded by  
W.Knight. */

void readAndEchoCharResponse (char & response)
{
    char junk;  // Holds chars that trail the desired response char.

    cin >> response;
    while (cin.get() != '\n'); // Clear the line.
    cout << response << endl;
} // readAndEchoCharResponse()


/************  R E A D   A N D   E C H O   I N T   R E S P O N S E  *********

This utility function assumes that an interactive user has been prompted
to enter an integer value, followed by a newline character.
It reads the first integer on the line and then clears all 
other characters from the input stream, up through the first newline character.
It also echoes the integer back to the user.  Documented and coded by 
W.Knight. */

void readAndEchoIntResponse (int & response)
{
    cin >> response;
    while (cin.get() != '\n');  // Clear the line.
    cout << response << endl;
} // readAndEchoIntResponse()


/**********  P R I N T   M E N U   A N D   G E T   A C T I O N  *************

This function displays a menu of actions and asks an interactive user to 
select one.  It will not return until it has read an acceptable response. 
Documented and coded by W.Knight. */

void printMenuAndGetAction (char & response)
{
    cout << "\n\n";
    cout << "Which of the following actions do you wish to take?\n";
    cout << "   B)uild or modify the current tree interactively.\n";
    cout << "   C)ount the number of sibling pairs in the tree.\n";
    cout << "   D)etermine whether the tree is height balanced.\n";
    cout << "   G)row new leaves on the current tree.\n";
    cout << "   P)rint the current tree on the screen.\n";
    cout << "   Q)uit.\n";
    cout << "   S)um the data in the leaves of the tree.\n";
    cout << "\n";
    cout << "Your choice:  ";
    
    readAndEchoCharResponse (response);
    
    while (1) // loop will continue until user gives an appropriate response
    {
        switch (response)
        {
        case 'B': case 'b': case 'C': case 'c': 
        case 'D': case 'd': case 'P': case 'p': 
        case 'Q': case 'q':
        case 'S': case 's': case 'G': case 'g':
            return;  // EXIT FROM THE FUNCTION WITH PARAMETER HAVING OK VALUE 
        default : cout << "Improper Response  "
                "Please type an appropriate letter.   ";
        }
        readAndEchoCharResponse (response);
    }
} // printMenuAndGetAction()

  
/*******************  T E S T   D Y N A M I C   A L L O C A T I O N  *********

This utility function assumes that the value of the parameter "pointer" 
has been obtained by a call to the "new" function.  If that function
was unable to dynamically allocate the required amount of memory space,
then it returned the value NULL.  This function tests to see whether
the value returned was NULL, and if it was, it terminates the program.
The parameter type is "pointer-to-void" so that any pointer data type
can be passed to this function.   Documented and coded by W. Knight. */

void testDynamicAllocation (void * pointer)
{
    if (!pointer)  // if "pointer" is NULL
    {
        cout << "\nDynamic allocation failure.  Program being terminated."
             << endl;
        exit(1);  // EXIT FROM THE ENTIRE PROGRAM
    }
} // testDynamicAllocation()

/******************  P R I N T   T R E E   V E R T I C A L L Y  *************

The following function prints a binary tree vertically graphically
showing the connections between parent and child nodes The function
and support functions were written by C. George, C423, Fall 2004. */

void graphPrint(nodePtr rootp)
{
    if(rootp == NULL) return;
	
    // create an array to hold the output as is it generated
    vector<string> output;
    int pos = 0;
    pos = graphPrintWalk(rootp, pos, 0, output);
    
    // print the root node
    cout << string(pos,' ') << rootp << endl;
    
    // print the other levels from top to bottom
    for(int i=0;i<output.size();i++)
        cout << output[i] << endl;
} // graphPrint()

// This function walks through the tree in-order to calculate the x
// position of each node in the tree. It then prints any child nodes
// to the appropriate output string and also prints inner-connecting
// links

int graphPrintWalk(nodePtr rootp, int &pos, int level, 
                   vector<string>& output)
{
    if(rootp == NULL) 
        return pos;
    else
    {
        // Expand the size of the output array if this is the first
        // node on a new level
        if(output.size() <= (level*2))
        {
            output.push_back("");
            output.push_back("");
        }
        
        // Calculate the x position of both child nodes and the current node
        int leftPos = graphPrintWalk(rootp->left, pos, level+1, output);
    
        int currPos = pos;
        
        ostringstream currDatum;
        currDatum << rootp;
        pos += currDatum.str().length()+1;
    
        int rightPos = graphPrintWalk(rootp->right, pos, level+1, output);
        
        // initialize the output streams with the current output for the level
        
        ostringstream linkLine, nodeLine;
    
        linkLine << output[level*2];
        nodeLine << output[level*2+1];

        // calculate the center of the current node
        int currOffset = currPos + (currDatum.str().length() / 2) - 1;
        
        // add the left node and its link to the current output for the level
        if(rootp->left != NULL)
        {
            // calculate the center of the left child node
            ostringstream leftDatum;
            leftDatum << rootp->left;
            int leftOffset = leftPos + (leftDatum.str().length() / 2) - 1;
            
            nodeLine << string(leftPos - nodeLine.str().length(), ' ') 
                     << rootp->left;
            
            // draw a link from this node to the left child node
            linkLine << string((leftOffset+1) - linkLine.str().length(), ' ') 
                     << string((currOffset)-(leftOffset+1), '_') << "/";
			
        }
    
        // add the right node and its link to the current output for the level
        if(rootp->right != NULL)
        {
            // calculate the center of the right child node
            ostringstream rightDatum;
            rightDatum << rootp->right;
            int rightOffset = rightPos + (rightDatum.str().length() / 2) - 1;
            
            nodeLine << string(rightPos - nodeLine.str().length(),' ') 
                     << rootp->right;
            
            // draw a link from this node to the right child node
            linkLine << string((currOffset+1) - linkLine.str().length(),' ') 
                     << "\\" <<	string((rightOffset-1) - (currOffset), '_') 
                     << " ";
        }
    
        // save the results for the current level
        output[level*2]   = linkLine.str();
        output[level*2+1] = nodeLine.str();
        
        return currPos;
    }
} // graphPrintWalk()

// This stream formatter outputs the contents of a node with a leading digit
// for datum values 0-9

ostream &operator<<(ostream &out, nodePtr rootp)
{
    out << ((rootp->datum >= 0 && rootp->datum < 10)?"0":"")
        << rootp->datum;
    return out;
} // operator<<()

/**************************  P R E T T Y   P R I N T  *************************
                                                                           
The following function sends to standard output a list of the integer data
values of the nodes in the binary tree pointed to by "rootp".  The values
are written in such a way as to show the logical structure of the tree.
The parameter "margin" should have a non-negative value, which determines 
the number of blank spaces that will be printed to the left of the root 
of the tree.  If no second argument is used when this function is called,
then the default value 0 will be used (see the function prototype).  
The function is recursive.   Documented and coded by W.Knight. */

void prettyPrint (nodePtr rootp, int margin) // 
{
    const int INDENTATION = 4;
    
    if (!rootp)
        return;
    
    else
    {
        prettyPrint(rootp->right, margin + INDENTATION);
        
        for (int i = 1; i <= margin; ++i)
            cout << " ";
        
        cout << rootp->datum << endl;
        
        prettyPrint(rootp->left, margin + INDENTATION);
    }
} // prettyPrint()


/*****************  B U I L D   A   B I N A R Y   T R E E  ******************
                                                                           
The following function allows an interactive user to construct a binary   
tree whose nodes hold integer data values.  The function is recursive. 
Documented and coded by W.Knight. 
Added by D. Vrajitoru in 2012: an option to delete the subtree when the 
rootp pointer is not null. */

void buildABinaryTree (nodePtr &rootp) // Parameter passed by REFERENCE!
{
    char action;
    
    while(1)  // Endless loop; there are two "return" statements in the loop.
              // They are executed whenever the user wants to move up the tree.
    {
        if (rootp == NULL)  // The tree or subtree pointed to by rootp is empty.
        {
            do  // This loop continues until the user responds with 'U' or 'C'.
            {
                cout << "\nYou are currently at an empty tree. If you wish to ";
                cout << "create\na new node at this position, type C and press";
                cout << " Enter.\nOtherwise ";
                cout << "type U to go up to the parent node (if any).  ";
                readAndEchoCharResponse (action);
            } while (action != 'C' && action!= 'c' && action!= 'U' 
                     && action!= 'u');
            if (action == 'U' || action == 'u')
                return; // Return control to the calling function
            else
            {
                rootp = new node;
                testDynamicAllocation (rootp);
                rootp->left  = rootp->right = NULL;
                cout << "Enter an integer for the new node:  ";
                readAndEchoIntResponse (rootp->datum);
            }	
        }
        else  // tree is not NULL
        {
            do  // This loop continues until user gives a suitable response.
            {      
                cout << "\nThe datum of the node at which you have arrived is\n";
                cout << "         ";
                cout << rootp->datum << endl;
                cout << "Enter  U  to go up to the parent node,\n";
                cout << "       L  to go down to the left,\n";
                cout << "       R  to go down to the right,\n";
                cout << "       M  to modify value of the datum stored here.\n";
                readAndEchoCharResponse (action);
            }
            while (action != 'U' && action !='u' && action !='L' && action !='l' 
                   && action != 'R' && action !='r' && action !='M' && action !='m'
                   && action != 'P' && action != 'p' && action != 'D' && action != 'd');
      
            switch (action)
            { 
            case 'U': case 'u': return;   // Go back to the calling function.
            case 'L': case 'l': buildABinaryTree (rootp->left);  break;
            case 'R': case 'r': buildABinaryTree (rootp->right); break;
            case 'P': case 'p': graphPrint(rootp); break;
            case 'M': case 'm': cout << "Enter a new datum value to replace"
                    " the current value.  ";
                readAndEchoIntResponse (rootp->datum);
                break;
            case 'D': case 'd': destroyABinaryTree(rootp); // the parameter is 
                // also passed by reference
            } // end of "switch"
        }   // end of "else"     
    }     // end of "while(1)" 
} // buildABinaryTree() 


/****************  D E S T R O Y   A   B I N A R Y   T R E E  ***************

The following function deallocates all the nodes in the binary tree
pointed to by "rootp", and it makes the tree empty (i.e., it changes to
NULL the value of the pointer passed to it).  The function is recursive.
The function documentation and heading were written by W. Knight.  */

void destroyABinaryTree(nodePtr &rootp)
{
    //Only attempt to delete the node and subtree if the node exists.
    if (rootp)
    {
        //Delete the tree from the bottom up.  This prevents memory leaks from deleting a low level
        //node and losing access to its subtree.
        destroyABinaryTree(rootp->left);
        destroyABinaryTree(rootp->right);

        //Once the recursive calls are done, actually delete the node referenced by rootp.
        delete rootp;
        //Set rootp to NULL, which, because it's a reference parameter, will set the left or right
        //data member to NULL as well.
        rootp = NULL;
    }
} // destroyABinaryTree()


/*******************  I S   H E I G H T   B A L A N C E D  ******************

This function determines whether the binary tree pointed to by "rootp"
is height balanced, by which we mean that every node in the tree has this
property:  the heights of its left and right subtrees differ by at most 1.
The function documentation and heading were written by W. Knight.  
See the third version of the function isPerfect for implementation ideas. 
*/

bool isHeightBalanced (nodePtr rootp)
{
    //This function is converted into a driver function for testHeightBalanced.

    //In this case, height is basically just a dummy variable not used for anything except to give
    //the testHeightBalanced function something to use to keep track of the height of the table.
    int height = 0;
    return testHeightBalanced(rootp, height);
} // isHeightBalanced()


/************  N U M B E R   O F   S I B L I N G   P A I R S  ***************
                                                                           
This function counts the sibling pairs (if any) in the binary tree pointed
to by "rootp".  The function is recursive.  The function documentation and 
heading were written by W. Knight.  
See the function numberOfOnlyChildren for implementation ideas. 
*/

int numberOfSiblingPairs (nodePtr rootp)
{
    if (!rootp)
        //Base case.  If the node doesn't exist, return 0.
        return 0;
    else
        //Adds one if both children exist, and zero if they don't, in addition to whatever the
        //furthing recursing adds.
        return   (rootp->left && rootp->right ? 1 : 0)
               + numberOfSiblingPairs(rootp->left)
               + numberOfSiblingPairs(rootp->right);
} // numberOfSiblingPairs()


/******************  S U M   O F   L E A F   D A T A  **********************

This function calculates and returns the sum of the data values in the leaves
of the binary tree pointed to by "rootp".  The function is recursive.  
The function documentation and heading were written by W. Knight.  */

int sumOfLeafData (nodePtr rootp)
{
    if (!rootp)
        //Base case.  If the node doesn't exist, return 0.
        return 0;
    else if (!rootp->left && !rootp->right)
        //If the node is a leaf, return its datum.  Don't need to recurse further because of the
        //fact that the node IS a leaf.
        return rootp->datum;
    else
        //This node has at least one child, so continue to recurse.
        return   sumOfLeafData(rootp->left)
               + sumOfLeafData(rootp->right);
} // sumOfLeafData()


/**********************  G R O W   N E W   L E A V E S  ****************

The following function adds a leaf to the binary tree pointed to by rootp
at every point at which the current tree has an empty subtree.  The datum
member in each new leaf is given the value 0 .  If the tree is empty, then
exactly one node is added to the tree.  The function is recursive.
The function documentation and heading were written by W. Knight.   */

void growNewLeaves (nodePtr & rootp)
{
    if (!rootp)
    {
        //Base case.  If a node doesn't exist, create it.
        rootp = new node;

        //Test to make sure the memory was allocated.
        testDynamicAllocation(rootp);

        //Set the default values of the node.
        rootp->datum = 0;
        rootp->left = NULL;
        rootp->right = NULL;
    }
    else
    {
        //This node exists.  Continue to recurse through the tree.
        growNewLeaves(rootp->left);
        growNewLeaves(rootp->right);
    }

} // growNewLeaves()


/****************************** testHeightBalanced *************************************************
Action:         This function actually tests (vs simply being the driver function) whether the tree
                (or subtree) starting at node rootp is height balanced or not.

Parameters:
IN:             nodePtr rootp, contains the pointer to the subtree to test.
OUT:            int &height, contains the height of the subtree that was tested.

Returns:        bool, representing whether the subtree tested is height balanced.

Precondition:   None
***************************************************************************************************/

bool testHeightBalanced(nodePtr rootp, int &height)
{
    if (!rootp)
    {
        //Base case.  If the node doesn't exist, set height to -1 and return true because a subtree
        //that has does not exist is by definition height balanced.  Also catches instances like the
        //root of the entire tree being NULL.
        height = -1;
        return true;
    }
    else
    {
        //The node exists.  Need to evaluate the height of its subtrees.

        int leftHeight = 0, rightHeight = 0;
        //Use recursion to determine the height of this node's subtrees and store that height in
        //leftHeight and rightHeight respectively.
        if (!testHeightBalanced(rootp->left, leftHeight) ||
            !testHeightBalanced(rootp->right, rightHeight))
            //If either of these functions return false, then the subtree is not height balanced,
            //so continue to relay that message up the chain of recursion.
            return false;
        //Check to see if the heights of the subtrees differ by more than 1.
        else if (abs(leftHeight - rightHeight) > 1)
            //If the height of the subtrees differ by more than one, the subtrees are not height
            //balanced.  Return 'false' to send that message up the chain of recursion.
            return false;
        //Assembles the height of the subtrees to be returned via height.
        else
        {
            //Set height to 1 plus the greater of leftHeight or rightHeight and return 'true'
            //so the recursion continues as required.
            height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
            return true;
        }
    }
} // testHeightBalanced()
