//
// Created by Xiaoyun Fu on 9/2/2022.
//

#include "kFactorialGreedy.h"
#include <bits/stdc++.h>

kFactorialGreedy::kFactorialGreedy(Function& givenFunction, vector<vector<int>>& groups, int given_k):f(givenFunction)
{
    myGroups = groups;
    k = given_k;
}
kFactorialGreedy::~kFactorialGreedy()
{

}


vector<int> kFactorialGreedy::kFactorialGreedyAlgorithmForInfluenceFunction(double& functionValueOfT, ofstream& logbook)
{
    vector<int> bestSet;
    double bestInfluence = -1;
    vector<vector<int>> orders = generateKFactorialOrderings();
    for(int i = 0; i < orders.size(); i++)  //check each of the k! orders
    {
        //generate order_i;
        vector<int> order_i = orders[i];

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
        logbook << "order_" << i << " = ";
        printVector(order_i, logbook);
        logbook << "T_" << i << " = ";
        printVector(currentSet, logbook);
        logbook << "functionValueOfT_" << i << " = " << currentInfluence << endl;

    }

    functionValueOfT = bestInfluence;
    return bestSet;

}

vector<int> kFactorialGreedy::kFactorialGreedyAlgorithmForCoverFunction(double& functionValueOfT)//finds the best T with maximized f(X) using k-greedy algorithm
{
    vector<vector<int>> orderings = generateKFactorialOrderings();
    vector<int> T;
    double largest_function_value_of_T = 0;
    for(int i =  0; i < orderings.size(); i++)
    {
        double functionValueOfT_i;
        vector<int> T_i = OneGreedyAlgorithm(orderings[i],functionValueOfT_i);
        if(functionValueOfT_i > largest_function_value_of_T)
        {
            T = T_i; //need deep copy
            largest_function_value_of_T = functionValueOfT_i;
        }
    }
    functionValueOfT = largest_function_value_of_T;
    return T;

}

/*following the ordering order_i = {i_1, i_2, ..., i_k} to find the greedy solution T_i
 * T_i = ∅
   for j = 0, · · · , k-1 do
   ----Let G_{i_j} be r_i[j]
   ----t = arg max v∈G_{i_j} f (v|T_i)
   ----T_i = T_i ∪ {t}
 * Return T_i
 */
vector<int> kFactorialGreedy::OneGreedyAlgorithm(vector<int> order_i, double& functionValueOfT_i)
{
    vector<int> T_i;
    //find x = arg max v∈G_{order_i} f (v)
    for(int j = 0; j < order_i.size(); j++)
    {
        int best_x_in_G_j  = INT_MIN;
        double largest_x_marginal_gain_for_G_j = INT_MIN;
//        cout << "G[" << order_i[j] <<"], ";
        for(int v : myGroups[order_i[j]])
        {
            double temp = f.marginalGain(v, T_i);
            if(temp > largest_x_marginal_gain_for_G_j) {
                largest_x_marginal_gain_for_G_j = temp;
                best_x_in_G_j = v;
            }
        }
        T_i.push_back(best_x_in_G_j);
    }
    functionValueOfT_i = f.functionValue(T_i); //implicitly returned through pass by reference
//    cout << "order for T_i";
//    printVector(order_i);
//    cout << "Ti = ";
//    printVector(T_i);
//    cout << "functionValueOfT_i = " << functionValueOfT_i << endl;
    return T_i;
}

vector<int> kFactorialGreedy::kFactorialGreedyAlgorithmForReachabilityFunction(double& functionValueOfT)
{
    vector<vector<int>> orderings = generateKFactorialOrderings();
    vector<int> T;
    double largest_function_value_of_T = 0;
    for(int i =  0; i < orderings.size(); i++)
    {
        double functionValueOfT_i;
        vector<int> T_i = OneGreedyAlgorithm(orderings[i],functionValueOfT_i);
        if(functionValueOfT_i > largest_function_value_of_T)
        {
            T = T_i; //need deep copy
            largest_function_value_of_T = functionValueOfT_i;
        }
    }
    functionValueOfT = largest_function_value_of_T;
    return T;
}

vector<vector<int>> kFactorialGreedy::generateKFactorialOrderings()
{
    vector<vector<int>> results;
    vector<int> oneOrder;
    for(int i = 0; i < k; i++)
        oneOrder.push_back(i);
    results.push_back(oneOrder);
    while(next_permutation(oneOrder.begin(), oneOrder.end()))
    {
        results.push_back(oneOrder);
    }
    return results;
}


void kFactorialGreedy::printKFactorialOrderings(vector<vector<int>> orderings)
{
    cout << "[";
    for(int i = 0; i < orderings.size(); i++)
    {
        cout << "[";
        for(int j = 0; j < orderings[i].size(); j++)
            cout << orderings[i][j] << ", ";
        cout<<"]" << endl;
    }
    cout << " ]" << endl;
}

void kFactorialGreedy::printVector(vector<int> v, ofstream& logbook)
{
    logbook <<"[";
    for(int i = 0; i < v.size(); i++)
        logbook << v[i] << ",";
    logbook <<"]" << endl;
}