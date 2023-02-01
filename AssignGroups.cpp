//
// Created by Xiaoyun Fu on 5/6/2022.
//

#include "AssignGroups.h"
#include <utility>
#include <algorithm>
#include <iostream>
#include "TIMCoverage.h"
#include <random>
#include <cassert>


using namespace std;

AssignGroups::AssignGroups(int givenSetSize, int givenNumGroups, Function& givenFunction): f(givenFunction)
{
    srand (time(NULL));
    groundSetSize = givenSetSize;
    numGroups = givenNumGroups;
}

AssignGroups::~AssignGroups()
{}


//Assume ground set V = {0, 1, ..., setSize -1}, partition them into numGroups groups randomly
//The givenFunction can be any function.
vector<vector<int>> AssignGroups::assignGroupsRandomly()
{
    vector<vector<int>> groups(numGroups);
    for(int v = 0; v < groundSetSize; v++)
    {
        int groupIDForV = rand() % numGroups;
        groups[groupIDForV].push_back(v);
    }
    for(int i = 0; i < numGroups; i++)
    {
        if(groups[i].size() == 0)
            groups.erase(groups.begin() + i);
    }
    return groups;
}


//Partition the elements into equal-sized groups in descending function value of each node
//The givenFunction can be any function.
vector<vector<int>> AssignGroups::assignGroupsByFunctionValue()
{
    vector<vector<int>> groups;
    vector<pair<double, int>> allElements;
    for(int i = 0; i < groundSetSize; i++)
    {
        double value = f.functionValue({i});
        pair<double, int> aPair(value, i);
        allElements.push_back(aPair);
    }
    sort(allElements.begin(), allElements.end(), greater<pair<double,int>>());

    int eachGroupSize = groundSetSize / numGroups;
    for(int i = 0; i < numGroups - 1; i++)
    {
        vector<int> temp;
        for(int j = eachGroupSize * i; j < eachGroupSize * (i + 1); j++)
            temp.push_back(allElements[j].second);
        groups.push_back(temp);
        cout << "size of group[" << i << "] = " << temp.size() << endl;
    }

    //add the remaining elements to the last group. Doing so is because setSize is not necessarily divisible by numGroups
    vector<int> temp;
    for(int j = eachGroupSize * (numGroups - 1); j < groundSetSize; j++)
        temp.push_back(allElements[j].second);
    groups.push_back(temp);
    cout << "size of group[" << numGroups - 1 << "] = " << temp.size() << endl;
    return groups;
}

//The givenFunction has to be InfluenceFunction
//Use normal influence maximization to find topK seed nodes, put them in group0,
//randomly distribute the rest of the nodes.
vector<vector<int>> AssignGroups::assignGroupsMixedStrategy(int topK)
{
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverageI(&lookupTableI);

    vector<vector<int>>* randomRRSetsForInfluenceMaximization = f.getRandomRRSetsForInfluenceMaximization();
    int n =f.getGroundSetSize();
    timCoverageI.initializeLookupTable(randomRRSetsForInfluenceMaximization,n );
    int R = f.getNumOfRRSets();
    timCoverageI.initializeDataStructures(R, n);
    vector<int> seedSet =  timCoverageI.findTopKNodesForInfluenceMaximization(topK, randomRRSetsForInfluenceMaximization);

    vector<vector<int>> groups(numGroups);
    for(int node : seedSet)
        groups[0].push_back(node);

    for(int v = 0; v < groundSetSize; v++)
    {
        if(find(seedSet.begin(),seedSet.end(), v) == seedSet.end())  //only asign nodes that are not in seedSet
        {
            int groupIDForV = rand() % numGroups;
            groups[groupIDForV].push_back(v);
        }
    }
    for(int i = 0; i < numGroups; i++)
    {
        if(groups[i].size() == 0)
            groups.erase(groups.begin() + i);
    }
    return groups;
}

