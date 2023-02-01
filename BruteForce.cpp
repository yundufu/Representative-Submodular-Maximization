//
// Created by Xiaoyun Fu on 5/7/2022.
//

#include "BruteForce.h"
#include <iostream>

BruteForce::BruteForce(Function& givenFunction, vector<vector<int>>& groups, int given_k):f(givenFunction){
    myGroups = groups;
    k = given_k;
}

BruteForce::~BruteForce(){

}

/*
 * - Z = emptySet;
 * - functionValueOfZ =0;
 * - For each set C ⊆ V such that C ∩ G_i = 1 for each G_i:
 * ------If f(C) > functionValueOfZ:
 * ------------Z = C;
 * ------------functionValueOfZ = f(C)
 * -return Z;
 */
vector<int> BruteForce::BruteForceAlgorithm(double& functionValueOfZ){
    vector<int> Z;
    double currentBest = 0;
    vector<vector<int>> collection = allSubset(myGroups, myGroups.size()-1);
    for(vector<int> C: collection)
    {
        double temp = f.functionValue(C);
        if(temp > currentBest)
        {
            Z = C;
            currentBest = temp;
        }
    }
    functionValueOfZ = currentBest;
    return Z;
}

//Generate all subsets C such that C ∩ G_i = 1 for each G_startGroupID,...,G_k  in groups
vector<vector<int>> BruteForce::allSubset(vector<vector<int>>& groups, int endGroupID)
{
    if(endGroupID == 0) //base case
    {  //only one group(the last group in groups) left to pick elements from
        vector<vector<int>> collection;
        for(int i = 0; i < groups[0].size(); i++)
            collection.push_back({groups[0][i]});
        return collection;
    }
    else if (endGroupID > 0){//recursive case
        //get the collection of subsets C' such that C' ∩ G_i = 1 for each G_{startGroupID+1},...,G_k  in groups startGroupID+1
        vector<vector<int>> subCollection = allSubset(groups, endGroupID -1);

        //append each element in G_startGroupID to each C'
        int numOfSubsets = groups[endGroupID].size() * subCollection.size();
//        cout<<"numOfSubsets = " << numOfSubsets << endl;
        vector<vector<int>> collection(numOfSubsets);
        for(int i = 0; i < subCollection.size(); i++)
        {
            for(int j = 0; j < groups[endGroupID].size(); j++)
            {
                vector<int> temp = subCollection[i];
                temp.push_back(groups[endGroupID][j]);
//                for(int x = 0; x < temp.size(); x++)
//                    cout << temp[x] << ", ";
//                cout<<endl<<endl;
                collection[i*groups[endGroupID].size() + j] = temp;
            }
        }
        return collection;
    }
}



