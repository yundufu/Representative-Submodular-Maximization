//
// Created by Xiaoyun Fu on 5/6/2022.
//

#include "NaturalGreedy.h"

NaturalGreedy::NaturalGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k):f(givenFunction)
{
    myGroups = groups;
    k = given_k;
}

NaturalGreedy::~NaturalGreedy(){

}

/*
 *  natural-greedy algorithm:
 * -Y = ∅
 *  D = {Group_1, ..., Group_k}
 * -For i = 1, 2, · · · , k do
   ----E = {v \in G_i| G_i \in D}
   ----x = arg max v∈E f(v|Y)
   ----Y = Y ∪ {x}
   ----remove groupID that x belongs to from D
   -return a set Y
 */


vector<int> NaturalGreedy::naturalGreedyAlgorithmForCoverFunction(double& functionValueOfY)
{
    vector<int> Y;
    vector<int> D; // D contains all the group numbers left to choose element from
    for(int i = 0; i < k; i++)
        D.push_back(i);

    for(int i = 0; i < k; i++){ //each iteration finds the best_x with largest marginal gain to current Y
        int best_x;
        double largestMarginalGain = -1;
        int bestGroupID;
        for(int i : D){ //loop through each uncovered groups
            for(int v : myGroups[i]){
                double marginalGainOfV = f.marginalGain(v, Y);
                if(marginalGainOfV > largestMarginalGain){
                    best_x = v;
                    largestMarginalGain = marginalGainOfV;
                    bestGroupID = i;
                }
            }
        }
        Y.push_back(best_x); //found the best_x with largest marginal gain in the uncovered groups
        D.erase(std::remove(D.begin(), D.end(), bestGroupID), D.end());
        // remove the group best_x belongs to from future consideration
    }
    functionValueOfY = f.functionValue(Y);
    return Y;
}

//this method is used for local search algorithm as it needs the node group membership
vector<int> NaturalGreedy::naturalGreedyAlgorithmReturnWithOrder(double& functionValueOfY, vector<int> order)
{
    vector<int> Y;
    vector<int> D; // D contains all the group numbers left to choose element from
    for(int i = 0; i < k; i++)
        D.push_back(i);

    for(int i = 0; i < k; i++){ //each iteration finds the best_x with largest marginal gain to current Y
        int best_x;
        double largestMarginalGain = -1;
        int bestGroupID;
        for(int i : D){ //loop through each uncovered groups
            for(int v : myGroups[i]){
                double marginalGainOfV = f.marginalGain(v, Y);
                if(marginalGainOfV > largestMarginalGain){
                    best_x = v;
                    largestMarginalGain = marginalGainOfV;
                    bestGroupID = i;
                }
            }
        }
        Y.push_back(best_x); //found the best_x with largest marginal gain in the uncovered groups
        order.push_back(bestGroupID);
        D.erase(std::remove(D.begin(), D.end(), bestGroupID), D.end());
        // remove the group best_x belongs to from future consideration
    }
    functionValueOfY = f.functionValue(Y);
    return Y;
}



vector<int> NaturalGreedy::naturalGreedyAlgorithmForInfluenceFunction(double &functionValueOfY)
{
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverageI(&lookupTableI, myGroups,f.getGroundSetSize());

    vector<vector<int>>* randomRRSetsForInfluenceMaximization = f.getRandomRRSetsForInfluenceMaximization();
    int n =f.getGroundSetSize();
    timCoverageI.initializeLookupTable(randomRRSetsForInfluenceMaximization,n );
    int R = f.getNumOfRRSets();
    timCoverageI.initializeDataStructures(R, n);
    double bestInfluence;
    vector<int> bestSet = timCoverageI.findTopKNodesForNaturalGreedy(k, randomRRSetsForInfluenceMaximization, bestInfluence);

    functionValueOfY = bestInfluence;
    return bestSet;
}


vector<int> NaturalGreedy::naturalGreedyAlgorithmForReachabilityFunction(double &functionValueOfY)
{
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverageI(&lookupTableI, myGroups,f.getGroundSetSize());

    vector<vector<int>> reverseReachableSets = f.getReverseReachableSets();
    int n =f.getGroundSetSize();
    timCoverageI.initializeLookupTable(&reverseReachableSets,n );
    timCoverageI.initializeDataStructures(n, n);
    double bestInfluence;
    vector<int> bestSet = timCoverageI.findTopKNodesForNaturalGreedy(k, &reverseReachableSets, bestInfluence);

    functionValueOfY = bestInfluence;
    return bestSet;
}