//The givenFunction has to be InfluenceFunction
//Assign the nodes into 2 groups:
//e* = argmax f(v), x = argmax f(v|e*) are in group 0
//y has the 2nd largest f(v|e*), z has the 3rd largest f(v|e*) are in group 1
//assign the other nodes randomly
vector<vector<int>> AssignGroups::assignGroupsForInfluenceFunctionTwoGroups()
{
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverageI(&lookupTableI);

    vector<vector<int>>* randomRRSetsForInfluenceMaximization = f.getRandomRRSetsForInfluenceMaximization();
    int n =f.getGroundSetSize();
    timCoverageI.initializeLookupTable(randomRRSetsForInfluenceMaximization,n );
    int R = f.getNumOfRRSets();
    timCoverageI.initializeDataStructures(R, n);
    vector<int> seedSet =  timCoverageI.findTopKNodesForInfluenceMaximization(1, randomRRSetsForInfluenceMaximization);

    //find e* and x
    vector<int> candidateGorup0 = timCoverageI.findTopKNodesForInfluenceMaximization(2, randomRRSetsForInfluenceMaximization);
    vector<vector<int>> groups(numGroups);
    for(int node : candidateGorup0)
        groups[0].push_back(node);

    //find y and z for group 1
    vector<int> temp (seedSet.begin(), seedSet.end());

    priority_queue<pair<double, int> > pq;
    for(int node = 0; node < n; node++)
    {
        if(find(candidateGorup0.begin(), candidateGorup0.end(), node) == candidateGorup0.end())
        {
            double value = f.marginalGain(node, temp);
            pair<double, int> aPair(value, node);
            pq.push(aPair);
        }
    }

    int y = pq.top().second;
    pq.pop();
    int z = pq.top().second;
    groups[1].push_back(y);
    groups[1].push_back(z);
    vector<int> selected = candidateGorup0;

    selected.push_back(y);
    selected.push_back(z);
    //assign the other nodes randomly

    for(int v = 0; v < groundSetSize; v++)
    {
        if(find(selected.begin(),selected.end(), v) == selected.end())  //only asign nodes that are not selected
        {
            int groupIDForV = rand() % numGroups;
            groups[groupIDForV].push_back(v);
        }
    }

    //remove empty groups
    for(int i = 0; i < numGroups; i++)
    {
        if(groups[i].size() == 0)
            groups.erase(groups.begin() + i);
    }
    return groups;
}

//The givenFunction can be any function.
//Sort the nodes in function value of each node in decreasing order, assign the top 1 node to G1, the top 2 node to G2
//the top 3 node to G3, and so on.
//Once all groups have been assigned a node, continue this process for the second round, until all nodes have been assigned.
//Some groups may have one fewer node than other groups because the last round is short of nodes
vector<vector<int>> AssignGroups::assignGroupsAlternatively()
{
    cout << "entered assignGroupsAlternatively() " <<endl;
    vector<vector<int>> groups(numGroups);
    priority_queue<pair<double, int>> pq; //max heap ordered by first element, <f(v), v>
    for(int i = 0; i < groundSetSize; i++)
    {
        pq.emplace(make_pair(f.functionValue(i), i));
    }
    while(!pq.empty())
    {
        for(int i = 0; i < numGroups; i++)
        {
            if(!pq.empty())
            {
                groups[i].push_back(pq.top().second);
                pq.pop();
            }
        }
    }
    for(int i = 0; i < numGroups; i++)
    {
        cout << "groups[" << i <<"].size() = " << groups[i].size() << endl;
    }
    cout << "groups are assgined" << endl;
    return groups;
}

