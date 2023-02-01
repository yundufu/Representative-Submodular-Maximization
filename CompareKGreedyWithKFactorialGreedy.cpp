/*
 *  In this class, we compare the results of our k-greedy algorithm with the k!-greedy algorithm
 *  for the equal submodular optimization problem.
 */

#include "Function.h"
#include "kGreedy.h"
#include "kFactorialGreedy.h"
#include "InfluenceFunction.h"
#include "SimpleFunction.h"
#include "CoverFunction.h"
#include "ReachabilityFunction.h"
#include "AssignGroups.h"

#include <ctime>    // For time()
#include <chrono>
#include <iostream>
#include <fstream>
#include <cassert>

void printSeedSet(vector<int> seedSet, ofstream& log);

int main() {

//constants
    const int NUM_RUNS = 5;
    const string INFLUENCE_FUNCTION = "Influence Function";
    const string COVER_FUNCTION = "Cover Function";
    const string REACHABILITY_FUNCTION = "Reachability Function";
    const string RANDOM_STARTEGY = "Random";
    const string SCREEN_INFLUENCERS = "Screen Influencers";


    //parameters
    string graphName = "CA-GrQcSim.txt";
    string graphPath =  "/home/me/RepresentativeSubmodularMaximization/graphs/";
    double edgeProbability = 0.02;  //used for influence function
    double percent = 0.01;
    int numOfGroups = 2;
    string outputPath = "/home/me/Desktop/logger.txt";

    string groupStrategy = RANDOM_STARTEGY;
//    string groupStrategy = SCREEN_INFLUENCERS;

    string functionName = COVER_FUNCTION;  // COVER_FUNCTION; REACHABILITY_FUNCTION
    CoverFunction f(graphPath + graphName);

//    string functionName = INFLUENCE_FUNCTION;
//    InfluenceFunction f(graphPath + graphName, edgeProbability);

//    string functionName = REACHABILITY_FUNCTION;
//    ReachabilityFunction f(graphPath + graphName);


//================================Actual Experiements========================================================
    int groundSetSize = f.getGroundSetSize();  // need to change this to graph size
    AssignGroups assignGroups(groundSetSize, numOfGroups, f);

    for (int j = 0; j < NUM_RUNS; j++) {

        //logging purpose
        ofstream logbook;
        logbook.open(outputPath, fstream::app);
        auto start = std::chrono::system_clock::now();
        std::time_t start_time = std::chrono::system_clock::to_time_t(start);
        logbook << std::ctime(&start_time);
        logbook << "graph name: " << graphName << endl;
        logbook << "n = " << f.getGroundSetSize() << endl;
        if (functionName == INFLUENCE_FUNCTION)
            logbook << "edge probability is set to " << edgeProbability << endl;

        //assign the groups
        vector<vector<int>> groups;
        if (groupStrategy == RANDOM_STARTEGY) {
            groups = assignGroups.assignGroupsRandomly();
            logbook << numOfGroups << " groups are generated by " << "randomly assign each node to a group" << endl;
        } else if (groupStrategy == SCREEN_INFLUENCERS) {
            groups = assignGroups.assignGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(numOfGroups,
                                                                                                           percent);
            logbook << numOfGroups << " are generated by putting e_max in G0, all v with f(v|e_max) > f(e_max) * "
                    << percent << " in G0, and rest of nodes randomly" << endl;
        }

        for (int i = 0; i < groups.size(); i++)
            logbook << "group[" << i << "].size() = " << groups[i].size() << ", ";
        logbook << endl;


        assert (groups.size() == numOfGroups);   //check if there are empty groups generated
        logbook << "Compare k-greedy algorithm with k!-greedy algorithm using " << functionName << endl;

        kGreedy kgreedy(f, groups, numOfGroups);
        double functionValueOfX;
        clock_t startTime1 = clock(); //using <ctime> library
        vector<int> X;
        if (functionName == INFLUENCE_FUNCTION) {
            X = kgreedy.kGreedyAlgorithmForInfluenceFunction(functionValueOfX);
        } else if (functionName == COVER_FUNCTION) {
            X = kgreedy.kGreedyAlgorithmForCoverFunction(functionValueOfX);
        } else if (functionName == REACHABILITY_FUNCTION) {
            X = kgreedy.kGreedyAlgorithmForReachabilityFunction(functionValueOfX);
        }
        clock_t endTime1 = clock();
        double timeFindingSeedSetX = double(endTime1 - startTime1) / CLOCKS_PER_SEC; //unit is seconds
        logbook << "Time to find the best seed set for k-greedy algorithm: " << timeFindingSeedSetX << " seconds."
                << endl;
        cout << "seed set for k-greedy algorithm found" << endl;
        logbook << "f(k-greedy algorithm seed set X) = " << functionValueOfX << "with X = ";
        printSeedSet(X, logbook);
        logbook << "--------------------------------------------------------------------------------------" << endl;


        kFactorialGreedy kfactorialGreedy(f, groups, numOfGroups);
        double functionValueOfT;
        clock_t startTime3 = clock(); //using <ctime> library
        vector<int> T;
        if (functionName == INFLUENCE_FUNCTION) {
            T = kfactorialGreedy.kFactorialGreedyAlgorithmForInfluenceFunction(functionValueOfT, logbook);
        } else if (functionName == COVER_FUNCTION) {
            T = kfactorialGreedy.kFactorialGreedyAlgorithmForCoverFunction(functionValueOfT);
        } else if (functionName == REACHABILITY_FUNCTION) {
            T = kfactorialGreedy.kFactorialGreedyAlgorithmForReachabilityFunction(functionValueOfT);
        }

        clock_t endTime3 = clock();
        double timeFindingSeedSetT = double(endTime3 - startTime3) / CLOCKS_PER_SEC; //unit is seconds
        logbook << "Time to find the best seed set for k!-greedy algorithm: " << timeFindingSeedSetT << " seconds."
                << endl;
        cout << "seed set for k!-greedy algorithm found" << endl;
        logbook << "f(k!-greedy algorithm seed set T) = " << functionValueOfT << "with T = ";
        printSeedSet(T, logbook);
        logbook << "--------------------------------------------------------------------------------------" << endl;
        logbook << "======================================================================================" << endl
                << endl;


    }

}


