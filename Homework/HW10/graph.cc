/********************************************************************
  Author:        Dana Vrajitoru, IUSB, CS
  Class:         C243 Data Structures
  File name:     graph.cc
  Last updated:  November 26, 2012.
  Description:   Implementation of the graph class.
  *********************************************************************/

#include "graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <queue>
#include <cctype>

#include "Utilities.h"

using namespace std;

// A sort of hash function for the names - works only if the names are
// in sequence, all of them starting with an uppercase, and none of
// them having the same starting letter. Like, 'A', 'B', 'C', etc.
int Graph::index(const string name)
{
    return int(name[0] - 'A');
} // Graph::index()

// Default constructor - makes an empty undirected graph.
Graph::Graph(int nv)
{
    numVertices = 0;
    numEdges = 0;
    directed = false;
    if (nv > 0)
        init(nv);
} // Graph::Graph()

// Destructor - empty the lists and the vector.
Graph::~Graph()
{
    makeEmpty();
} // Graph::~Graph()

// Starts the graph over.
void Graph::makeEmpty()
{
    directed = false;
    if (numVertices)
    {
        for (int i = 0; i < numVertices; i++)
            vertices[i].edgeList.clear();
        vertices.erase(vertices.begin(), vertices.end());
        numVertices = 0;
        numEdges = 0;
    }
} // Graph::makeEmpty()

// Tells us if the graph is directed.
bool Graph::isDirected()
{
    return directed;
} // Graph::isDirected()

// Set the direction flag to directed or undirected.
void Graph::setDirected()
{
    directed = true;
} // Graph::setDirected()

void Graph::setUndirected()
{
    directed = false;
} // Graph::setUndirected()

// Initialize the data structure for the specified number of vertices.
void Graph::init(int nv)
{
    addVertex(nv - 1); // Last index we need
} // Graph::init()

// Adds a vertex with the specified ind_nex. It also adds all the
// intermediate vertices from the current position to the new one. It
// creates labels with uppercase letters in order.
void Graph::addVertex(int index)
{
    string s = "A";
    for (int i = numVertices; i <= index; i++)
    {
        s[0] = char('A' + i);
        addVertex(s);
    }
} // Graph::addVertex()

// Adds a vertex with a given name.
void Graph::addVertex(const string theName)
{
    vertex newVertex;
    newVertex.name = theName;
    vertices.push_back(newVertex);
    numVertices++;
}

// Adds an edge based on vertex names. If the graph is undirected,
// it adds the edge in both directions.
void Graph::addEdge(char *name1, char *name2)
{
    int index1 = index(string(name1));
    int index2 = index(string(name2));
    if (index1 > numVertices)
        addVertex(index1);
    if (index2 >= numVertices)
        addVertex(index2);
    addEdge(index1, index2);
} // Graph::addEdge()

// Adds an edge based on vertex names. If the graph is undirected,
// it adds the edge in both directions.
void Graph::addEdge(const string name1, const string name2)
{
    int index1 = index(name1);
    int index2 = index(name2);
    if (index1 >= numVertices)
        addVertex(name1);
    if (index2 >= numVertices)
        addVertex(name2);
    addEdge(index1, index2);
} // Graph::addEdge()

// Adds an edge between vertices identified by their index
void Graph::addEdge(const int index1, const int index2)
{
    vertices[index1].edgeList.push_back(index2);
    if (!directed)
        vertices[index2].edgeList.push_back(index1);
    numEdges++;
} // Graph::addEdge()

// Reads the graph from a file
void Graph::read(const char *filename)
{
    ifstream fin(filename);
    char buffer[20], buffer1[20];
    int nrv;
    fin >> buffer; // read the letter U or D
    if (buffer[0] == 'd' || buffer[0] == 'D')
        directed = true;
    fin >> nrv; // read the number of vertices

    for (int i = 0; i < nrv; i++) // read all the names of vertices
    {
        fin >> buffer; // read the vertex name
        addVertex(string(buffer)); // this will also update numVertices
    }

    // read all the edges until the end of the file 
    while (!fin.eof() && fin.good()) // we have not reached the end of the file
    {
        fin >> buffer;
        if (strlen(buffer) && fin.good()) // was the first name read correctly?
        {
            fin >> buffer1;
            if (strlen(buffer) && strlen(buffer1))  // how about the second?
            {
                // we know both names are valid here
                buffer[strlen(buffer) - 1] = '\0'; // Deleting the ','
                addEdge(string(buffer), string(buffer1));
            }
        }
    }
    fin.close();
} // Graph::read()