//The givenFunction can be any function.
//Randomly select a node v, collect all nodes reachable to v to form a group, stop if collected n/k nodes,
//avoiding nodes that have joined other groups.
//Do this for k-1 groups, and dump everything left to the last group.
//Group size may vary largely depending on the reachability.
vector<vector<int>> AssignGroups::assignGroupsFromRandomSourceAndReachability(vector<vector<int>>*  graph)
{
    vector<vector<int>> groups;
    set<int> visited;
    vector<int> remaining;
    for(int i = 0; i < groundSetSize; i++)
        remaining.push_back(i);

    //form a group in each iteration, form numGroups - 1 groups in total
    for(int i = 0; i < numGroups - 1; i++)
    {
        int source = remaining[rand() % remaining.size()];  //select a random source
        //remove it from remaining nodes
        remaining.erase(remove(remaining.begin(), remaining.end(), source), remaining.end());
        queue<int> queue;
        queue.push(source);
        visited.insert(source);
        vector<int> currentGroup;
        assert(currentGroup.size() == 0);
        currentGroup.push_back(source);
        assert( queue.size() == 1);
        while(!queue.empty())
        {
            int currentNode = queue.front();
            queue.pop();
            for(int neighbor : (*graph)[currentNode])
            {
                if(visited.find(neighbor) == visited.end()) //neighbor is not visited
                {
                    visited.insert(neighbor);
                    queue.push(neighbor);
                    currentGroup.push_back(neighbor);
                    remaining.erase(remove(remaining.begin(), remaining.end(), neighbor), remaining.end());
                }
            }
            if(currentGroup.size() >= groundSetSize/numGroups)
                break;  //break out of the while loop, this group is full
        }
        groups.push_back(currentGroup);
        cout << "groups[" << i << "].size = " << currentGroup.size() << endl;
    }

    //dump the rest to the last group
    groups.push_back(remaining);
    cout << "groups[" << numGroups - 1 << "].size = " << remaining.size() << endl;

    return groups;
}

//The givenFunction has to be InfluenceFunction
//Only generate 2 groups.
//Let x_0, x_1, x_2, x_3 be the top 4 elements in the topK Seed nodes selected submodularly,
//i.e., x_0 = argmax f(v), x_1 = argmax f(v|{x_0}), x_2 = argmax f(v|{x_0, x_1}), x_3 = argmax f(v|{x_0, x_1, x_2})
//Creat two groups. Assign x_0 and x_1 to group 0, assign x_2 and x_3 to group 1, assign the rest of nodes randomly
vector<vector<int>> AssignGroups::assignTwoGroupsForInfluenceFunctionSubmodularly()
{
    vector<vector<int>> groups(2);

    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverage(&lookupTableI);
    timCoverage.initializeLookupTable(f.getRandomRRSetsForInfluenceMaximization(),f.getGroundSetSize() );
    timCoverage.initializeDataStructures(f.getNumOfRRSets(), f.getGroundSetSize());
    vector<int> topFourNodes = timCoverage.findTopKNodesForInfluenceMaximization(4,f.getRandomRRSetsForInfluenceMaximization());

    groups[0].push_back(topFourNodes[0]);
    groups[0].push_back(topFourNodes[1]);
    groups[1].push_back(topFourNodes[2]);
    groups[1].push_back(topFourNodes[3]);

    for(int v = 0; v < groundSetSize; v++)
    {
        if( find(topFourNodes.begin(), topFourNodes.end(), v) == topFourNodes.end())
        {
            int groupIDForV = rand() % 2;
            groups[groupIDForV].push_back(v);
        }
    }
    for(int i = 0; i < numGroups; i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }

    return groups;
}

