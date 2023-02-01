//
// Created by Xiaoyun Fu on 8/4/2022.
//
/*
 * f(v) is defined as the number of outgoing neighbors of v in the graph G
 */

#include "CoverFunction.h"

CoverFunction::CoverFunction(string graphName)
{
    myGraph = make_unique<Graph>(graphName);//for RRSets-based algo
    myGraph->readGraph();
    n  = myGraph->getNumberOfVertices();
    m = myGraph->getNumberOfEdges();  //number of edges in graph

}

//returns the number of outgoing neighbors of nodes in aSet
double CoverFunction::functionValue(vector<int> aSet)
{
    set<int> outgoingNeighbors;
    vector<vector<int>>* theGraph = myGraph->getGraph();
    for(int v : aSet)
    {
        for(int outgoingNeighbor : (*theGraph)[v])
            outgoingNeighbors.insert(outgoingNeighbor);
    }
    return outgoingNeighbors.size();
}

//returns the number of outgoing neighbors of node
double CoverFunction::functionValue(int node)
{
    vector<vector<int>>* theGraph = myGraph->getGraph();
    return (*theGraph)[node].size();
}

double CoverFunction::marginalGain(int v, vector<int> aSet) //returns a value of f(aSet ∪ {v}) - f(aSet)
{
    set<int> outgoingNeighborsOfASet;
    vector<vector<int>>* theGraph = myGraph->getGraph();
    for(int node : aSet)
    {
        for(int outgoingNeighbor : (*theGraph)[node])
            outgoingNeighborsOfASet.insert(outgoingNeighbor);
    }
    int sizebefore = outgoingNeighborsOfASet.size();//f(aSet)
    for(int node : (*theGraph)[v])     //After this loop, outgoingNeighborsOfASet.size() = f({v} U aSet)
        outgoingNeighborsOfASet.insert(node);

    return outgoingNeighborsOfASet.size() - sizebefore;
}

double CoverFunction::marginalGain(vector<int> A, vector<int> aSet) //returns a value of f(aSet ∪ A) - f(aSet)
{
    set<int> outgoingNeighborsOfASet;
    vector<vector<int>>* theGraph = myGraph->getGraph();
    for(int node : aSet)
    {
        for(int outgoingNeighbor : (*theGraph)[node])
            outgoingNeighborsOfASet.insert(outgoingNeighbor);
    }
    int sizebefore = outgoingNeighborsOfASet.size();//f(aSet)

    for(int node : A)    //After this loop, outgoingNeighborsOfASet.size() = f(A U aSet)
    {
        for(int outgoingNeighbor : (*theGraph)[node])
            outgoingNeighborsOfASet.insert(outgoingNeighbor);
    }

    return outgoingNeighborsOfASet.size() - sizebefore;
}

CoverFunction::~CoverFunction()
{

}
int CoverFunction::getGroundSetSize()
{
    return n;

}
int CoverFunction::getNumOfEdges()
{
    return m;
}
void CoverFunction::printVector(vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
        cout<<v[i]<<", ";
    cout<<endl;
}
vector<vector<int>>* CoverFunction::getGraph()
{
    return myGraph->getGraph();
}
