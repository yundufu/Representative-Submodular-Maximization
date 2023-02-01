//
// Created by Xiaoyun Fu on 5/5/2022.
//

#include "kGreedy.h"
#include <iostream>

kGreedy::kGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k):f(givenFunction) {
    myGroups = groups;
    k = given_k;
}

kGreedy::~kGreedy()
{

}

vector<int> kGreedy::kGreedyAlgorithmForReachabilityFunction(double& functionValueOfX)
{
    vector<int> bestSet;
    double bestInfluence = -1;
    for(int i = 0; i < k; i++)
    {
        //generate order_i;
        vector<int> order_i;
        order_i.push_back(i);
        for(int j = 0; j < k; j++)
        {
            if(j!= i)
                order_i.push_back(j);
        }

        //Find greedy output seed set following order_i
        vector<vector<int>> lookupTableI;
        TIMCoverage timCoverageI(&lookupTableI, myGroups,f.getGroundSetSize());

        vector<vector<int>> reverseReachableSets = f.getReverseReachableSets();
        int n =f.getGroundSetSize();
        timCoverageI.initializeLookupTable(&reverseReachableSets,n );
        timCoverageI.initializeDataStructures(n, n);
        double currentInfluence;
        vector<int> currentSet = timCoverageI.findTopKNodesForKGreedyOneOrder(k, &reverseReachableSets, order_i,currentInfluence);

        //compare with best seed set
        if(currentInfluence > bestInfluence)
        {
            bestSet = currentSet;
            bestInfluence = currentInfluence;
        }
    }

    functionValueOfX = bestInfluence;
    return bestSet;
}

/*
 * finds the best X with maximized f(X) using k-greedy algorithm
 * value of f(X) is returned implicitly through pass by reference
 */
vector<int> kGreedy::kGreedyAlgorithmForInfluenceFunction(double& functionValueOfX)
{
    vector<int> bestSet;
    double bestInfluence = -1;
    for(int i = 0; i < k; i++)
    {
        //generate order_i;
        vector<int> order_i;
        order_i.push_back(i);
        for(int j = 0; j < k; j++)
        {
            if(j!= i)
                order_i.push_back(j);
        }

        //Find greedy output seed set following order_i
        vector<vector<int>> lookupTableI;
        TIMCoverage timCoverageI(&lookupTableI, myGroups,f.getGroundSetSize());

        vector<vector<int>>* randomRRSetsForInfluenceMaximization = f.getRandomRRSetsForInfluenceMaximization();
        int n =f.getGroundSetSize();
        timCoverageI.initializeLookupTable(randomRRSetsForInfluenceMaximization,n );
        int R = f.getNumOfRRSets();
        timCoverageI.initializeDataStructures(R, n);
        double currentInfluence;
        vector<int> currentSet = timCoverageI.findTopKNodesForKGreedyOneOrder(k, randomRRSetsForInfluenceMaximization, order_i,currentInfluence);

        //compare with best seed set
        if(currentInfluence > bestInfluence)
        {
            bestSet = currentSet;
            bestInfluence = currentInfluence;
        }
    }

    functionValueOfX = bestInfluence;
    return bestSet;
}



vector<int> kGreedy::kGreedyAlgorithmForCoverFunction(double& functionValueOfX)
{
    vector<int> X;
    double largest_function_value_of_X = 0;
    for(int i =  0; i < k; i++)
    {
        double functionValueOfX_i;
        vector<int> X_i = OneGreedyAlgorithm(i,functionValueOfX_i);
        if(functionValueOfX_i > largest_function_value_of_X)
        {
            X = X_i; //need deep copy
            largest_function_value_of_X = functionValueOfX_i;
        }
    }
    functionValueOfX = largest_function_value_of_X;
    return X;
}




/*following the ordering order_i = {i, 1, ..., i-1, i+1, ..., k} to find the greedy solution X_i
 * X_i = ∅
   for j = 0, · · · , k-1 do
   ----Let G_{i_j} be r_i[j]
   ----x = arg max v∈G_{i_j} f (v|X_i)
   ----X_i = X_i ∪ {x}
 * Return X_i
 */
vector<int> kGreedy::OneGreedyAlgorithm(int order_i, double& functionValueOfX_i)
{
//    cout << "In OneGreedyAlgorithm(), checking group " << order_i << endl;
    vector<int> X_i;
    //find x = arg max v∈G_{order_i} f (v)
    int best_x = myGroups[order_i][0];  //take the first element in Group_{order_i} to be the best_x
//    vector<int> tempVector = {best_x};
    clock_t startTime1 = clock(); //using <ctime> library
    double largest_x_value = f.functionValue(best_x);
    clock_t endTime1 = clock();
//    cout << "In kGeedy, time taken for one functionValue() call = " << (endTime1 -startTime1)/ (1.0 * CLOCKS_PER_SEC) << "seconds" << endl;
    for(int v : myGroups[order_i]){
        if(v != myGroups[order_i][0]){
            //Make sure v is not in X_i yet to correctly compute the marginal gain

            double temp = f.marginalGain(v, X_i); //X_i is empty now, safe to call the marginalGain function

            if(temp > largest_x_value) {
                largest_x_value = temp;
                best_x = v;
            }
        }
    }
    X_i.push_back(best_x);

    for(int j = 0; j < k; j++){
        if(j != order_i) //skip G_{order_i} because it is already checked above
        {
//            cout << "In OneGreedyAlgorithm(), checking group " << j << endl;
            //find x = arg max v∈G_j f (v|X_i)
            int best_x_in_G_j = myGroups[j][0];
            //take the first element in Group_j to be the element in Group_j with the largest marginal gain
            clock_t startTime2 = clock();
            double largest_x_marginal_gain_for_G_j = f.marginalGain(best_x_in_G_j, X_i);
            clock_t endTime2 = clock();
//            cout << "In kGeedy, time taken for one marginalGain() call = " << (endTime2 -startTime2) / (1.0 * CLOCKS_PER_SEC) << "seconds" << endl;
            for(int v : myGroups[j]){
                if(v != myGroups[j][0]){
                    double temp = f.marginalGain(v, X_i);
                    if(temp > largest_x_marginal_gain_for_G_j) {
                        largest_x_marginal_gain_for_G_j = temp;
                        best_x_in_G_j = v;
                    }
                }
            }
            X_i.push_back(best_x_in_G_j);
        }
    }
    functionValueOfX_i = f.functionValue(X_i); //implicitly returned through pass by reference
    return X_i;
}