//Only generate 2 groups.
//The givenFunction has to be Influence Function
//Put e_max in G0, put all v with f(v|e_max) > percent * f(e_max) in G0. Put the rest of the nodes in G1.
vector<vector<int>> AssignGroups::assignTwoGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(double percent)
{
    vector<vector<int>> groups(2);
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverage(&lookupTableI);
    timCoverage.initializeLookupTable(f.getRandomRRSetsForInfluenceMaximization(),f.getGroundSetSize() );
    timCoverage.initializeDataStructures(f.getNumOfRRSets(), f.getGroundSetSize());
    vector<int> emax= timCoverage.findTopKNodesForInfluenceMaximization(1,f.getRandomRRSetsForInfluenceMaximization());
    groups[0].push_back(emax[0]);
    double functionValueOfEmax = f.functionValue(emax);
//    cout << "In AssignGroups:: assignTwoGroupsAccordingToMarginalGainithRespectToEmax(double percent), groundSet Size = " << f.getGroundSetSize()<< endl;
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != emax[0])
        {
            if(timCoverage.marginalGainOfNodeWithRespectToEmax(i) > percent * functionValueOfEmax)
            {
                groups[0].push_back(i);
//                cout << "f(" << i << "|" << emax[0] <<") = " << timCoverage.marginalGainOfNodeWithRespectToEmax(i) << "> " << percent * functionValueOfEmax << endl;
            }
            else
                groups[1].push_back(i);
        }
    }
    for(int i = 0; i < groups.size(); i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }
    return groups;
}

//Generate any number of groups
//The givenFunction has to be Influence Function.
//Put e_max in G0, put all v with f(v|e_max) > percent * f(e_max) in G0. Put the rest of the nodes randomly in the other (numGroups - 1) groups.
vector<vector<int>> AssignGroups::assignGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(int numGroups, double percent)
{
    vector<vector<int>> groups(numGroups);
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverage(&lookupTableI);
    timCoverage.initializeLookupTable(f.getRandomRRSetsForInfluenceMaximization(),f.getGroundSetSize() );
    timCoverage.initializeDataStructures(f.getNumOfRRSets(), f.getGroundSetSize());
    vector<int> emax= timCoverage.findTopKNodesForInfluenceMaximization(1,f.getRandomRRSetsForInfluenceMaximization());
    groups[0].push_back(emax[0]);
    double functionValueOfEmax = f.functionValue(emax);
//    cout << "In AssignGroups:: assignTwoGroupsAccordingToMarginalGainithRespectToEmax(double percent), groundSet Size = " << f.getGroundSetSize()<< endl;
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != emax[0])
        {
            if(timCoverage.marginalGainOfNodeWithRespectToEmax(i) > percent * functionValueOfEmax)
            {
                groups[0].push_back(i);
//                cout << "f(" << i << "|" << emax[0] <<") = " << timCoverage.marginalGainOfNodeWithRespectToEmax(i) << "> " << percent * functionValueOfEmax << endl;
            }
            else
            {
//                //randomly assign this node to the remaining groups
//                int groupID = 1 + rand() % (numGroups-1);
                //randonly assign this node to a group
                int groupID = rand() % numGroups;
                groups[groupID].push_back(i);
            }
        }
    }
    for(int i = 0; i < groups.size(); i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }
    return groups;
}

//Generate any number of groups
//The givenFunction is Reachability Function
//Put e_max in G0, put all v with f(v|e_max) > percent * f(e_max) in G0. Put the rest of the nodes randomly in the other (numGroups - 1) groups.
vector<vector<int>> AssignGroups::assignGroupsAccordingToMarginalGainWithRespectToEmaxForReachabilityFunction(int numGroups, double percent)
{
    vector<vector<int>> groups(numGroups);

    //find e_max with largest f(v)
    int e_max;
    int largestFunctionValue = -1;
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        double currentFunctionValue = f.functionValue(i);
        if(currentFunctionValue > largestFunctionValue)
        {
            e_max = i;
            largestFunctionValue = currentFunctionValue;
        }
    }
    //add e_max to groups[0]
    groups[0].push_back(e_max);

    //compute the marginal gain of each node v with respect to e_max, then assign them to groups accordingly
    vector<int> temp{e_max};
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != e_max)
        {
            if(f.marginalGain(i, temp) > percent * largestFunctionValue )
            {
                groups[0].push_back(i);
//                cout << "f(" << i << "|" << emax[0] <<") = " << timCoverage.marginalGainOfNodeWithRespectToEmax(i) << "> " << percent * functionValueOfEmax << endl;
            }
            else
            { //                //randomly assign this node to the remaining groups
//                int groupID = 1 + rand() % (numGroups-1);
                //randonly assign this node to a group
                int groupID = rand() % numGroups;
                groups[groupID].push_back(i);
            }

        }
    }
    for(int i = 0; i < groups.size(); i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }
    return groups;


}

