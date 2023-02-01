//
// Created by Xiaoyun Fu on 8/29/2022.
//

#include "ReachabilityFunction.h"


ReachabilityFunction::ReachabilityFunction(string graphName)
{
    myGraph = make_unique<Graph>(graphName);//for RRSets-based algo
    myGraph->readGraph();
    n  = myGraph->getNumberOfVertices();
    m = myGraph->getNumberOfEdges();  //number of edges in graph
    generateReachableSets();
    cout << "reachableSets generated" << endl;
    generateReverseReachableSets();
    cout << "reverse reachableSets generated" << endl;
}

//returns the number of nodes (including all nodes in aSet) that are reachable from aSet in myGraph
double ReachabilityFunction::functionValue(vector<int> aSet)
{
    set<int> reachableNodes;
    for(int node : aSet)
    {
        for(int neighbor: reachableSets[node])
        {
            reachableNodes.insert(neighbor);
        }
    }
    return reachableNodes.size();
}

//return the number of nodes (including node itself) that are reachable from node in myGraph
double ReachabilityFunction::functionValue(int node)
{
    return reachableSets[node].size();
}

double ReachabilityFunction::marginalGain(int v, vector<int> aSet)  //returns a value of f(aSet ∪ {v}) - f(aSet)
{
    set<int> reachableNodes;
    for(int node : aSet) //compute all the reachable nodes from aSet
    {
        for(int neighbor: reachableSets[node])
        {
            reachableNodes.insert(neighbor);
        }
    }
    double smaller = reachableNodes.size();
    for(int neighbor: reachableSets[v])  //compute all the reachable nodes from aSet U {v}
    {
        reachableNodes.insert(neighbor);
    }
    return reachableNodes.size() - smaller;
}

//returns the value of f(aSet ∪ A) - f(aSet)
double ReachabilityFunction::marginalGain(vector<int> A, vector<int> aSet)
{
    set<int> reachableNodes;
    for(int node : aSet) //compute all the reachable nodes from aSet
    {
        for(int neighbor: reachableSets[node])
        {
            reachableNodes.insert(neighbor);
        }
    }
    double smaller = reachableNodes.size();
    for(int node : A) //compute all the reachable nodes from aSet U A
    {
        for(int neighbor: reachableSets[node])
        {
            reachableNodes.insert(neighbor);
        }
    }
    return reachableNodes.size() - smaller;
}


ReachabilityFunction::~ReachabilityFunction()
{

}

int ReachabilityFunction::getGroundSetSize()
{
    return n;
}
int ReachabilityFunction::getNumOfEdges()
{
    return m;
}
vector<vector<int>>* ReachabilityFunction::getGraph()
{
    return myGraph->getGraph();
}




void ReachabilityFunction::generateReachableSets()
{
    vector<vector<int>>* graph = myGraph->getGraph();
    for(int i = 0; i < n; i++)
    {
        vector<int> empty;
        reachableSets.push_back(empty);
    }

    for(int i = 0; i < n; i++) //generate the reachable set for each node i
    {
        priority_queue<int> queue;
        set<int> visited;
        reachableSets[i].push_back(i);
        queue.push(i);
        visited.insert(i);
        while(!queue.empty())
        {
            int current = queue.top();
            queue.pop();
            for(int neighbor : (*graph)[current])
            {
                if(visited.count(neighbor) == 0) //neighbor is not visited
                {
                    reachableSets[i].push_back(neighbor);
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }
    }
}

void ReachabilityFunction::generateReverseReachableSets()
{
    vector<vector<int>>* graph = myGraph->getGraphTranspose();
    for(int i = 0; i < n; i++)
    {
        vector<int> empty;
        reverseReachableSets.push_back(empty);
    }

    for(int i = 0; i < n; i++) //generate the reachable set for each node i
    {
        priority_queue<int> queue;
        set<int> visited;
        reverseReachableSets[i].push_back(i);
        queue.push(i);
        visited.insert(i);
        while(!queue.empty())
        {
            int current = queue.top();
            queue.pop();
            for(int neighbor : (*graph)[current])
            {
                if(visited.count(neighbor) == 0) //neighbor is not visited
                {
                    reverseReachableSets[i].push_back(neighbor);
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }
    }
}

vector<vector<int>> ReachabilityFunction::getReachableSets()
{
    return reachableSets;
}

vector<vector<int>> ReachabilityFunction::getReverseReachableSets()
{
    return reverseReachableSets;
}

void ReachabilityFunction::printVector(vector<int> v)
{
    for(int i = 0; i < v.size(); i++)
        cout<<v[i]<<", ";
    cout<<endl;
}