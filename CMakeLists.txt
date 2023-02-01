cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp AssignGroups.cpp AssignGroups.h BruteForce.cpp BruteForce.h CompareFainessFirstWithGreedyFirst.cpp CompareKGreedyWithBruteForce.cpp CompareKGreedyWithNatualGreedy.cpp CompareKGreedyWithXminAsGuidanceForGreedy.cpp FairnessFirst.cpp FairnessFirst.h Function.cpp Function.h Graph.cpp Graph.h GreedyFirst.cpp GreedyFirst.h InfluenceFunction.cpp InfluenceFunction.h kGreedy.cpp kGreedy.h MarginalGainFunction.cpp MarginalGainFunction.h NaturalGreedy.cpp NaturalGreedy.h SimpleFunction.cpp SimpleFunction.h XminAsGuidanceForkGreedy.cpp XminAsGuidanceForkGreedy.h TIMCoverage.cpp TIMCoverage.h CoverFunction.cpp CoverFunction.h ReachabilityFunction.cpp ReachabilityFunction.h kFactorialGreedy.cpp kFactorialGreedy.h CompareKGreedyWithKFactorialGreedy.cpp LocalSearch.cpp LocalSearch.h AuxiliaryFunctionG.cpp AuxiliaryFunctionG.h CompareKGreedyWithLocalSearch.cpp GenerateManualGraph.cpp)