//Generate any number of groups
//The givenFunction is Cover Function
//Put e_max in G0, put all v with f(v|e_max) > percent * f(e_max) in G0. Put the rest of the nodes randomly in the other (numGroups - 1) groups.
vector<vector<int>> AssignGroups::assignGroupsAccordingToMarginalGainWithRespectToEmaxForCoverFunction(int numGroups, double percent)
{
    vector<vector<int>> groups(numGroups);

    //find e_max with largest f(v)
    int e_max;
    int largestFunctionValue = -1;
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        double currentFunctionValue = f.functionValue(i);
        if(currentFunctionValue > largestFunctionValue)
        {
            e_max = i;
            largestFunctionValue = currentFunctionValue;
        }
    }
    //add e_max to groups[0]
    groups[0].push_back(e_max);

    //compute the marginal gain of each node v with respect to e_max, then assign them to groups accordingly
    vector<int> temp{e_max};
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != e_max)
        {
            if(f.marginalGain(i, temp) > percent * largestFunctionValue )
            {
                groups[0].push_back(i);
//                cout << "f(" << i << "|" << emax[0] <<") = " << timCoverage.marginalGainOfNodeWithRespectToEmax(i) << "> " << percent * functionValueOfEmax << endl;
            }
            else
            { //                //randomly assign this node to the remaining groups
//                int groupID = 1 + rand() % (numGroups-1);
                //randonly assign this node to a group
                int groupID = rand() % numGroups;
                groups[groupID].push_back(i);
            }

        }
    }
    for(int i = 0; i < groups.size(); i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }
    return groups;

}


//Only generate 2 groups.
//The givenFunction has to be easily computed.
//Put e_max in G0, put all v with f(v|e_max) > percent * f(e_max) in G0. Put the rest of the nodes in G1.
vector<vector<int>> AssignGroups::assignTwoGroupsAccordingToMarginalGainWithRespectToEmaxForCoverFunction(double percent)
{
    vector<vector<int>> groups(2);

    //find e_max with largest f(v)
    int e_max;
    int largestFunctionValue = -1;
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        double currentFunctionValue = f.functionValue(i);
        if(currentFunctionValue > largestFunctionValue)
        {
            e_max = i;
            largestFunctionValue = currentFunctionValue;
        }
    }

    groups[0].push_back(e_max);

    //compute the marginal gain of each node v with respect to e_max, then assign them to groups accordingly
    vector<int> temp{e_max};
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != e_max)
        {
            if(f.marginalGain(i, temp) > percent * largestFunctionValue )
            {
                groups[0].push_back(i);
//                cout << "f(" << i << "|" << emax[0] <<") = " << timCoverage.marginalGainOfNodeWithRespectToEmax(i) << "> " << percent * functionValueOfEmax << endl;
            }
            else
                groups[1].push_back(i);
        }
    }
    for(int i = 0; i < groups.size(); i++)
    {
        cout << "group[" << i << "].size() = " << groups[i].size() << endl;
    }
    return groups;
}



