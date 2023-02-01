
#include "Graph.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <random>
#include <queue> //for RRSet of single node belief
#include <unordered_set>  //for RRSet of single node belief


/*
  Graph.h
  Influence Maximization Problem under IC model

*/


using namespace std;

Graph::Graph(string fileName) {
    this->standardProbability = false;
    this->graphName = fileName;
}

/* Read the graph data from a file filename and store it in the instance variabble graph,
 * and compute its transpose graph.
 * Along the way, mark each node as unvisited(stored in instance variable visited)
 * and compute their inDegrees(stored in instance variable inDegree)
 * The format of the file:
 * All nodes are represented by an int. Each line contains two ints indicating an edge
 * from the first int to the second int
 * first line: n m
 * 2nd line: int1 int 2    //denoting an edge from node int1 to node int2
 *  .
 *  .
 *  .
 * last line: intx inty
 */
void Graph::readGraph() {

    ifstream  myFile(graphName);
    string s;
    if(!myFile.good()) {
        throw std::invalid_argument( "Graph file does not exist: " + graphName );
    }
    if(myFile.is_open()) {
        int beginningPosition = myFile.tellg(); //Getting the current read position.
        myFile >> n >> m;  //the first line in graph should contain n and m

        int from, to;
        double edgeProbability;
        bool probabilityAssigned = false;
        int maxDegree = 0;  //what is the meaning to compute maxDegree? It is not used.

        string line;
        myFile.seekg(beginningPosition);
        getline(myFile, line);
        int firstEdgePosition = int (myFile.tellg());
        getline(myFile, line);
        istringstream iss(line);

        //why not put this inside the while loop?
        //This is used to check if the file contains probability values along with each edge
        iss >> from >> to;   //reading an edge
        probabilityAssigned = (iss >> edgeProbability)? true:false;
        this->edgeProbabilitiesAssigned = probabilityAssigned?true:false;

        for(int i = 0; i < n; i++) {
            graph.push_back(vector<int>());
            visited.push_back(false);
            inDegree.push_back(0);
        }
//        myFile.seekg(firstEdgePosition); //using this line missed some edges
        myFile.seekg(beginningPosition);
        getline(myFile, line);
        while (myFile >> from >> to) {
            if (probabilityAssigned) {
                myFile >> edgeProbability;
                edgeProbabilities[to_string(from) + "#" + to_string(to)] = edgeProbability;
            }

            pair<int, int> edge;  //store an edge in the edge set
            edge.first = from;
            edge.second = to;
            edgeSet.push_back(edge);

            graph[from].push_back(to);    //add "to" to "from"'s adjacency list
            inDegree[to] = inDegree[to] + 1;
            if(inDegree[to] > maxDegree) {
                maxDegree = inDegree[to];
            }
        }
        myFile.close();
    }
    graphTranspose = constructTranspose(graph);
    visitMark = vector<int>(n); //what is the use of this?

//    cout << "n = " << n << ", graph size = " << graph.size() << endl;
//    cout << "m = " << m << ", edgeSet size = " << edgeSet.size() << endl;
    assert(edgeSet.size() == m);  //for debugging: check if the number of edges are correctly read.

    int numEdges = 0;
    for(int i = 0; i < graph.size(); i++)
    {
        numEdges += graph[i].size();
    }
//    cout << "m = " << m << ", numEdges = " << numEdges << endl;
    assert(numEdges == m);
}


//generates a random number in {0, 1, 2, ..., randomNumberLimit - 1}
int Graph:: generateRandomNumber(int u, int v) {

    int randomNumberLimit;
    if(this->standardProbability) {
        randomNumberLimit = this->propogationProbabilityNumber;
    }
    else if(this->edgeProbabilitiesAssigned) {
        randomNumberLimit = this->edgeProbabilities[to_string(u) + "#" + to_string(v)];
    }
    else {//p(a, b) = 1 / indegreeOf(b)
        randomNumberLimit = inDegree[v];
    }
    cout << "randomNumberLimit = " << randomNumberLimit << endl;
    return rand() % randomNumberLimit;
}


bool Graph:: flipCoinOnEdge(int u, int v) {

    double randomDouble = double (rand()) / double (RAND_MAX);
//                    cout << "randomDouble = " << randomDouble << endl;
    if(this->standardProbability) {
        return randomDouble < this->propogationProbability;
    }
    else if(this->edgeProbabilitiesAssigned) {
        return randomDouble < this->edgeProbabilities[to_string(u) + "#" + to_string(v)];
    } else{
        return randomDouble < ( 1.0 / inDegree[v]);   // the default is p(u,v) = 1/ inDegree[v]
    }




//    int randomNumber = generateRandomNumber(u, v);
////    cout << "random number generated is " << randomNumber << endl;
//    return randomNumber == 0;  //the probability randomNumber == 0 is 1/randomNumberLimit
}




int Graph::getNumberOfVertices() {
    return this->n;
}

int Graph::getNumberOfEdges() {
    return this->m;
}

