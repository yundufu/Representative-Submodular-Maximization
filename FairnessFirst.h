////
//// Created by me on 5/11/22.
////
//
//#ifndef UNTITLED_FAIRNESSFIRST_H
//#define UNTITLED_FAIRNESSFIRST_H
//
///*
// * This class implement the Fairness_first algorithm for the relaxed equal submodular optimization problem where
// * we are given a ground set V ⊆ N with l disjoint groups G_1 ∪ ... ∪G_l = V,
// * a monotone submodular function f: 2^V -> R, an integer k >= l, and we aim to find a set S such that
// * f(S) is maximized subject to |S ∩ G_i| >= 1 for each group G_i, and |S| <= k.
// * Fairness_first algorithm:
// * -X_gf = outputs of running k-greedy algorithm on input V, f (·), ℓ
// * -while |X_gf| < k do
//   ----x = arg max v∈V\X_gf f (v|X_gf)
//   ----X_gf = X_gf ∪ {x}
//   -return a set X_gf
// */
//
//#include "kGreedy.h"
//#include "Function.h"
//
//class FairnessFirst {
//private:
//    Function& f;
//    vector<vector<int>> myGroups;
//    int k;
//    int l; //should be equal to myGroups.size()
//
//
//public:
//    FairnessFirst(Function& givenFunction, vector<vector<int>>& groups, int given_k);
//    ~FairnessFirst();
//    vector<int> fairnessFirstAlgorithm(double& functionValueOfX_gf);
//    int greedySingleChoiceIgnoreGroupMembership(vector<int> aSet);
//    void printVector(vector<int> v);
//};
//
//
//
//#endif //UNTITLED_FAIRNESSFIRST_H
