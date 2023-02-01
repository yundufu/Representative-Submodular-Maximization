//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_KGREEDY_H
#define UNTITLED_KGREEDY_H




/*
 * This class implement the k-greedy algorithm for the equal submodular optimization problem where
 * we are given a ground set V ⊆ N with k disjoint groups G_1 ∪ ... ∪G_k = V,
 * a monotone submodular function f: 2^V -> R, an integer k, and we aim to find a set S such that
 * f(S) is maximized subject to |S ∩ G_i| = 1 for each group G_i, and |S| <= k.
 * k-greedy algorithm:
 * -For i = 1, 2, · · · , k do
   ----Let r_i be an ordering of the k groups that starts with G_i and the rest of the groups are
     in increasing group number order.
   ----X_i = ∅
   ----for j = 1, · · · , k do
   --------Let G_{i_j} be the jth element in r_i
   --------x = arg max v∈G_{i_j} f (v|X_i)
   --------X_i = X_i ∪ {x}
   -return a set X = argmax X′∈{X_1,··· ,X_k} f(X′)
 */

#include "Function.h"
#include <vector>
#include "TIMCoverage.h"

using namespace std;

class kGreedy {
private:
    Function& f;
    vector<vector<int>> myGroups;
    int k;

public:
    kGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k);
    ~kGreedy();
    vector<int> kGreedyAlgorithmForReachabilityFunction(double& functionValueOfX);
    vector<int> kGreedyAlgorithmForInfluenceFunction(double& functionValueOfX);
    vector<int> kGreedyAlgorithmForCoverFunction(double& functionValueOfX);//finds the best X with maximized f(X) using k-greedy algorithm
    vector<int> OneGreedyAlgorithm(int order_i, double& functionValueOfX_i);
    //finds the best X_i following the ordering r_i
    // order is the group number that r_i starts with,
    //  eg. r_i = G3, G1, G2, G4, then order = 3

};


#endif //UNTITLED_KGREEDY_H
