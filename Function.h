//
// Created by me on 5/11/22.
//

#ifndef UNTITLED_FUNCTION_H
#define UNTITLED_FUNCTION_H


/*
 * This class is an abstract class for a monotone submodular function f: 2^N -> R. We assumed the ground set V ⊆ N of
 * natural numbers.
 * The specific submodular functions we use, like influence function, need to inherit this class
 */

#include <vector>

using namespace std;

class Function {
private:
    int groundSetSize;  //V = {0, 1, 2,..., n -1}
public:
    Function(int size);
    Function();
    virtual double functionValue(int node) = 0;
    virtual double functionValue(vector<int> aSet) = 0; //implement f(emptySet)  = 0
    // the subclasses have to implement their own function body
    virtual double marginalGain(int v, vector<int> aSet) = 0;  //returns a value of f(aSet ∪ {v}) - f(aSet)
    virtual double marginalGain(vector<int> A, vector<int> aSet) = 0;
    //returns a value of f(aSet ∪ A) - f(aSet)
    virtual int getGroundSetSize();
    virtual vector<vector<int>> getReachableSets();
    virtual vector<vector<int>> getReverseReachableSets();
    virtual vector<vector<int>>* getRandomRRSetsForInfluenceMaximization();
    virtual int getNumOfRRSets();
    ~Function();
};




#endif //UNTITLED_FUNCTION_H
