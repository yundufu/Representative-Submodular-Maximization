//
// Created by Xiaoyun Fu on 5/5/2022.
//

#include "Function.h"
#include <iostream>

Function::Function(int size){
    groundSetSize = size;
}


Function::Function(){
    groundSetSize = 0;
}

int Function::getGroundSetSize()
{
    return groundSetSize;
}

int Function::getNumOfRRSets()
{
    return 0;
}

vector<vector<int>>* Function::getRandomRRSetsForInfluenceMaximization() {
    return nullptr;
}

//used for ReachabilityFunction
vector<vector<int>> Function::getReachableSets()
{
    vector<vector<int>> empty;
    return empty;
}

//used for ReachabilityFunction
vector<vector<int>> Function::getReverseReachableSets()
{
    vector<vector<int>> empty;
    return empty;
}

//double Function::functionValue(vector<int> aSet)
//{
//    return 0; //We assumed f(emptySet) = 0
//}
//// the subclasses have to implement their own function body

//double Function::marginalGain(int v, vector<int> aSet)  //returns a value of f(aSet ∪ {v}) - f(aSet)
//{
//    return 0;
//}
//
//double Function::marginalGain(vector<int> A, vector<int> aSet)
//{
//    return 0;
//}

Function::~Function()
{
  cout << "calling destructor" << endl;
}