void printSeedSet(vector<int> seedSet, ofstream& log)
{
    log << "Seed set size: " << seedSet.size() << endl;
    log << "Seed Nodes: ";
    for(int ele : seedSet){
        log << ele << ", ";
    }
    log << endl;
}
//=======================================================================================
//=======================================================================================




//
////=========================================================================================
////==================Use this version if functionValue is easy to compute===================
////    assert (groups.size() == k);
//
//#include "Function.h"
//#include "kGreedy.h"
//#include "NaturalGreedy.h"
//#include "CoverFunction.h"
//#include "AssignGroups.h"
//
//#include <ctime>    // For time()
//#include <iostream>
//#include <fstream>
//#include <cassert>
//void printSeedSet(vector<int> seedSet, ofstream& log);
//
//int main()
//{
//
////Define the function f
////    SimpleFunction f(groundSetSize);
//    string graphName = "CA-HepThSim.txt";   // try 3 graphs with size n = 10k, 100k, 1M.
//    CoverFunction f(graphName);
//
//    //logging purpose
//    ofstream logbook;
//    logbook.open("E:\\DataFiles\\Clion\\FairSubmodularOptimization\\log.txt", fstream::app);
////    logbook.open("/home/me/Desktop/FairSubmodularOptimization/log.txt", fstream::app);
//    logbook << "graph name: " << graphName << " for CoverFunction" << endl;
//    logbook << "n = " << f.getGroundSetSize() <<  endl;
//
//    int groundSetSize = f.getGroundSetSize();  // need to change this to graph size
//    int numOfGroups = 3;  //try 2,3, 5, 10, 20, 50
//    cout << "ground set size  = " << groundSetSize << "double verify this!!!!!!!!!!!!!!" << endl;
//
//
////Assign the groups
//    AssignGroups assignGroups(groundSetSize, numOfGroups, f);
////    vector<vector<int>> groups = assignGroups.assignGroupsAlternatively();
//    vector<vector<int>> groups = assignGroups.assignGroupsFromRandomSourceAndReachability(f.getGraph());
//
////        vector<vector<int>> groups = assignGroups.assignGroupsByFunctionValue();
//    assert (groups.size() == numOfGroups);   //check if there are empty groups generated
//    cout<<"size of groups(should be " << numOfGroups  <<"): " << groups.size()<<endl;
//
//    logbook<< "Compare k-greedy algorithm with Natural greedy algorithm using InfluenceFunction:" << endl;
////    logbook<< numOfGroups << " groups are generated by " << "assigning nodes to groups in descending order of f(v) round by round" << endl;
////    logbook<< numOfGroups << " groups are generated by " << "putting top " << topK <<" influence nodes in group 0, and randomly assign each remaining node to a group" << endl;
////    logbook<< numOfGroups << " are generated by " << "assigning elements in descending f(x) value into equal size groups" << endl;
////    logbook<< numOfGroups << " groups are generated by " << "putting e = argmax f(v) and x = argmax f(v|e) in group 0, putting y with 2nd largest f(v|e) and " <<
////           "z with 3rd larfest f(v|e) in group 1, and randomly assigning the remaining" << endl;
//logbook  << numOfGroups << " are generated from a random source v and reachable nodes from it up to roughly n/k" << endl;
//    kGreedy kgreedy(f, groups, numOfGroups);
//    double functionValueOfX;
//    clock_t startTime1 = clock(); //using <ctime> library
//    vector<int> X = kgreedy.kGreedyAlgorithmForInfluenceFunction(functionValueOfX);
//    clock_t endTime1 = clock();
//    double timeFindingSeedSetX = double(endTime1 - startTime1) / CLOCKS_PER_SEC; //unit is seconds
//    logbook << "Time to find the best seed set for k-greedy algorithm: " << timeFindingSeedSetX << " seconds." << endl;
//    cout << "seed set for k-greedy algorithm found" << endl;
//    logbook << "f(k-greedy algorithm seed set X) = " << functionValueOfX << "with X = " ;
//    printSeedSet(X, logbook);
//    logbook << "--------------------------------------------------------------------------------------"<< endl;
//
//
//
//    NaturalGreedy naturalGreedy(f,groups,numOfGroups);
//    double functionValueOfY;
//    clock_t startTime2 = clock(); //using <ctime> library
//    vector<int> Y = naturalGreedy.naturalGreedyAlgorithmForInfluenceFunction(functionValueOfY);
//    clock_t endTime2 = clock();
//    double timeFindingSeedSetY = double(endTime2 - startTime2) / CLOCKS_PER_SEC; //unit is seconds
//    logbook << "Time to find the best seed set for natural greedy algorithm: " << timeFindingSeedSetY << " seconds." << endl;
//    cout << "seed set for natural greedy algorithm found" << endl;
//    logbook << "f(natural greedy algorithm seed set Y) = " << functionValueOfY << "with Y = " ;
//    printSeedSet(Y, logbook);
//    logbook << "--------------------------------------------------------------------------------------"<< endl;
//    logbook << "======================================================================================" << endl << endl;
//}
//
//
//void printSeedSet(vector<int> seedSet, ofstream& log)
//{
//    log << "Seed set size: " << seedSet.size() << endl;
//    log << "Seed Nodes: ";
//    for(int ele : seedSet){
//        log << ele << ", ";
//    }
//    log << endl;
//}
//
////============================================================================================
////============================================================================================