//generate a single RRSet for Influence Maximization and store it in the collection with rrSetID
vector<int> Graph::generateRandomRRSetForInfluenceMaximization(int randomVertex, int rrSetID) {
    q.clear();
    rrSetsForInfluenceMaximization[rrSetID].push_back(randomVertex);
    q.push_back(randomVertex);
    int nVisitMark = 0;
    visitMark[nVisitMark++] = randomVertex;
    visited[randomVertex] = true;
    while(!q.empty()){
        int expand=q.front();
        q.pop_front();
        for(int j = 0; j < (int) graphTranspose[expand].size(); j++){
            int v = graphTranspose[expand][j];
            bool selected = this->flipCoinOnEdge(v, expand);
//            if(selected == 0)
//                cout << "edge " << v << "->" << expand << "is not selected " << endl;
            if(!selected)
                continue;
            if(visited[v])
                continue;
            if(!visited[v])
            {
                visitMark[nVisitMark++] = v;
                visited[v] = true;
            }
            q.push_back(v);
            rrSetsForInfluenceMaximization[rrSetID].push_back(v); //this fills up the (rrSetID)th entry
        }
    }

    for(int i = 0;i < nVisitMark; i++) {
        visited[visitMark[i]] = false;

    }
    return rrSetsForInfluenceMaximization[rrSetID];

}



void Graph::generateRandomRRSetsForInfluenceMaximization(int R) {
//    srand(time(0));
    vector<vector<int>>().swap(rrSetsForInfluenceMaximization);
    this->rrSetsForInfluenceMaximization = vector<vector<int>>();
    int totalSize = 0;
    clock_t begin = clock();
    while(rrSetsForInfluenceMaximization.size() < R) {
        rrSetsForInfluenceMaximization.push_back(vector<int>());
    }


    //Need to check if multiple calls all give uniform distribution
    //maybe need to initialize this in the constructor and pass as a parameter
    timesThisNodeWasPicked = vector<int>(n, 0);

    int max = 0;
    for(int i = 0; i < R; i++) {
        int randomVertex = rand() % n;
//        cout << "Entered generateRandomRRSetsForInfluenceMaximization in for-loop rand generated  " << randomVertex << endl;
        if(randomVertex > max)
            max = randomVertex;

//        cout << randomVertex << " is picked(previous) " << timesThisNodeWasPicked[randomVertex] << " times now" << endl;
        this->timesThisNodeWasPicked[randomVertex]++;
//        cout << randomVertex << " is picked " << timesThisNodeWasPicked[randomVertex] << " times now" << endl;
//        cout << "random vertex = " << randomVertex << endl;
        generateRandomRRSetForInfluenceMaximization(randomVertex, i);
        totalSize += rrSetsForInfluenceMaximization[i].size();
    }
//    cout << " largest random number generated is : " << max << endl;
//    cout << "\n Total Size is " << totalSize << endl;
//    cout << "R              = " << R << endl;
//    cout << "Average size  is " << (float)totalSize/(float)R << endl;

}



//return the random RRSets for Influence maximization
vector<vector<int>>* Graph::getRandomRRSetsForInfluenceMaximization() {
    return &rrSetsForInfluenceMaximization;
}


void Graph::clearRandomRRSets(){
    rrSetsForInfluenceMaximization.clear();
    rrSetsForInfluenceMaximization.swap(rrSetsForInfluenceMaximization);
}

vector<vector<int>>* Graph::getGraph() {
    return &this->graph;
}

vector<vector<int>>* Graph::getGraphTranspose() {
    return &this->graphTranspose;
}


vector<vector<int>> Graph::constructTranspose(vector<vector<int>> someGraph) {
    vector<vector<int>> transposedGraph = vector<vector<int>>();
    for(int i = 0; i < someGraph.size(); i++) {
        transposedGraph.push_back(vector<int>());
    }
    for(int i = 0; i < someGraph.size(); i++) {
        for(int v : someGraph[i]) { //edge i -> v in someGraph
            transposedGraph[v].push_back(i);//edge v -> i in transposedGraph
        }
    }
    return transposedGraph;
}

void Graph::assertTransposeIsCorrect() {
    assert(graph.size() == n);
    int edges = 0;

    for (int i = 0; i < n; i++) {
        for (int j : graph[i]) {
            edges++;
        }
    }
    assert(edges == m);
    int edgeCount = 0;
    int reverseEdgeCount = 0;
    for (int i = 0; i< n; i++) {
        int u = i;
        for (int j = 0; j < graph[u].size(); j++) {
            edgeCount++;
            int v = graph[u][j];  //edge u -> v in graph
            bool reverseEdgePresent = false;
            vector<int> reverseEdges = graphTranspose[v]; //edge v -> someNode in graphTranspose
            for(int uPrime : reverseEdges) {
                if(uPrime == u) {
                    reverseEdgeCount++;
                    reverseEdgePresent = true;
                }
            }
            assert(reverseEdgePresent);
        }

    }
    assert(edgeCount == reverseEdgeCount);
    assert(edgeCount == m);
}

void Graph::setPropogationProbability(float p) {
    this->propogationProbability = p;
    this->standardProbability = true;
    this->propogationProbabilityNumber = float (1 / p);
}

int Graph::getPropogationProbabilityNumber() {
    return this->propogationProbabilityNumber;
}

void Graph::printGraph(const vector<vector<int>>* thisGraph)
{
    for(int i = 0; i < thisGraph->size(); i++)
        for(int j = 0; j < thisGraph[i].size(); j++) //need to check
            cout << i << " -> " << &thisGraph[i][j] << endl;
}


