# Dikjstra's Algorithm
**Problem and Specs**
  - Build a graph from an input file and then calculate the minimum costing path to each vertex from a starting position
  - all input files are in said format: City1;City2;DistanceBetween
    * cities may have spaces between them
    * certain cities may not be in the same segment of graph of the starting point i.e. disconnected graph. Must factor in these cities
      and say "Not on Path" or some message of that sort
  - client file should take in file name in command line input. display error if improper file name/syntax is used!
  - use either parallel arrays/struct to organize data. I WILL USE ARRAYS.
    * vectors for the vertices list and previous list. arrays for the marked and distance values

**My Data Structures**
  - 2 vectors, one for previous and one for all vertices
    * all vertices will put into a vector to check for duplicates when adding to the graph
  - 2 arrays, one for distances and one for marked values
    * dist[] and mark[] will be used in method to find min value of a non-marked vertex for dikjstra's algorithm
  - Graph object used to store the information in the graph
  - Queue object used to get the children/vertices pointed to by the starting vertex and all subsequent marked vertices

# The Graph Class
The Graph class has a maximum number of 50 vertices due to the maxVertices value defined in the constructor. There are methods to check the
status of the Graph class such as IsEmpty() and isFull(). Neither of these methods will be used in my algorithm, however. Similarly, I will not
be using ClearMarks(), MarkVertex(), IsMarked(), or IndexIs() as I would prefer to use the parallel arrays and STL's algorithm to perform these
functions. Below are all functions described in short, along with which I am using and which I am not:

# Functions Being Used
- AddVertex(): void function that adds a vertex into the vertices vector of the Graph class
- AddEdge(): void function that creates a connection between two vertices and adds the weight between the two to the adjancency matrix: edges[][]
- WeightIs(): returns the weight of a given vertex to the user
- GetToVertices(): fills a queue with all vertices connected to a provided vertex based on information in the adjancency matrix

# Functions Unused
- IsFull(): a function that returns if the Graph is full. Not using as it assumed from my specifications that the number of vertices will not exceed 50.
- IsEmpty(): a function that retusn if the Graph is empty. Not using as I do not need to check if the Graph is empty. I am not modifying it, only adding.
- ClearMarks(): void function that clears all marks in the graph. Not using as I have my own bool array for marks in the client file.
- MarkVertex(): void function that marks a vertex in the graph. Not using as I have my own bool array for marks in the client file.
- IsMarked(): returns if a given vertex is marked. Not using as I have my own bool array for marks in the client file.
- IndexIs(): returns the index of a vertex in the vertices vector of the Graph class. Not using as I will be using my own vertex vector in the client file.

# Building the Graph
To build the graph I will read in the data and separate it by the semicolons, and then input items accordingly. If a city is NOT in the
vector (meaning it has not been added) then I will use the Graph class' AddVertex() function. After either adding the vertices or skipping
over due to them already being added in, I will use the Graph class' AddEdge() function to create a connection between the two. When this is all
done, the graph is built! This will be built in a separate method with the Graph object being sent in as pass by reference.

# Dijkstra's Algorithm
First, I will request the user for a starting point. From there, I will use the Graph class' GetToVertices() to fill my queue and begin going through
all of the vertices connected to my starting point. Then, these items will be dequeued and have their weights input into the dist[] array if their values
are less than the values already in there. Note that for the first run, these values will ALWAYS be input as all values starting in the dist[] array are
INT_MAX and all numbers I am handling are less than that.

After inserting all these numbers, the program will then determine which is the lowest, non-marked value through function getMin() and will return the
index of that value. The program has a built in "safety switch" that will choose a non-marked value to start on, even if it is not technically the "lowest value".
This prevents the program from not selecting a proper value or selecting a marked value that it inteprets as being the lowest. This also helps the program stop
when there are no more paths to take. If the program EVER returns an index with that points to a value with INT_MAX, then the program breaks out of the loop.
When the lowest distance found is INT_MAX, this means that there were no distances lower than the default value (INT_MAX) and that there are no more paths for the
computer to take. The main loop that runs this code is a for loop that runs vertex.size()-1 times, as the first vertex (starting vertex) is already marked.

Assuming a valid (non-INT_MAX) value is found, it is marked, given a distance, and previous value. Then it is chosen as the next starting vertex to investigate. The
process above repeats for this new vertex until either this loop ends OR a break occurs due to getMin() returning an index that points to INT_MAX in the dist[] array.

Finally, to print the resulting table, I will use formatting functions to pad the strings and organize them in a table-like format. The format will follow:
  - Vertex: print all vertex in order of their distances from low to high
  - Distance: print all distances from low to high. If distance = INT_MAX, then print "Not on Path". Starting dist is "0".
  - Previous: print all previous accesses to each vertex organized by distances low to high. If distance = INT_MAX, print "N/A". Starting prev is "N/A"
<!-->
    Sample Input:
    1;2;1
    1;3;1
    2;4;1
    6;7;1
    7;8;1
    8;7;1

    Starting Vertex Selected: 1

    Sample Output:
    ================================================================
        Vertex                 Distance                 Previous

        1                        0                      N/A
        2                        1                        1
        3                        1                        1
        4                        2                        2
        6              Not On Path                      N/A
        7              Not On Path                      N/A
        8              Not On Path                      N/A
    ================================================================
