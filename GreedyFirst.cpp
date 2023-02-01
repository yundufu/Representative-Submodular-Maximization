////
//// Created by Xiaoyun Fu on 5/9/2022.
////
//
//#include "GreedyFirst.h"
//#include "kGreedy.h"
//#include "MarginalGainFunction.h"
//#include <iostream>
//#include <algorithm>
//#include <unordered_set>
//
//GreedyFirst::GreedyFirst(Function& givenFunction, vector<vector<int>>& groups, int given_k): f(givenFunction){
//    myGroups = groups;
//    k = given_k;
//    l = myGroups.size();
//}
//GreedyFirst::~GreedyFirst(){
//
//}
//
///*
// *  greedy_first algorithm:
// *  X_gf = D = ∅
//    while |X_gf| < k − ℓ + |D| do
//        x = argmaxv∈V \X_gf f (v|X_gf)
//        X_gf = X_gf ∪ {x}
//        D = {Gi|∃x ∈ Gi ∩ X_gf}
//    C = {v ∈ Gi|Gi ∈ D}
//    X_remaining = outputs of running k-greedy algorithm on input  f (·|X′), V \C, ℓ − |D|.
//    return a set X = X_gf ∪ X_remaining
// */
//vector<int> GreedyFirst::greedyFirstAlgorithm(double& functionValueOfX_gf){
//    vector<int> X_gf;
//    unordered_set<int> D;
//    while(X_gf.size() < k - l + D.size())
//    {
//        int groupIDOfX;
//        int x =greedySingleChoiceIgnoreGroupMembership(X_gf, groupIDOfX);
//        X_gf.push_back(x);
//        //find(aSet.begin(), aSet.end(), v) == aSet.end()
//        D.insert(groupIDOfX); // D only contains unique elements
//    }
//    cout << "size of D: " << D.size() << endl;
//    for(int i = l-1; i>= 0; i--) //generate C by removing all covered groups from consideration
//    {
//        if(D.find(i) != D.end()) // group_i is covered
//            myGroups.erase(myGroups.begin() + i);
//    }
//    cout<< "myGroups size = " << myGroups.size() << endl;
//    MarginalGainFunction g(f, X_gf);
//    kGreedy kGreedy(g, myGroups, l-D.size() );
//    double temp;
//    vector<int> X_remaining = kGreedy.kGreedyAlgorithmForInfluenceFunction(temp);
//    cout<<"X_remaining size = " << X_remaining.size() << endl;
//    for(int i = 0; i < X_remaining.size(); i++)
//        X_gf.push_back(X_remaining[i]);
//    functionValueOfX_gf = f.functionValue(X_gf);
//    return X_gf;
//}
//
//int GreedyFirst::greedySingleChoiceIgnoreGroupMembership(vector<int> aSet, int& belongGroupID){
//    int result;
//    double largestMarginalGain = -1;
//    int groupIDOfResult;
//    for(int i = 0; i < myGroups.size(); i++)
//    {
//        for(int v : myGroups[i])
//        {
//            if(find(aSet.begin(), aSet.end(), v) == aSet.end())
//            {  //only select from elements in groundSet\aSet
//                double temp = f.marginalGain(v,aSet);
//                if(temp > largestMarginalGain)
//                {
//                    result = v;
//                    largestMarginalGain = temp;
//                    groupIDOfResult = i;
//                }
//            }
//        }
//    }
//    belongGroupID = groupIDOfResult;
//    return result;
//}
//
//void GreedyFirst::printVector(vector<int> v){
//    for(int i = 0; i < v.size(); i++)
//        cout<<v[i]<<", ";
//    cout<<endl;
//}