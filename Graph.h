//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_GRAPH_H
#define UNTITLED_GRAPH_H


/*
  Graph.h
  This class reads the graph data and generates the RRSets according to the Influence Maximization algorithm.
  See method definitions in Graph.cpp

*/


#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <ctime>
#include <deque>
#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <random>
#include <set>

using namespace std;


class Graph {
private:
    float propogationProbability;  //this is used when we want to assign the same probability to each edge
    bool standardProbability;
    int propogationProbabilityNumber;
    unordered_map<string, double> edgeProbabilities; //edge “u#v", prob(u,v)
    bool edgeProbabilitiesAssigned;
    string graphName;

    int n, m; //n is # of nodes in graph, m is # of edges in graph
    vector<vector<int> > graph;
    vector<vector<int> > graphTranspose; //used in generating RRSets
    vector<pair<int, int>> edgeSet; //store all edges in graph, used as the start of generating a RRSet
    //         from   to  used to randomly select a start edge for BFS in generating a RRSet

public:

    Graph(string fileName);
    vector<vector<int>> rrSetsForInfluenceMaximization;  //the RRSets generated for Influence Maximization
    deque<int> q;
    vector<int> inDegree;
    vector<bool> visited; //to mark the status of a node when using BFS in generating RRSets
    vector<int> visitMark;
    void readGraph();


    //Numbers
    int getNumberOfVertices();
    int getNumberOfEdges();

    //pointer to the graph
    vector<vector<int>>* getGraph();
    vector<vector<int>>* getGraphTranspose();
    void assertTransposeIsCorrect();//to check if the computed tranpose graph is correct

    vector<vector<int> > constructTranspose(vector<vector<int> > aGraph);
    void generateRandomRRSetsForInfluenceMaximization(int R);
    vector<int> generateRandomRRSetForInfluenceMaximization(int randomVertex, int rrSetID);


    void clearRandomRRSets();
    vector<vector<int>>* getRandomRRSetsForInfluenceMaximization();


    //Functions for propogation probability
    void setPropogationProbability(float p);
    bool flipCoinOnEdge(int u, int v);
    int generateRandomNumber(int u, int v);
    int getPropogationProbabilityNumber();

    //methods for debugging
    void printGraph(const vector<vector<int>>* thisGraph);
    vector<int> timesThisNodeWasPicked;
    vector<int> timesThisEdgeWasPicked;
    int maxRandEdge = 0;

};


#endif //UNTITLED_GRAPH_H