//Only generate 2 groups.
//The givenFunction has to be Influence Function.
//Let e_max, x2, x3, x4, ... be the topK nodes selected submodularly
//Let e_max, s2, s3, s4,... be the topK nodes selected modularly
//Put e_max in G0, put s2 in G1, put x2, x3, ..., x_num to G0 if they are not equal to s2. put the rest in G1.
vector<vector<int>> AssignGroups::assignGroupsMixedModularAndSubModularForInfluenceFunction(int num)
{
    //find top num submodular seed nodes
    vector<vector<int>> groups(2);
    vector<vector<int>> lookupTableI;
    TIMCoverage timCoverage(&lookupTableI);
    timCoverage.initializeLookupTable(f.getRandomRRSetsForInfluenceMaximization(),f.getGroundSetSize() );
    timCoverage.initializeDataStructures(f.getNumOfRRSets(), f.getGroundSetSize());
    vector<int> submodularSeedSet= timCoverage.findTopKNodesForInfluenceMaximization(num,f.getRandomRRSetsForInfluenceMaximization());

    //find v with second largest f(v)
    vector<vector<int>> lookupTableI2;
    TIMCoverage timCoverage2(&lookupTableI2);
    timCoverage2.initializeLookupTable(f.getRandomRRSetsForInfluenceMaximization(),f.getGroundSetSize() );
    timCoverage2.initializeDataStructures(f.getNumOfRRSets(), f.getGroundSetSize());
    vector<int> modularSeedSet = timCoverage2.findTopKNodesModular(2);

    //add top num submodular seed nodes to G0 if they are not s2
    for(int i = 0; i < num; i++)
    {
        if(submodularSeedSet[i] != modularSeedSet[1]) //modularSeedSet[1] is s2
            groups[0].push_back(submodularSeedSet[i]);
    }

    //add s2 to G1
    groups[1].push_back(modularSeedSet[1]);

    //add the remaining nodes randomly into G0 and G1
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != modularSeedSet[1] && find(submodularSeedSet.begin(),submodularSeedSet.end(), i) == submodularSeedSet.end())
        {
            int groupNumber  = rand() % 2;
            groups[groupNumber].push_back(i);
        }
    }
    return groups;
}



//Only generate 2 groups.
//The givenFunction can be any function.
//Let e_max, x2, x3, x4, ... be the topK nodes selected submodularly
//Let e_max, s2, s3, s4,... be the topK nodes selected modularly
//Put e_max in G0, put s2 in G1, put x2, x3, ..., x_num to G0 if they are not equal to s2. put the rest in G1.
vector<vector<int>> AssignGroups::assignGroupsMixedModularAndSubModularForSimpleFunctions(int num)
{
    //find top num submodular seed nodes
    vector<vector<int>> groups(2);
    vector<int> submodularSeedSet;

    for(int i = 0; i < num; i++)
    {
        double largestMarginalGain = 0;
        int bestNode;
        for(int j = 0; j < groundSetSize; j++)
        {
            double marginalGainOfNodeJ = f.marginalGain(j, submodularSeedSet);
            if(find(submodularSeedSet.begin(), submodularSeedSet.end(), j) == submodularSeedSet.end()   && marginalGainOfNodeJ > largestMarginalGain)
            {
                largestMarginalGain = marginalGainOfNodeJ;
                bestNode = j;
            }
        }
        submodularSeedSet.push_back(bestNode);
    }

    //find v with second largest f(v)
    int s2;
    double functionValueOfS2 = 0;
    for(int i = 0; i < groundSetSize; i++)
    {
        if(i != submodularSeedSet[0])
        {
            double functionValueOfI = f.functionValue(i);
            if(functionValueOfI > functionValueOfS2)
            {
                functionValueOfS2 = functionValueOfI;
                s2 = i;
            }
        }
    }

    //add top num submodular seed nodes to G0 if they are not s2
    for(int i = 0; i < num; i++)
    {
        if(submodularSeedSet[i] != s2)
            groups[0].push_back(submodularSeedSet[i]);
    }

    //add s2 to G1
    groups[1].push_back(s2);

    //add the remaining nodes randomly into G0 and G1
    for(int i = 0; i < f.getGroundSetSize(); i++)
    {
        if(i != s2 && find(submodularSeedSet.begin(),submodularSeedSet.end(), i) == submodularSeedSet.end())
        {
            int groupNumber  = rand() % 2;
            groups[groupNumber].push_back(i);
        }
    }
    return groups;
}