// Simple print of the graph.
void Graph::print()
{
    int i;
    if (directed)
        cout << "The graph is directed" << endl;
    else
        cout << "The graph is undirected" << endl;
    cout << "The graph contains " << numVertices
         << " vertices and " << numEdges << " edges" << endl;

    if (numVertices)
    {
        // print all the names
        cout << "The vertex names are: ";
        for (i = 0; i < numVertices; i++)
            cout << vertices[i].name << ' ';

        // print all the adjacency lists
        cout << endl
             << "The adjacency list for each vertex is:" << endl;
        for (i = 0; i < numVertices; i++)
        {
            cout << "vertex " << i << ": ";
            ListIntIter iter;
            iter = vertices[i].edgeList.begin();
            while (iter != vertices[i].edgeList.end())
            {
                // here we have an edge from i to *iter.
                cout << *iter << ' '; // output the content of the node
                iter++;               // move to the next node
            }
            cout << endl;
        }
    }
} // Graph::print()

// Functions to be implemented by the students.

// Writes the graph to a file that is compatible with the function read.
void Graph::write(const char *filename)
{
    //Open the output file and verify that it was opened successfully.
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Opening file \"" << filename << "\" for output failed.\n";
        return;
    }

    //Write out the type of graph.
    fout << (directed ? 'D' : 'U') << '\n';
    
    //Write the number of vertices.
    fout << numVertices << '\n';
    
    //Write the name of each vertex.
    for (int i = 0; i < numVertices; ++i)
        fout << vertices[i].name << '\n';

    //Allocate and verify an array to keep track of the vertices visited.
    bool *visited = new (nothrow) bool[numVertices];
    testAllocation(visited);
    for (int i = 0; i < numVertices; ++i)
        visited[i] = false;

    //Write the edge list.
    for (int i = 0; i < numVertices; ++i)
    {
        //Mark the current vertex as visited.
        visited[i] = true;
        //Iterate through the current vertex's edge list.
        for (ListIntIter iter = vertices[i].edgeList.begin();
             iter != vertices[i].edgeList.end(); iter++)
            //If the graph is not directed, rely on whether the edge vertex has been visited
            //or not to determine whether to write the edge to the file.  If the graph is directed
            //write all the edges anyway.
            if (!visited[*iter] || directed)
                fout << vertices[i].name << ", " << vertices[*iter].name << '\n';
    }
        
    //Close the file.
    fout.close();
} // Graph::write()

