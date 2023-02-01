//
// Created by me on 8/4/22.
//

#ifndef UNTITLED_COVERFUNCTION_H
#define UNTITLED_COVERFUNCTION_H


#include "Function.h"
#include "Graph.h"
#include <string>


class CoverFunction : public Function{
public:
    CoverFunction(string graphName);
    virtual double functionValue(vector<int> aSet);
    virtual double functionValue(int node);
    virtual double marginalGain(int v, vector<int> aSet);  //returns a value of f(aSet ∪ {v}) - f(aSet)
    virtual double marginalGain(vector<int> A, vector<int> aSet); //returns a value of f(aSet ∪ A) - f(aSet)
    ~CoverFunction();
    virtual int getGroundSetSize();
    virtual int getNumOfEdges();
    virtual vector<vector<int>>* getGraph();
private:
    int n; //number of nodes
    int m; //num of edges
    unique_ptr<Graph> myGraph;
    void printVector(vector<int> v);

};




#endif //UNTITLED_COVERFUNCTION_H
