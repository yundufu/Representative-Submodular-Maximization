//
// Created by me on 9/24/22.
//

#ifndef UNTITLED_AUXILIARYFUNCTIONG_H
#define UNTITLED_AUXILIARYFUNCTIONG_H




#include "Function.h"
#include "Graph.h"
#include <string>
#include "NaturalGreedy.h"

//This class takes a base function f as input, and compute the auxiliary function g
//This class is used for LocalSearchAlgorithm in class LocalSearch

using namespace  std;

class AuxiliaryFunctionG : public Function{
private:
    Function& f;  //baseFunction

    //constants
    double epsilon; // =0.1
    double r;// rank = k
    double c;//curvature = 1
    double alpha; // =1
    int epsilon2;
    vector<double> ProbabilityDistribution;
    double HarmonicNumber(int j);



    int n; //number of nodes
    int R; //number of RRsets generated
    int N;
    int I;
    unique_ptr<Graph> myGraph;
    void printVector(vector<int> v);

    //helper methods for computing function value
    vector<int> sampleOf(vector<int> S);
    double approximateG(int N, vector<int> S);

public:
    AuxiliaryFunctionG();
    AuxiliaryFunctionG(Function& givenFunction,int numGroups);

    virtual double functionValue(vector<int> aSet);
    virtual double functionValue(int node);
    virtual double marginalGain(int v, vector<int> aSet);  //returns a value of f(aSet ∪ {v}) - f(aSet)
    virtual double marginalGain(vector<int> A, vector<int> aSet); //returns a value of f(aSet ∪ A) - f(aSet)
    ~AuxiliaryFunctionG();
    virtual int getGroundSetSize();


    virtual bool isIntersected(vector<int> A, const vector<int>& B);
    virtual vector<vector<int>>* getRandomRRSetsForInfluenceMaximization();
    virtual int getNumOfRRSets();
    virtual vector<vector<int>>* getGraph();

    //compute the constants
    int computeI();
    int computeN();
    int getI();
    int getN();
    double computeEpsilon2();
    double getEpsilon2();

};


#endif //UNTITLED_AUXILIARYFUNCTIONG_H
