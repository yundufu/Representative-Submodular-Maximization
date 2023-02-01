////
//// Created by me on 5/11/22.
////
//
//#ifndef UNTITLED_GREEDYFIRST_H
//#define UNTITLED_GREEDYFIRST_H
//
//
///*
// * This class implement the Fairness_first algorithm for the relaxed equal submodular optimization problem where
// * we are given a ground set V ⊆ N with l disjoint groups G_1 ∪ ... ∪ G_l = V,
// * a monotone submodular function f: 2^V -> R, an integer k >= l, and we aim to find a set S such that
// * f(S) is maximized subject to |S ∩ G_i| >= 1 for each group G_i, and |S| <= k.
// * greedy_first algorithm:
// *  X_gf = D = ∅
//    while |X_gf| < k − ℓ + |D| do
//        x = argmaxv∈V \X_gf f (v|X_gf)
//        X_gf = X_gf ∪ {x}
//        D = {Gi|∃x ∈ Gi ∩ X_gf}
//    C = {v ∈ Gi|Gi ∈ D}
//    X_remaining = outputs of running k-greedy algorithm on input V \C, f (·|X′), ℓ − |D|.
//    return a set X = X_gf ∪ X_remaining
// */
//
//#include "Function.h"
//
//using namespace std;
//
//class GreedyFirst {
//private:
//    Function& f;
//    vector<vector<int>> myGroups;
//    int k;
//    int l; //should be equal to myGroups.size()
//
//
//public:
//    GreedyFirst(Function& givenFunction, vector<vector<int>>& groups, int given_k);
//    ~GreedyFirst();
//    vector<int> greedyFirstAlgorithm(double& functionValueOfX_gf);
//    int greedySingleChoiceIgnoreGroupMembership(vector<int> aSet, int& belongGroupID);
//    void printVector(vector<int> v);
//};
//
//
//
//#endif //UNTITLED_GREEDYFIRST_H
