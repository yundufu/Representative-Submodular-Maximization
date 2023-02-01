//
// Created by me on 9/24/22.
//

#ifndef UNTITLED_LOCALSEARCH_H
#define UNTITLED_LOCALSEARCH_H



/*
 * This class implement the local search algorithm from paper MONOTONE SUBMODULAR MAXIMIZATION OVER A MATROID
   VIA NON-OBLIVIOUS LOCAL SEARCH by YUVAL FILMUS† AND JUSTIN WARD‡
   We customized the parameters in the algorithm coping with the representative submodular maximzation problem
   More specifically, the curvature c = 1, alpha = 1, epsilon = 0.1, rank r = k,
   the probability distribution P = [0.1, 0.2, 0.3,...1], selecting p from P with equal probability 1/10.

 */

#include "Function.h"
#include "AuxiliaryFunctionG.h"
#include <vector>
#include <fstream>


using namespace std;

class LocalSearch {
private:
    Function& f;
    vector<vector<int>> myGroups;
    int k; //numOfGroups
    int n; //groundSet Size

    //compute S_init and g(S_init)
    vector<int> computeSInitial(AuxiliaryFunctionG& g, double& functionValueOfW, vector<int>& order);

    //methods to compute g~(S)
    double approximateG(int N, vector<int> S);
    vector<int> sampleOf(vector<int> S);

public:
    LocalSearch(Function& givenFunction, vector<vector<int>>& groups, int given_k);
    ~LocalSearch();
    vector<int> LocalSearchAlgorithm(double& functionValueOfU, ofstream& logbook);

};

#endif //UNTITLED_LOCALSEARCH_H
