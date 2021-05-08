#include<iostream>
#include<algorithm>
#include<fstream>
#include<cstring>
#include<climits>
#include<sstream>

#include "graph.h"

using namespace std;

// INPUT & INITIALIZING FUNCTIONS //
bool checkArgs(int argc, char** argv); // checks arguments passed in command line
void getInput(const string& input, string& origin, string& destination, int& distance); // gets input from file
void createGraph(Graph<string>& myGraph, char* fileName, vector<string>& vertices); // creates graph based on input data
void initArrays(bool mark[], int dist[], const int& size); // initializes the mark and distance arrays
vector<string> tokenizeInput(const string& input); // tokenizes input based on delimiter ';'
int getStart(vector<string>& vertex); // requests user input for starting position in Dijkstra's Algorithm

// DIJKSTRA'S ALGORITHM //
void dijkstra(const Graph<string>& myGraph, Queue<string>& myQ, vector<string>& vertex, bool mark[], int dist[], vector<string>& prev, const int& START); // runs dijkstra's algorithm
int getMin(int dist[], bool mark[], const int& size); // gets minimum value in array
void printTable(const vector<string>& vertex, bool mark[], int dist[], const vector<string>& prev, const int& size); // print table after the algorithm finishes

// STRING MANIPULATION //
string padFront(string out, const int& length); // add spaces to front of vertex name for formatting purposes
string padBack(string out, const int& length); // add spaces to end of vertex name for formatting purposes
string toString(int i); // turns integer into string

// ERROR HANDLING
void errorMessage(); // displays an error message to the user

// main method
int main(int argc, char** argv) {
    system("clear");
    if(!checkArgs(argc, argv))
        return 1; // error in command line arguments

    // Graph, Queue, and create graph vars
    Graph<string> myGraph(50);
    Queue<string> myQ(50);
    vector<string> vertex; // unique vertices assigned in createGraph
    int numVertices = 0;
    createGraph(myGraph, argv[1], vertex); // create myGraph using fileName (argv[1])
    
    // Array vars for Dikjstra's Algorithm
    // & vector to contain previous destinations
    bool mark[vertex.size()];
    int dist[vertex.size()];
    vector<string> prev(vertex.size()); // size = vertex.size()
    initArrays(mark, dist, vertex.size()); // initialize arrays with false and INT_MAX

    // run dijkstra's algorithm
    dijkstra(myGraph, myQ, vertex, mark, dist, prev, getStart(vertex));
    printTable(vertex, mark, dist, prev, vertex.size());

    return 0;
}

// creates graph from input data
void createGraph(Graph<string>& myGraph, char* fileName, vector<string>& vertices) {
    // open file
    ifstream inFile;
    inFile.open(fileName);

    // input vars
    string input;
    string origin;
    string destination;
    int distance;

    // duplicate vector
    // checks and prevents multiple of the same vertex being added
    vector<string> vertex;

    // while inFile has lines, add vertex and edge size from token vector
    while(getline(inFile, input)) {
        getInput(input, origin, destination, distance); // pass by reference allows function to change input vars
        
        // first pass
        if(vertex.size()==0) {
            myGraph.AddVertex(origin); // add origin vertex
            vertex.push_back(origin);
            
            myGraph.AddVertex(destination); // add destination vertex
            vertex.push_back(destination);
            
            myGraph.AddEdge(origin, destination, distance); // add edge between two new vertex
        }
        
        // future passes
        else {
            // only add origin if not already in vector
            if(find(vertex.begin(), vertex.end(), origin)==vertex.end()) {
                myGraph.AddVertex(origin); // add origin vertex
                vertex.push_back(origin);
            }
            // only add destination if not already in vector 
            if(find(vertex.begin(), vertex.end(), destination)==vertex.end()) {
                myGraph.AddVertex(destination); // add destination vertex
                vertex.push_back(destination);
            }

            myGraph.AddEdge(origin, destination, distance); // add edge between two new vertex
        }
    }
    vertices = vertex;
}

// initialize mark and distance arrays with false and INT_MAX
void initArrays(bool mark[], int dist[], const int& size) {
    for(int i=0; i<size; i++) {
        mark[i] = false;
        dist[i] = INT_MAX;
    }
}

// get input from inFile
void getInput(const string& input, string& origin, string& destination, int& distance) {
    // token var
    vector<string> tokens;

    // tokenize input
    tokens = tokenizeInput(input); // tokenize the input string based on the ; delimiter
    origin = tokens[0];
    destination = tokens[1];

    // convert value 3 (tokens[2]) into integer form using atoi
    distance = atoi(tokens[2].c_str());
}

// tokenize input based on ; delimiter
vector<string> tokenizeInput(const string& input) {
    // loop through until length is reached
    int i=0;
    string token = "";
    vector<string> tokens;

    // loop while under length, and search for ';'
    while(i<input.length()) {
        while(input[i]!=';'&&i<input.length()) {
            token+=input[i++]; // add input[i] to token and increment
        }
        tokens.push_back(token); // push string to vector
        token = ""; // reset string
        i++; // increment i again
    }
    return tokens;
}

