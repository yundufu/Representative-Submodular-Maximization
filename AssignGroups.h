//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_ASSIGNGROUPS_H
#define UNTITLED_ASSIGNGROUPS_H


/*
 * This class implements multiple ways to assign groups for a ground set V ⊆ N,
 * i.e., partitioning them into disjoint groups.
 * Assume V = {0, 1, ..., givenSetSize -1}
 * Some of the strategies do not need the information of the given function.
 * Some of them need calling the given function for function values and marginal gain values.
 */

#include "Function.h"
#include "InfluenceFunction.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "NaturalGreedy.h"
#include "Graph.h"
#include "TIMCoverage.h"

using namespace  std;

class AssignGroups {
public:
    AssignGroups(int givenSetSize, int givenNumGroups, Function& givenFunction);
    vector<vector<int>> assignGroupsRandomly();
    vector<vector<int>> assignGroupsByFunctionValue();
    vector<vector<int>> assignGroupsMixedStrategy(int topK);
    vector<vector<int>> assignGroupsForInfluenceFunctionTwoGroups();
    vector<vector<int>> assignTwoGroupsForInfluenceFunctionSubmodularly();
    vector<vector<int>> assignGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(int numGroups, double percent);
    vector<vector<int>> assignGroupsAccordingToMarginalGainWithRespectToEmaxForCoverFunction(int numGroups, double percent);
    vector<vector<int>> assignTwoGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(double percent);
    vector<vector<int>> assignTwoGroupsAccordingToMarginalGainWithRespectToEmaxForCoverFunction(double percent);
    vector<vector<int>> assignGroupsAccordingToMarginalGainWithRespectToEmaxForReachabilityFunction(int numGroups, double percent);
    vector<vector<int>> assignGroupsAlternatively();
    vector<vector<int>> assignGroupsFromRandomSourceAndReachability(vector<vector<int>>* graph);
    vector<vector<int>> assignGroupsMixedModularAndSubModularForInfluenceFunction(int num);
    vector<vector<int>> assignGroupsMixedModularAndSubModularForSimpleFunctions(int num);
    ~AssignGroups();
//    bool sortbysecdesc(const pair<double,int> &a, const pair<double,int> &b);


private:
    int groundSetSize;
    int numGroups;
    Function& f;
};




#endif //UNTITLED_ASSIGNGROUPS_H