// Inputs a graph from the user. First it should ask for the number of
// vertices, then call the function init with the number of vertices
// to automatically generate the labels as uppercase letters, and then
// ask the use to input the edges. The user will be expected to enter
// uppercase letters to identify the edges, and anything else should
// end the input process.
void Graph::input()
{
    //Empty the graph in preparation for the user entering one of their own.
    makeEmpty();

    //Ask the user for the number of vertices, initialize the graph with that number of vertices,
    //and give feedback to the user stating that the specified number of vertices have been added
    //with names beginning at 'A' and progressing alphabetically.
    int localNumVertices = 0;
    while (localNumVertices < 1 || localNumVertices > 26)
    {
        cout << "How many vertices will be in the graph? (1-26) ";
        getInput(localNumVertices);
    }
    init(localNumVertices);
    cout << localNumVertices << (localNumVertices == 1 ? " vertex has" : " vertices have")
         << " been added to the graph.\n"
         << "The names of the vertices begin at \"A\" and progress alphabetically.\n\n";

    //Ask the user if the graph is directed or not and if it is, proceed to call setDirected() to
    //set the graph as directed.
    char isDirected = 0;
    while (isDirected != 'Y' && isDirected != 'N')
    {
        cout << "Is the graph directed? (Y/N) ";
        cin >> isDirected;
        isDirected = toupper(isDirected);
    }
    if (isDirected == 'Y')
        setDirected();

    //Add edges to the graph.
    char edge1[2] = {'A'}, edge2[2] = {'A'};
    cout << "\nAdd edges in the format \"First-Vertex-Name Second-Vertex-Name\", for\n"
         << "example: \"A B\".  Enter anything other than capital letters to finish.\n";
    //Loop while the variables used to input the edges are valid.
    while (edge1[0] >= 'A' && edge1[0] <= 'Z' && edge2[0] >= 'A' && edge2[0] <= 'Z')
    {
        //Ask the user to add an edge.
        cout << "Add an edge: ";

        //Get the first vertex of the edge.
        cin >> edge1;
        //Check that the user hasn't typed a non-capital-letter, and if so, immediately break
        //of the while loop.
        if (edge1[0] < 'A' || edge1[0] > 'Z')
            break;

        //Eat the space in between the vertices.
        cin.get();

        //Get the second vertex of the edge.
        cin >> edge2;
        //Check that the user hasn't typed a non-capital-letter, and if so, immediately break
        //of the while loop.
        if (edge2[0] < 'A' || edge2[0] > 'Z')
            break;

        //Logic to validate the edges before adding them.
        //Checks to make sure the edges aren't the same (e.g. - A A).
        if (edge1[0] == edge2[0])
        {
            cout << "Cannot add an edge from a vertex to itself.\n";
        }
        //Checks to make sure the edges are within the bounds of the graph.
        else if (index(edge1) >= numVertices || index(edge2) >= numVertices)
        {
            cout << "One or both of those vertices don't exist.\n";
        }
        //Edges aren't obviously bad.
        else
        {
            //Check to make sure the edge doesn't already exist.
            bool edgeExists = false;
            //Iterate through the edgeList of the vertex specified by edge1.
            for (ListIntIter iter = vertices[index(edge1)].edgeList.begin();
                 iter != vertices[index(edge1)].edgeList.end() && !edgeExists; iter++)
                if (*iter == index(edge2))
                    //If the edge exists in the edgeList of the vertex specified by edge1, set the
                    //sentinel variable edgeExists to true.
                    edgeExists = true;

            //Depending on whether the edge already exists, either warn the user that it already
            //exists or add it then give feedback to the user saying as such.
            if (edgeExists)
                cout << "Edge already exists.\n";
            else
            {
                addEdge(edge1, edge2);
                cout << "Edge added.\n";
            }
        }
    }
} // Graph::input()

// Print the graph in depth-first and breadth-first order. Implement
// one of these two functions.
void Graph::printDf()
{
    ;
} // Graph::printDf()

void Graph::printBf()
{
    //Check if the graph is empty or not.
    if (!numVertices)
    {
        cout << "The graph is empty.\n";
        return;
    }

    //Set up the array of those vertices that have been visited, including allocating it,
    //testing said allocation, and then initializing it to indicate that no vertices have
    //been visited yet.
    bool *visited = new (nothrow) bool[numVertices];
    testAllocation(visited);
    for (int i = 0; i < numVertices; ++i)
        visited[i] = false;

    //The processing queue for vertices.
    queue<int> processingQueue;

    //The connected component counter.
    int ccCounter = 1;

    //This loop simply ensures that, because we are simply printing the graph in breadth-first
    //order rather than searching, that all possible connected components are covered.
    for (int i = 0; i < numVertices; ++i)
    {
        //If the vertex referenced by i hasn't been visited yet...
        if (!visited[i])
        {
            //Indicate the start of a new connected component in the graph.
            cout << "Connected Component " << ccCounter++ << ": ";
            
            //Add the vertex to the processing queue.
            processingQueue.push(i);

            //Traverse and print the graph in breadth-first order, starting from the initial vertex.
            while (!processingQueue.empty())
            {
                //Declare a variable holding the index of the vertex that is currently being
                //worked on.
                int currentVertex = processingQueue.front();

                //Print out the name of the vertex, mark it as visited, and then dequeue it.
                cout << vertices[currentVertex].name << ' ';
                visited[currentVertex] = true;
                processingQueue.pop();

                //Run through the current vertex's edge list and add any vertices not already
                //visited to the processing queue.
                for (ListIntIter iter = vertices[currentVertex].edgeList.begin();
                     iter != vertices[currentVertex].edgeList.end(); ++iter)
                {
                    if (!visited[*iter])
                    {
                        visited[*iter] = true;
                        processingQueue.push(*iter);
                    }
                }
            }
            cout << '\n';
        }
    }
} // Graph::printBf()
