//
// Created by me on 9/4/22.
//

#ifndef UNTITLED_KFACTORIALGREEDY_H
#define UNTITLED_KFACTORIALGREEDY_H


/*
 * This class implement the k!-greedy algorithm for the equal submodular optimization problem where
 * we are given a ground set V ⊆ N with k disjoint groups G_1 ∪ ... ∪G_k = V,
 * a monotone submodular function f: 2^V -> R, an integer k, and we aim to find a set S such that
 * f(S) is maximized subject to |S ∩ G_i| = 1 for each group G_i, and |S| <= k.
 * k-greedy algorithm:
 * -For i = 1, 2, · · · , k! do
   ----Let r_i be an ordering of the k groups
   ----for all r_i
   --------T_i = ∅
   --------for j = 1, · · · , k do
   ------------Let G_{i_j} be the jth element in r_i
   ------------t = arg max v∈G_{i_j} f (v|T_i)
   ------------T_i = T_i ∪ {t}
   -return a set T = argmax T′∈{T_1,··· ,T_k!} f(T′)
 */

#include "Function.h"
#include <vector>
#include "TIMCoverage.h"

using namespace std;

class kFactorialGreedy {
private:
    Function& f;
    vector<vector<int>> myGroups;
    int k;


public:
    kFactorialGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k);
    ~kFactorialGreedy();
    vector<int> kFactorialGreedyAlgorithmForInfluenceFunction(double& functionValueOfT, ofstream& logbook);//finds the best T with maximized f(X) using k-greedy algorithm
    vector<int> kFactorialGreedyAlgorithmForReachabilityFunction(double& functionValueOfT);
    vector<int> kFactorialGreedyAlgorithmForCoverFunction(double& functionValueOfT);
    vector<int> OneGreedyAlgorithm(vector<int> order_i, double& functionValueOfT_i);
    //finds the best X_i following the ordering r_i
    // order is the group number that r_i starts with,
    //  eg. r_i = G3, G1, G2, G4, then order = 3

    vector<vector<int>> generateKFactorialOrderings();
    void printKFactorialOrderings(vector<vector<int>> orderings);
    void printVector(vector<int> v, ofstream& logbook);

};




#endif //UNTITLED_KFACTORIALGREEDY_H
