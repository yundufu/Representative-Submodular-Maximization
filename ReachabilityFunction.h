//
// Created by me on 8/30/22.
//

#ifndef UNTITLED_REACHABILITYFUNCTION_H
#define UNTITLED_REACHABILITYFUNCTION_H


/*
 * This class implement the reachability function where f(v) = number of reachable nodes from v in G
 */
#include "Function.h"
#include "Graph.h"
#include <string>

using namespace  std;

class ReachabilityFunction  : public Function{
public:
    ReachabilityFunction(string graphName);
    virtual double functionValue(vector<int> aSet);
    virtual double functionValue(int node);
    virtual double marginalGain(int v, vector<int> aSet);  //returns a value of f(aSet ∪ {v}) - f(aSet)
    virtual double marginalGain(vector<int> A, vector<int> aSet); //returns a value of f(aSet ∪ A) - f(aSet)
    ~ReachabilityFunction();
    virtual void generateReachableSets();
    virtual vector<vector<int>> getReachableSets();
    virtual void generateReverseReachableSets();
    virtual vector<vector<int>> getReverseReachableSets();
    virtual int getGroundSetSize();
    virtual int getNumOfEdges();
    virtual vector<vector<int>>* getGraph();
private:
    int n; //number of nodes
    int m; //num of edges
    unique_ptr<Graph> myGraph;
    void printVector(vector<int> v);
    vector<vector<int>> reachableSets;
    vector<vector<int>> reverseReachableSets;
};


#endif //UNTITLED_REACHABILITYFUNCTION_H
