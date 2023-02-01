//
// Created by me on 5/26/22.
//

#ifndef UNTITLED_TIMCOVERAGE_H
#define UNTITLED_TIMCOVERAGE_H


/*
 * This class is used to find the best seed set for the k-greedy algorithm
 */

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <queue>
#include <set>
#include <math.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct QueueComparator {
    bool operator()(pair<int, int> a, pair<int, int> b)
    {
        return a.second < b.second;
    }
};

struct ReverseQueueComparator {
    bool operator()(pair<int, int> a, pair<int, int> b)
    {
        return a.second > b.second;
    }
};

class NodeChecker {
public:
    virtual bool isNodeValid(int nodeID)=0;  //??
};

class TIMCoverage {
private:
    int numberOfRRSetsCovered;
    priority_queue<pair<int, int>, vector<pair<int, int>>, ReverseQueueComparator> reverseQueue;
    vector<bool> nodeMark;  //true means this node is eligible to be selected as seed
    vector<bool> edgeMark; //false means this RR set is not covered.
    vector<int> coverage;
    vector<vector<int>> *lookupTable;
    int R;
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator> queue;
    vector<vector<int>> myGroups;
    vector<int> groupMembership;
    int groundSetSize;
public:
    TIMCoverage(vector<vector<int>> *lookupTable);
//    TIMCoverage();
    TIMCoverage( const TIMCoverage &obj);
    ~TIMCoverage();
    void initializeLookupTable(vector<vector<int>>* randomRRSets, int n) ;
    void clearLookupTable(int n);
    void initializeDataStructures(int R, int n);
    void clearDataStructures();



    int countForVertex(int u);
    void decrementCountForVertex(int u, int setID);
    void incrementCountForVertex(int u, int randomRRSetID);
    int numberOfNewRRSetsCoveredByVertex(int vertex);
    void incrementRRSetsCovered(int number);
    vector<int> getRRSetsCoveredByVertex(int vertex);

    double marginalGainWithVertex(int vertex, double scalingFactor);
    void offsetCoverage(int vertex, int offset);
    void updatePriorityQueueWithCurrentValues();

    set<int> findTopKNodesForBeliefMaximization(int k, vector<vector<int>>* rrSets);
    vector<int> findTopKNodesModular(int k);

    //used for normal influence maximization problem
    pair<int, double> findMaxInfluentialNodeAndUpdateModel(vector<vector<int>> *rrSets) ;
    pair<int, double> findMaxInfluentialNodeAndUpdateModel(vector<vector<int>> *rrSets, NodeChecker *nodeChecker);
    vector<int> findTopKNodesForInfluenceMaximization(int k, vector<vector<int>>* rrSets);

    //used for k-greedy algo
    TIMCoverage(vector<vector<int>> *lookupTable, vector<vector<int>>& groups, int givenSetSize);
    pair<int, double> findMaxInfluentialNodeAndUpdateModelForKGreedy(vector<vector<int>> *rrSets, int group, vector<int> order) ;
    pair<int, double> findMaxInfluentialNodeAndUpdateModelForKGreedy(vector<vector<int>> *rrSets, NodeChecker *nodeChecker, int group, vector<int> order);
    vector<int> findTopKNodesForKGreedyOneOrder(int k, vector<vector<int>> *rrSets, vector<int>& order, double& influence);

    //used for natural greedy algo
    pair<int, double> findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(vector<vector<int>> *rrSets) ;
    pair<int, double> findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(vector<vector<int>> *rrSets, NodeChecker *nodeChecker);
    vector<int> findTopKNodesForNaturalGreedy(int k, vector<vector<int>> *rrSets,double& influence);

    //used for AssignGroups() purposes
    double marginalGainOfNodeWithRespectToEmax(int v);

    set<pair<int, int>> findTopKNodesWithInfluence(int k, vector<vector<int>> *rrSets);
    pair<vector<int>,int> findTopKNodesFromCandidatesWithoutUpdate(int k, vector<vector<int>> *rrSets, set<int> candidateNodes);
    void constructReverseQueue();


    void addToSeed(int vertex, vector<vector<int>> *rrSets);
    double findInfluence(set<int> seedSet, double scalingFactor);
    int getNumberOfRRSetsCovered();

    vector<bool>* getNodeMark();
    vector<bool>* getEdgeMark();
    vector<vector<int>>* getLookupTable();


    TIMCoverage& operator=( const TIMCoverage &obj);

    //methods for debugging
    void initialize1(int n);
    void initialize2(vector<vector<int>>* randomRRSets);
    void initialize3(vector<vector<int>>* randomRRSets, int rrSetID);

};


#endif //UNTITLED_TIMCOVERAGE_H
