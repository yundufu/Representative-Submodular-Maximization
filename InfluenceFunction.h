//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_INFLUENCEFUNCTION_H
#define UNTITLED_INFLUENCEFUNCTION_H



/*
 * This class implements the influence function sigma: 2^V -> R under independent cascade model
 *
 */

#include "Function.h"
#include "Graph.h"
#include <string>
#include "TIMCoverage.h"


using namespace  std;

class InfluenceFunction : public Function{
public:
    InfluenceFunction(string graphName);
    InfluenceFunction(string graphName, double edgeProbability);
    virtual double functionValue(vector<int> aSet);
    virtual double functionValue(int node);
    virtual double marginalGain(int v, vector<int> aSet);  //returns a value of f(aSet ∪ {v}) - f(aSet)
    virtual double marginalGain(vector<int> A, vector<int> aSet); //returns a value of f(aSet ∪ A) - f(aSet)
    ~InfluenceFunction();
    virtual int getGroundSetSize();
    virtual int getNumOfEdges();

    virtual bool isIntersected(vector<int> A, const vector<int>& B);
    virtual vector<vector<int>>* getRandomRRSetsForInfluenceMaximization();
    virtual int getNumOfRRSets();
    virtual vector<vector<int>>* getGraph();
private:
    int n; //number of nodes
    int m; //num of edges
    int R; //number of RRsets generated
    unique_ptr<Graph> myGraph;
    int findInfluenceGivenSeedSet(unique_ptr<Graph> &graph, set<int> seedSet, int R);
    void printVector(vector<int> v);
//    vector<vector<int>> computeLookupTable(vector<vector<int>>* randomRRSets, int n);
//    vector<vector<int>> lookupTable;
};


#endif //UNTITLED_INFLUENCEFUNCTION_H
