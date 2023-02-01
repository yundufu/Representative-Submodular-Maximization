////
//// Created by Xiaoyun Fu on 5/9/2022.
////
//
//#include "FairnessFirst.h"
//#include <algorithm>
//#include <iostream>
//
//FairnessFirst::FairnessFirst(Function& givenFunction, vector<vector<int>>& groups, int given_k): f(givenFunction)
//{
//    myGroups = groups;
//    k = given_k;
//    l = myGroups.size();
//}
//
//FairnessFirst::~FairnessFirst(){}
//
///*
// * -X_gf = outputs of running k-greedy algorithm on input V, f (·), ℓ
// * -while |X_gf| < k do
//   ----x = arg max v∈V\X_gf f (v|X_gf)
//   ----X_gf = X_gf ∪ {x}
//   -return a set X_gf
// */
//vector<int> FairnessFirst::fairnessFirstAlgorithm(double& functionValueOfX_gf){
//    kGreedy kGreedy(f,myGroups,l);
//    double temp;
//    vector<int> X_ff = kGreedy.kGreedyAlgorithmForInfluenceFunction(temp);
//    for(int i = 0; i < k-l; i++)
//    {
//        int x = greedySingleChoiceIgnoreGroupMembership(X_ff);
//        X_ff.push_back(x);
//    }
//    functionValueOfX_gf = f.functionValue(X_ff);
//    return X_ff;
//}
//
////ground set is myGroups, returns x = arg max v∈groundSet\aSet f (v|aSet)
//int FairnessFirst::greedySingleChoiceIgnoreGroupMembership(vector<int> aSet)
//{
//    int result;
//    double largestMarginalGain = -1;
//    for(vector<int> vec : myGroups)
//    {
//        for(int v : vec)
//        {
//            if(find(aSet.begin(), aSet.end(), v) == aSet.end())
//            {  //only select from elements in groundSet\aSet
//                double temp = f.marginalGain(v,aSet);
//                if(temp > largestMarginalGain)
//                {
//                    result = v;
//                    largestMarginalGain = temp;
//                }
//            }
//        }
//    }
//    return result;
//}
//
//
//void FairnessFirst::printVector(vector<int> v)
//{
//    for(int i = 0; i < v.size(); i++)
//        cout<<v[i]<<", ";
//    cout<<endl;
//}