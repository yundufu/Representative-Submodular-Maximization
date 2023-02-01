//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_BRUTEFORCE_H
#define UNTITLED_BRUTEFORCE_H

/*
 * This class implement the brute-force algorithm for the equal submodular optimization problem where
 * we are given a ground set V ⊆ N with k disjoint groups G_1 ∪ ... ∪G_k = V,
 * a monotone submodular function f: 2^V -> R, an integer k, and we aim to find a set S such that
 * f(S) is maximized subject to |S ∩ G_i| = 1 for each group G_i, and |S| <= k.
 * brute-force algorithm:
 * - Z = emptySet;
 * - functionValueOfZ =0;
 * - For each set C ⊆ V such that C∩ G_i = 1 for each G_i:
 * ------If f(C) > functionValueOfZ:
 * ------------Z = C;
 * ------------functionValueOfZ = f(C)
 * -return Z;
 */
#include "Function.h"
#include <vector>
using namespace std;

class BruteForce {
private:
    Function& f;
    vector<vector<int>> myGroups;
    int k;

public:
    BruteForce(Function& givenFunction, vector<vector<int>>& groups, int given_k);
    ~BruteForce();
    vector<int> BruteForceAlgorithm(double& functionValueOfZ);
    vector<vector<int>> allSubset(vector<vector<int>>& groups, int endGroupID);
};


#endif //UNTITLED_BRUTEFORCE_H
