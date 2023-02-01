//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_NATURALGREEDY_H
#define UNTITLED_NATURALGREEDY_H





/*
 * This class implement the natural-greedy algorithm for the equal submodular optimization problem where
 * we are given a ground set V ⊆ N with k disjoint groups G_1 ∪ ... ∪G_k = V,
 * a monotone submodular function f: 2^V -> R, an integer k, and we aim to find a set S such that
 * f(S) is maximized subject to |S ∩ G_i| = 1 for each group G_i, and |S| <= k.
 * natural-greedy algorithm:
 * -Y = ∅
 * -D = {Group_1, ..., Group_k}
 * -For i = 1, 2, · · · , k do
   --------E = {v \in G_i| G_i \in D}
   --------x = arg max v∈E f(v|Y)
   --------Y = Y ∪ {x}
   -return a set Y
 */

#include "Function.h"
#include <vector>
#include <algorithm>
#include "TIMCoverage.h"
using namespace std;

class NaturalGreedy {
private:
    Function& f;
    vector<vector<int>> myGroups;
    int k;

public:
    NaturalGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k);
    ~NaturalGreedy();
    vector<int> naturalGreedyAlgorithmForReachabilityFunction(double &functionValueOfY);
    vector<int> naturalGreedyAlgorithmForInfluenceFunction(double &functionValueOfY);
    vector<int> naturalGreedyAlgorithmForCoverFunction(double& functionValueOfY);
    vector<int> naturalGreedyAlgorithmReturnWithOrder(double& functionValueOfY, vector<int> order);
};





#endif //UNTITLED_NATURALGREEDY_H
