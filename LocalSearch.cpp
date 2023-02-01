//
// Created by Xiaoyun Fu on 9/15/2022.
//

#include "LocalSearch.h"
#include  <random>
#include <time.h>
#include <iostream>
#include <cassert>



LocalSearch::LocalSearch(Function& givenFunction, vector<vector<int>>& groups, int given_k):f(givenFunction)
{
    myGroups = groups;
    k = given_k;
    assert (k == groups.size());
    n = f.getGroundSetSize();

}

LocalSearch::~LocalSearch()
{

}
vector<int> LocalSearch::LocalSearchAlgorithm(double& functionValueOfU, ofstream& logbook)
{
    AuxiliaryFunctionG g(f, k);
    //compute the constants
    double epsilon2 = g.computeEpsilon2();
    int I = g.getI();
    double auxiliaryFunctionValueOfU;
    vector<int> order;
    vector<int> U = computeSInitial(g, auxiliaryFunctionValueOfU, order);
    cout << "SInitialFound" << endl;
    for(int i = 0; i < I; i++)
    {
        bool done = true;
        for(int j = 0; j < order.size(); j++)
        {
            bool shouldBreak = false;
            for(int y : myGroups[order[j]])   //for each y that is in the same groups as x = U[j]
            {
                vector<int> UPrime = U;
                //replace x with y
                UPrime[j] = y;
                double auxiliaryFunctionValueOfUPrime = g.functionValue(UPrime);
                if(auxiliaryFunctionValueOfUPrime > (1.0 + epsilon2) * auxiliaryFunctionValueOfU)
                {
                    auxiliaryFunctionValueOfU = auxiliaryFunctionValueOfUPrime;
                    U = UPrime;
                    done = false;
                    shouldBreak = true;
                    break;    //this breaks out of the y-loop
                }
            }
            //if the y-loop achieved improvement, then break out of the j-loop; otherwise the j-loop continues to next iteration
            if(shouldBreak)
                break;    //this breaks out of the j-loop
        }
        if(done)
        {
            functionValueOfU = f.functionValue(U);
            return U;
        }
    }

    functionValueOfU = 0;
    vector<int> empty;
    return empty;   //Error (Search did not converge in I iterations)
}


//Uses natural greedy algorithm to compute S_init
vector<int> LocalSearch::computeSInitial(AuxiliaryFunctionG& g, double& functionValueOfW, vector<int>& order)
{
    NaturalGreedy naturalGreedy(g,myGroups,k);
    vector<int> W = naturalGreedy.naturalGreedyAlgorithmReturnWithOrder(functionValueOfW, order);
    return W;
}

