


/*
 *
 * This program reads a graph, computes the corresponding RRSets and implements the influence function
 * under independent cascade model.
 */

#include "InfluenceFunction.h"
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <random>
#include <algorithm>




InfluenceFunction::InfluenceFunction(string graphName)
{//use default uniform edge probability 1/ indegree
    srand (time(0));
    myGraph = make_unique<Graph>(graphName);//for RRSets-based algo
    myGraph->readGraph();
    n  = myGraph->getNumberOfVertices();
    m = myGraph->getNumberOfEdges();  //number of edges in graph
    double epsilon = 2;
    R =  (8 + 2 * epsilon) * n * (2 * log(n) + log(2)) / (epsilon * epsilon);
    myGraph->generateRandomRRSetsForInfluenceMaximization(R);
    cout << R << "RRsets are generated" << endl;
//    lookupTable = computeLookupTable(myGraph->getRandomRRSetsForInfluenceMaximization(), n);
}

InfluenceFunction::InfluenceFunction(string graphName, double edgeProbability)
{
    srand (time(0));
    myGraph = make_unique<Graph>(graphName);//for RRSets-based algo
    myGraph->setPropogationProbability(edgeProbability); //set uniform edge probability
    myGraph->readGraph();
    n  = myGraph->getNumberOfVertices();
    m = myGraph->getNumberOfEdges();  //number of edges in graph
    double epsilon = 2;
    R =  (8 + 2 * epsilon) * n * (2 * log(n) + log(2)) / (epsilon * epsilon);
    myGraph->generateRandomRRSetsForInfluenceMaximization(R);
    cout << R << "RRsets are generated" << endl;
//    lookupTable = computeLookupTable(myGraph->getRandomRRSetsForInfluenceMaximization(), n);
}


//return sigma({node}) under IC model
double InfluenceFunction::functionValue(int node)
{
    //find the number of RRsets that contain node
    int diffusionNum = 0;
    for(vector<int> v : myGraph->rrSetsForInfluenceMaximization){
        if(find(v.begin(), v.end(), node) != v.end()){ //this RRSet intersect with {node}
            diffusionNum++;
        }
    }


//    int diffusionNum = lookupTable[node].size();
//    //(*lookupTable)[node] is a vector<int> that contains the set of RRSet IDs that contain node

    return (1.0*diffusionNum / R) * n;
}

//pass the seedSet aSet as parameter, and return sigma(S) under IC model
double InfluenceFunction::functionValue(vector<int> aSet)
{
    //find the number of RRsets that contain some node in aSet
    int diffusionNum = 0;
    for(vector<int> v : myGraph->rrSetsForInfluenceMaximization){
        for(int j : v){
            if(find(aSet.begin(), aSet.end(), j) != aSet.end()){ //this RRSet intersect with S
                diffusionNum++;
                break;
            }
        }
    }


//    set<int> RRSetIDsIntersectASet;
//    for(int node : aSet)
//    {
//        for(int ID : lookupTable[node])
//        {//(*lookupTable)[node] is a vector<int> that contains the set of RRSet IDs that contain node
//            RRSetIDsIntersectASet.insert(ID);
//        }
//    }
//    int  diffusionNum = RRSetIDsIntersectASet.size();

    return (1.0 * diffusionNum / R) * n;

}



//vector<vector<int>> InfluenceFunction::computeLookupTable(vector<vector<int>>* randomRRSets, int n)
//{
//    vector<vector<int>> lookupTable(n);
//    for(int rrSetID = 0; rrSetID < randomRRSets->size(); rrSetID++) {
//        for(int vertex: (*randomRRSets)[rrSetID]) {
//            lookupTable[vertex].push_back(rrSetID);
//        }
//    }
//    return lookupTable;
//}


vector<vector<int>>* InfluenceFunction::getRandomRRSetsForInfluenceMaximization() {
    return myGraph->getRandomRRSetsForInfluenceMaximization();
}




int InfluenceFunction::getNumOfRRSets()
{
    return R;
}


double InfluenceFunction::marginalGain(int v, vector<int> aSet)  //returns a value of f(aSet ∪ {v}) - f(aSet)
{
    int diffusionNum = 0;
    for(vector<int> vec : myGraph->rrSetsForInfluenceMaximization){
        if(find(vec.begin(), vec.end(), v) != vec.end())  //This RR set contains v
        {
            bool intersectWithaSet = false;
            for(int j : vec)
            {
                if(find(aSet.begin(), aSet.end(), j) != aSet.end()){ //this RRSet intersect with aSet
                    intersectWithaSet = true;
                    break;
                }
            }
            if(!intersectWithaSet)
            {
                diffusionNum++;
            }
        }
    }


////compute the set of RRSets that contain v but not intersect with aSet
//    vector<int> RRSetIDsContainingV = lookupTable[v];
//    set<int> RRSetIDsIntersectASet;
//    for(int node : aSet)
//    {
//        for(int ID : lookupTable[node])
//        {//(*lookupTable)[node] is a vector<int> that contains the set of RRSet IDs that contain node
//            RRSetIDsIntersectASet.insert(ID);
//        }
//    }
//    int diffusionNum = 0;
//    for(int ID : RRSetIDsContainingV)
//    {
//        if(RRSetIDsIntersectASet.find(ID) == RRSetIDsIntersectASet.end())
//            diffusionNum++;  //this RRset ID is in RRSetIDsContainingV but not in RRSetIDsIntersectASet
//    }

    return (1.0*diffusionNum / R) * n;
}

double InfluenceFunction::marginalGain(vector<int> A, vector<int> aSet) //returns a value of f(aSet ∪ A) - f(aSet)
{
    int diffusionNum = 0;
    for(vector<int> vec : myGraph->rrSetsForInfluenceMaximization){
        if(isIntersected(A, vec))  //This RR set contains intersect with A
        {
            bool intersectWithaSet = false;
            for(int j : vec)
            {
                if(find(aSet.begin(), aSet.end(), j) != aSet.end()){ //this RRSet intersect with aSet
                    intersectWithaSet = true;
                    break;
                }
            }
            if(!intersectWithaSet)
            {
                diffusionNum++;
            }
        }
    }
    return (1.0*diffusionNum / R) * n;

}
InfluenceFunction::~InfluenceFunction()
{

}


void InfluenceFunction::printVector(vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
        cout<<v[i]<<", ";
    cout<<endl;
}


int InfluenceFunction::getGroundSetSize()
{
    return n;
}

int InfluenceFunction::getNumOfEdges()
{
    return m;
}



bool InfluenceFunction::isIntersected(vector<int> small, const vector<int>& big)
{
    for(int ele : small)
    {
        if(find(big.begin(),big.end(),ele) != big.end()) //they intersect
            return true;
    }
    return false;
}


vector<vector<int>>* InfluenceFunction::getGraph()
{
    return myGraph->getGraph();
}