// checks all arguments passed into the command line
bool checkArgs(int argc, char** argv) {
    // catch invalid file input
    if(argc==1) {
        cerr<<"No file declared in command line! Please input a file name to use this program."<<endl;
        return false;
    }
    // invalid usage of command
    else if(argc!=2) {
        cerr<<"Invalid usage of command! Please use the command as this: ./a.out [fileName]"<<endl;
        return false;
    }

    // check if file can be opened
    ifstream inFile;
    inFile.open(argv[1]);
    if(!inFile.is_open()) {
        cerr<<"File "<<argv[1]<<" could not be opened."<<endl;
        inFile.close();
        return false;
    }
    inFile.close();
    return true;
}

// requests user input for starting position
int getStart(vector<string>& vertex) {
    // main loop for menu
    sort(vertex.begin(), vertex.end()); // sorts vertex in alphabetical order
    bool exit = false;
    do {
        cout<<"\t==========DIJKSTRA'S ALGORITHM=========="<<endl;
        for(int i=0; i<vertex.size(); i++)
            cout<<"\t= "<<padBack(vertex[i], 38-vertex[i].length())<<endl;
        cout<<"\t========================================"<<endl<<endl;

        // vars
        string input;
        vector<string>::iterator in;

        cout<<"\tPlease enter the name of your desired starting point: ";
        getline(cin, input);

        // find if the name exists or not
        in = find(vertex.begin(), vertex.end(), input); 
        if(in!=vertex.end())
            return in-vertex.begin(); // returns index of found value
        else
            errorMessage(); // print error message for invalid input (not in vector)
    } while(!exit);
    return -1; // for unreachable return errors  
}

// find a path to every possible vertex in the graph using Dijkstra's Algorithm
void dijkstra(const Graph<string>& myGraph, Queue<string>& myQ, vector<string>& vertex, bool mark[], int dist[], vector<string>& prev, const int& START) {
    // on dijkstra's first run, set starting index dist to 0 and prev to N/A and go from there
    mark[START] = true;
    prev[START] = "N/A";
    dist[START] = 0;
    const int size = vertex.size();

    int index = START;
    vector<string>::iterator in; // val to help grab index of item searched for by dijkstra

    for(int i=0; i<size-1; i++) {
        // fill queue with new vertices
        myGraph.GetToVertices(vertex[index], myQ);

        // empties queue and gathers lowest values
        // will not add any values or mark any values
        // if the queue is empty, as the end of the path
        // has been reached
        if(!myQ.isEmpty()) {
            while(!myQ.isEmpty()) {
                in = find(vertex.begin(), vertex.end(), myQ.getFront());
                int weight = myGraph.WeightIs(vertex[index], myQ.dequeue())+dist[index];
                
                // only change value if lower than curr value
                if(dist[in-vertex.begin()]>weight) {
                    dist[in-vertex.begin()] = weight;
                    prev[in-vertex.begin()] = vertex[index];
                }
            }
            int MIN = getMin(dist, mark, size);
            index = MIN;
            
            // if getMin returns an index where dist[index]==INT_MAX, the algorithm hits a
            // roadblock, as that means INT_MAX is the lowest value the computer could find,
            // meaning that there are no more paths left for the computer to take
            if(dist[MIN]==INT_MAX)
                break;

            mark[MIN] = true;            
        }
    }
}

// print the table resulting from Dijkstra's Algorithm
void printTable(const vector<string>& vertex, bool mark[], int dist[], const vector<string>& prev, const int& size) {
    // reset mark list
    // allows user to use getMin() again
    for(int i=0; i<size; i++)
        mark[i] = false;
    
    // print every value in vertex vector based on distance (low-high)
    cout<<"\t================================================================================"<<endl;
    cout<<"\t"<<padFront("Vertex", 18)<<padFront(" ", 7)<<padFront("Distance", 18)<<padFront(" ", 7)<<padFront("Previous", 18)<<endl<<endl;
    for(int i=0; i<size; i++) {
        int index = getMin(dist, mark, size); // get index of min value
        mark[index] = true; // only want to print once! so mark value at i as true
        cout<<"\t"<<padFront(vertex[index], 18)<<padFront(" ", 7)<<padFront(dist[index]==INT_MAX?"Not On Path":toString(dist[index]), 18); // ternary allows program to print "NOT ON PATH" if INT_MAX detected
        cout<<padFront(" ", 7)<<padFront(dist[index]==INT_MAX?"N/A":prev[index], 18)<<endl;
    }
    cout<<"\t================================================================================"<<endl;
}

// gets minimum value in array
int getMin(int dist[], bool mark[], const int& size) {
    // get a starting low value
    // cannot be from a marked vertex
    int low;
    for(int i=0; i<size; i++) {
        if(!mark[i]) {
            low = i;
            break;
        }
    }

    // find lowest value
    for(int i=1; i<size; i++)
        if(dist[i]<dist[low]&&!mark[i]) // new min must not be from a marked vertex
            low = i;
    return low; // returns index of lowest value in array
}

// add spaces to the front of a string
string padFront(string out, const int& length) {
    while(out.length()<length) {
        out = " "+out;
    }
    return out;
}

// add spaces to the end of a string
string padBack(string out, const int& length) {
    for(int i=0; i<length-1; i++) {
        out+=" ";
    }
    out+="=";
    return out;
}

// turns integer into string
string toString(int i) {
    stringstream s;
    s<<i;
    return s.str();
}

// prints error message to user
void errorMessage() {
    string temp;
    cerr<<"\tPlease input a valid option. Press enter to continue...";
    getline(cin, temp);
    system("clear");
}