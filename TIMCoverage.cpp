

#include "TIMCoverage.h"

// why is lookupTable given as a parameter?
// who will compute it besides me calling initializeLookupTable()?
//Constructor
TIMCoverage::TIMCoverage(vector<vector<int>> *lookupTable) {
    this->lookupTable = lookupTable;
    this->numberOfRRSetsCovered = 0;
}

//TIMCoverage::TIMCoverage()
//{
//    this->numberOfRRSetsCovered = 0;
//}


//TIMCoverage::~TIMCoverage()
//{
//   delete lookupTable;
//}

TIMCoverage::~TIMCoverage() {
    this->lookupTable = nullptr;
}

TIMCoverage::TIMCoverage(vector<vector<int>> *lookupTable, vector<vector<int>>& groups, int givenSetSize) : groundSetSize(givenSetSize), groupMembership(givenSetSize, -1)
{
    this->lookupTable = lookupTable;
    this->numberOfRRSetsCovered = 0;
    myGroups = groups;
    for(int i = 0; i < myGroups.size(); i++)
    {
        for(int node : myGroups[i])
        {
            groupMembership[node] = i;
        }
    }
}

pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModelForKGreedy(vector<vector<int>> *rrSets, int group, vector<int> order)
{
    return findMaxInfluentialNodeAndUpdateModelForKGreedy(rrSets, nullptr, group, order);
}


pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModelForKGreedy(vector<vector<int>> *rrSets, NodeChecker *nodeChecker, int groupID, vector<int> order)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>* queue = &this->queue;
    vector<int> passedNodes;
    int index = find(order.begin(), order.end(), groupID) - order.begin();
    set<int> laterGroupIDs;
    for(int i = index + 1; i < order.size();i++)
    {
        laterGroupIDs.insert(order[i]);
    }

    vector<int>* coverage = &this->coverage;
    vector<bool>* nodeMark = &this->nodeMark;
    int maximumGainNode = -1;
    int influence = -1;
//    cout << "myGroups.size() =" << myGroups.size() << endl;
    vector<int> groupmembers = myGroups[groupID];
//    cout << "queue.size()" << queue->size() << endl;
    while(!queue->empty()) {//Among the unselected nodes(eligible nodes), find the node with max marginal gain(coverage number)
        pair<int,int> element = queue->top();  //<node i, number of RRSets containing i>
//        cout << "element = " << element.first<< " is in group " << groupMembership[element.first] << ", current group is " << groupID << endl;
        if(element.second > (*coverage)[element.first]) { //update queue
//            cout << "enters if loop because of inconsistency" << endl;
            queue->pop();
            element.second = (*coverage)[element.first];
            queue->push(element);
            continue;
        }
        queue->pop();
        if (nodeChecker!=NULL && !nodeChecker->isNodeValid(element.first)) {
            continue;
        }
        if(!(*nodeMark)[element.first]) {  //if the node is marked false, it is not eligible to consider as a seed,
            continue;                      // so just pass it.
        }

        if(find(groupmembers.begin(),groupmembers.end(), element.first) == groupmembers.end())
        {       //current node is not in this Group, need to skip this node for now , but need to add it back later!!!!!
            assert(laterGroupIDs.count(groupMembership[element.first]) == 1);
            passedNodes.push_back(element.first);
            continue;
        }

        maximumGainNode = element.first;
        influence = (*coverage)[element.first]; //the number of elements containing this node
//        cout << "maximumGainNode = " << maximumGainNode << " with marginal influence = " << influence << "  is in group " << groupID << endl;
        break;  //skip the while loop, because the node with max marginal gain is found
        // ( queue already sorted that, so the top element in queue is the one!)
    }

    int R = this->R;
    double scaledInfluence = (double) influence * nodeMark->size()/R;

    //update the rest of the data structure to find the next element with max marginal gain
    vector<bool>* edgeMark = &this->edgeMark;
//    cout << "in kGreedy, node " << maximumGainNode << " is in group " << groupID << endl;
    for(int node : myGroups[groupID])
        (*nodeMark)[node] = false;  //remove these nodes from future consideration because this group is covered

    int numberCovered = this->countForVertex(maximumGainNode);
    vector<int> edgeInfluence = (*this->lookupTable)[maximumGainNode]; //the IDs of RRsets containing this node

    for (int i = 0; i < numberCovered; i++) { //for each RRset RR containing this node:
        if ((*edgeMark)[edgeInfluence[i]]) continue; //if RR is already processed previously

        vector<int> nList = (*rrSets)[edgeInfluence[i]]; //if this is the first time RR is covered,
        for (int l : nList) {                           //then need to remove this RR from future consideration by removing
            if ((*nodeMark)[l]) {                       //it from i's coverage where i is in RR
                (*coverage)[l]--;
            }
        }
        (*edgeMark)[edgeInfluence[i]] = true;
        this->numberOfRRSetsCovered++;
    }

    for(int node: passedNodes) //pushback the passed nodes to queue
    {
        pair<int,int> aPair(node, (*coverage)[node]);
        queue->push(aPair);
    }

    return make_pair(maximumGainNode, scaledInfluence);
}


//calculate influence
vector<int> TIMCoverage::findTopKNodesForKGreedyOneOrder(int k, vector<vector<int>> *rrSets, vector<int>& order, double& influence)
{
    vector<int> topKNodes;
    double result =0;
    for(int i = 0; i < k; i++) {
//        cout <<"i = " << i <<", order["<< i <<"] = " << order[i] << endl;
        pair<int, double> current = findMaxInfluentialNodeAndUpdateModelForKGreedy(rrSets, order[i], order);
        topKNodes.push_back(current.first);
        result = result + current.second;
    }
    influence = result;
    return topKNodes;
}


pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(vector<vector<int>> *rrSets)
{
    return findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(rrSets, nullptr);
}


//============================================================================
//Need to change the body
pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(vector<vector<int>> *rrSets, NodeChecker *nodeChecker)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>* queue = &this->queue;

    vector<int>* coverage = &this->coverage;
    vector<bool>* nodeMark = &this->nodeMark;
    int maximumGainNode = -1;
    int influence = 0;
//    cout << "myGroups.size() =" << myGroups.size() << endl;
    while(!queue->empty()) {//Among the unselected nodes(eligible nodes), find the node with max marginal gain(coverage number)
        pair<int,int> element = queue->top();  //<node i, number of RRSets containing i>
        if(element.second > (*coverage)[element.first]) { //what is this block used for?
            queue->pop();
            element.second = (*coverage)[element.first];
            queue->push(element);
            continue;
        }
        queue->pop();
        if (nodeChecker!=NULL && !nodeChecker->isNodeValid(element.first)) {
            continue;
        }
        if(!(*nodeMark)[element.first]) {  //if the node is marked false, it is not eligible to consider as a seed,
            continue;                      // so just pass it.
        }

        maximumGainNode = element.first;
        influence = (*coverage)[element.first]; //the number of elements containing this node
        break;  //skip the while loop, because the node with max marginal gain is found
        // ( queue already sorted that, so the top element in queue is the one!)
    }

    int R = this->R;
    double scaledInfluence = (double) influence * nodeMark->size()/R;

    //update the rest of the data structure to find the next element with max marginal gain
    vector<bool>* edgeMark = &this->edgeMark;
    //update for all nodes in the same group as maximumGainNode
    int groupID = groupMembership[maximumGainNode];
//    cout << "in NaturalGreedy, node " << maximumGainNode << " is in group " << groupID << endl;
    for(int node: myGroups[groupID])
        (*nodeMark)[node] = false; //remove these nodes from future consideration because this group is covered

    int numberCovered = this->countForVertex(maximumGainNode);
    vector<int> edgeInfluence = (*this->lookupTable)[maximumGainNode]; //the IDs of RRsets containing this node

    for (int i = 0; i < numberCovered; i++) { //for each RRset RR containing this node:
        if ((*edgeMark)[edgeInfluence[i]]) continue; //if RR is already processed previously

        vector<int> nList = (*rrSets)[edgeInfluence[i]]; //if this is the first time RR is covered,
        for (int l : nList) {                           //then need to remove this RR from future consideration by removing
            if ((*nodeMark)[l]) {                       //it from i's coverage where i is in RR
                (*coverage)[l]--;
            }
        }
        (*edgeMark)[edgeInfluence[i]] = true;
        this->numberOfRRSetsCovered++;
    }

    return make_pair(maximumGainNode, scaledInfluence);

}
vector<int> TIMCoverage::findTopKNodesForNaturalGreedy(int k, vector<vector<int>> *rrSets, double& influence)
{
    vector<int> topKNodes;
    double result =0;
    for(int i = 0; i < k; i++) {
        pair<int, double> current = findMaxInfluentialNodeAndUpdateModelForNaturalGreedy(rrSets);
        topKNodes.push_back(current.first);
        result = result + current.second;
    }
    influence = result;
    return topKNodes;
}




// n is the number of nodes in the graph
// given the computed collection of RRSets, for each node i, find the set of RRSets containing i
// lookupTable contains <node i, vector<IDs of RRSets containing i> >
void TIMCoverage::initializeLookupTable(vector<vector<int>>* randomRRSets, int n) {
    groundSetSize= n;
    initialize1(n);
    //the rrSetID is defined as the order each set appears in the collection
    initialize2(randomRRSets);
}

//used in initializeLookupTable()
void TIMCoverage::initialize1(int n)
{
    for(int i = 0; i < n; i++) {
        (*lookupTable).push_back(vector<int>());
    }
}
//used in initializeLookupTable()
void TIMCoverage::initialize2(vector<vector<int>>* randomRRSets)
{
    for(int rrSetID = 0; rrSetID < randomRRSets->size(); rrSetID++) {
        initialize3(randomRRSets, rrSetID);
    }
}

void TIMCoverage::initialize3(vector<vector<int>>* randomRRSets, int rrSetID)
{
    for(int vertex: (*randomRRSets)[rrSetID]) {
        incrementCountForVertex(vertex, rrSetID);
    }
}

// this randomRRSet contains node u
void TIMCoverage::incrementCountForVertex(int u, int randomRRSetID) {
    (*lookupTable)[u].push_back(randomRRSetID);
}

//has to be called after calling initializeLookupTable()
vector<vector<int>>* TIMCoverage::getLookupTable()
{
    return lookupTable;
}

//void TIMCoverage::clearLookupTable(int n)
//{
////  for(int i = 0; i < n; i++)
////  {
////      (*lookupTable)[i].clear();
////  }

//  delete lookupTable;
//}



void TIMCoverage::initializeDataStructures(int R, int n) {
    this->R = R;

    int numberCovered;
    for (int i = 0; i < n; i++) {
        nodeMark.push_back(false);
        coverage.push_back(0);  //number of (new?) RRSets covered by node i?
    }
    for (int i = 0; i < R; i++) { //R is the number of RRSets.
        edgeMark.push_back(false);
    }
    for (int i = 0; i < n; i++) {
        numberCovered = this->countForVertex(i); //number of RRSets containing node i
        queue.push(make_pair(i, numberCovered));
        coverage[i] = numberCovered;
        nodeMark[i] = true;
    }
}


// the number of RRSets containing node u
int TIMCoverage::countForVertex(int u) {
    if(u >= this->lookupTable->size())
        cout << " node u = " << u << ",lookupTable->size() = " << lookupTable->size() <<  endl;
    assert(u < this->lookupTable->size());  //u need to be a valid node
    return (int)(*lookupTable)[u].size();
}

//void TIMCoverage::clearDataStructures()
//{
//   R = 0;
//   nodeMark.clear();
//   coverage.clear();
//   edgeMark.clear();
//   while(!queue.empty())
//       queue.pop();
//}






//what does erase do? remove setID from u's list meaning this RRset does not contain node u anymore?
void TIMCoverage::decrementCountForVertex(int u, int setID) {
    (*lookupTable)[u].erase(std::remove((*lookupTable)[u].begin(), (*lookupTable)[u].end(), setID), (*lookupTable)[u].end());
}

//returns the set of IDs of RRSets containing this vertex?
vector<int> TIMCoverage::getRRSetsCoveredByVertex(int vertex) {
    return (*lookupTable)[vertex];
}

//why new?
int TIMCoverage::numberOfNewRRSetsCoveredByVertex(int vertex) {
    return this->coverage[vertex];
}

//what is the use of this? what is the meaning of scaling factor?
double TIMCoverage::marginalGainWithVertex(int vertex, double scalingFactor) {
    return (this->coverage[vertex] * scalingFactor);
}

//what does offset mean? what is it used for?
void TIMCoverage::offsetCoverage(int vertex, int offset) {
    this->coverage[vertex] = this->coverage[vertex] + offset;
}

//while the best node is selected, update queue to select the next best node?
void TIMCoverage::updatePriorityQueueWithCurrentValues() {
//    this->queue = priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>();
    while (!this->queue.empty()) {
        this->queue.pop();
    }

    for (int i = 0; i < this->coverage.size(); i++) {
        this->queue.push(make_pair(i, this->coverage[i]));
    }
}

//why do we need this reverse queue? To find the k min influential nodes?
void TIMCoverage::constructReverseQueue() {
    while(!this->reverseQueue.empty()) {
        this->reverseQueue.pop();
    }
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator> *queueCopy = new priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>(this->queue);
    while (!queueCopy->empty()) {
        this->reverseQueue.push(queueCopy->top());
        queueCopy->pop();
    }
    delete queueCopy;
}


set<int> TIMCoverage::findTopKNodesForBeliefMaximization(int k, vector<vector<int>> *rrSets) {
    set<int> topKNodes;
    for(int i = 0; i < k; i++) {
        topKNodes.insert(findMaxInfluentialNodeAndUpdateModel(rrSets).first);
    }
    return topKNodes;
}


vector<int> TIMCoverage::findTopKNodesForInfluenceMaximization(int k, vector<vector<int>> *rrSets) {
    vector<int> topKNodes;
    for(int i = 0; i < k; i++) {
        topKNodes.push_back(findMaxInfluentialNodeAndUpdateModel(rrSets).first);
    }
    return topKNodes;
}


set<pair<int, int>> TIMCoverage::findTopKNodesWithInfluence(int k, vector<vector<int>> *rrSets) {
    set<pair<int, int>> topKNodesWithInfluence;
    for(int i = 0; i < k; i++) {
        topKNodesWithInfluence.insert(findMaxInfluentialNodeAndUpdateModel(rrSets));
    }                                // but this returns a pair of <int, double>
    return topKNodesWithInfluence;
}


pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModel(vector<vector<int>> *rrSets) {
    return findMaxInfluentialNodeAndUpdateModel(rrSets, nullptr);
}


//what does this pair<int, double> store?  what does update model mean?
pair<int, double> TIMCoverage::findMaxInfluentialNodeAndUpdateModel(vector<vector<int>>* rrSets, NodeChecker* nodeChecker) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>* queue = &this->queue;

    vector<int>* coverage = &this->coverage;
    vector<bool>* nodeMark = &this->nodeMark;
    int maximumGainNode = -1;
    int influence = 0;
    while(!queue->empty()) {//Among the unselected nodes(eligible nodes), find the node with max marginal gain(coverage number)
        pair<int,int> element = queue->top();  //<node i, number of RRSets containing i>
        if(element.second > (*coverage)[element.first]) { //this block used for updating the queue
            queue->pop();
            element.second = (*coverage)[element.first];
            queue->push(element);
            continue;
        }

        queue->pop();
        if (nodeChecker!=NULL && !nodeChecker->isNodeValid(element.first)) {
            continue;
        }
        if(!(*nodeMark)[element.first]) {  //if the node is marked false, it is not eligible to consider as a seed,
            continue;                      // so just pass it.
        }

        maximumGainNode = element.first;
        influence = (*coverage)[element.first]; //the number of elements containing this node
        break;  //skip the while loop, because the node with max marginal gain is found
        // ( queue already sorted that, so the top element in queue is the one!)
    }

    int R = this->R;
    double scaledInfluence =  1.0 * influence * nodeMark->size()/R;

    //update the rest of the data structure to find the next element with max marginal gain
    vector<bool>* edgeMark = &this->edgeMark;
    (*nodeMark)[maximumGainNode] = false;  //remove this node from future consideration because it is already selected as seed in this iteration
    int numberCovered = this->countForVertex(maximumGainNode);
    vector<int> edgeInfluence = (*this->lookupTable)[maximumGainNode]; //the IDs of RRsets containing this node

    for (int i = 0; i < numberCovered; i++) { //for each RRset RR containing this node:
        if ((*edgeMark)[edgeInfluence[i]]) continue; //if RR is already processed previously

        vector<int> nList = (*rrSets)[edgeInfluence[i]]; //if this is the first time RR is covered,
        for (int l : nList) {                           //then need to remove this RR from future consideration by removing
            if ((*nodeMark)[l]) {                       //it from i's coverage where i is in RR
                (*coverage)[l]--;           //remove this RRset from node nodeMark[l]'s list
            }
        }
        (*edgeMark)[edgeInfluence[i]] = true;
        this->numberOfRRSetsCovered++;
    }

    return make_pair(maximumGainNode, scaledInfluence);
}

//this method is only used to find the marginal gain of node v with respect to e_max.
//Should call this method right after findTopKNodesForInfluenceMaximization(1,R); in AssignGroups::assignTwoGroupsAccordingToMarginalGainWithRespectToEmaxForInfluenceFunction(double percent)
double TIMCoverage::marginalGainOfNodeWithRespectToEmax(int v)
{
//    cout << "In TIMCoverage::marginalGainOfNodeWithRespectToEmax(int v), groundSetSize = " << groundSetSize << endl;
    if(v <0 || v >= this->coverage.size())
        cout << "the node " << v << " is out of bound for node range [0, " << this->coverage.size() <<"]."<<endl;
    return 1.0 * this->coverage[v] * this->nodeMark.size()/ R;

}

vector<int> TIMCoverage::findTopKNodesModular(int k) {
    int i = 0;
    vector<int> seedSet;
    while(i < k) {
        seedSet.push_back(this->queue.top().first);
        this->queue.pop();
        i++;
    }
    return seedSet;
}



pair<vector<int>,int> TIMCoverage::findTopKNodesFromCandidatesWithoutUpdate(int k, vector<vector<int>> *rrSets, set<int> candidateNodes) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator> *queue = new priority_queue<pair<int, int>, vector<pair<int, int>>, QueueComparator>(this->queue);

    vector<int> *coverage = new vector<int>(this->coverage);
    vector<bool> *nodeMark = new vector<bool>(this->nodeMark);
    vector<bool> *edgeMark = new vector<bool>(this->edgeMark);
    int maximumGainNode = -1;
    vector<int> topKNodes;
    int rrSetsCovered = 0;
    while(!queue->empty() && topKNodes.size()<=k) {
        pair<int,int> element = queue->top();
        if(element.second > (*coverage)[element.first]) {
            queue->pop();
            element.second = (*coverage)[element.first];
            queue->push(element);
            continue;
        }

        queue->pop();
        if (candidateNodes.find(element.first)==candidateNodes.end()) {
            continue;
        }
        if(!(*nodeMark)[element.first]) {
            continue;
        }
        topKNodes.push_back(element.first);

        int numberCovered = this->countForVertex(element.first);
        vector<int> edgeInfluence = (*this->lookupTable)[element.first];

        for (int i = 0; i < numberCovered; i++) {
            if ((*edgeMark)[edgeInfluence[i]]) continue;

            vector<int> nList = (*rrSets)[edgeInfluence[i]];
            for (int l :
                    nList) {
                if ((*nodeMark)[l]) {
                    (*coverage)[l]--;
                }
            }
            (*edgeMark)[edgeInfluence[i]] = true;
            rrSetsCovered++;
        }
    }


    delete coverage;
    delete nodeMark;
    delete edgeMark;
    delete queue;
    return make_pair(topKNodes, rrSetsCovered);
}

void TIMCoverage::addToSeed(int vertex, vector<vector<int>> *rrSets) {

    vector<int>* coverage = &this->coverage;
    vector<bool>* nodeMark = &this->nodeMark;
    vector<bool>* edgeMark = &this->edgeMark;
    (*nodeMark)[vertex] = false;
    int numberCovered = this->countForVertex(vertex);
    vector<int> edgeInfluence = (*this->lookupTable)[vertex];

    for (int i = 0; i < numberCovered; i++) {
        if ((*edgeMark)[edgeInfluence[i]])
            continue;

        vector<int> nList = (*rrSets)[edgeInfluence[i]];
        for (int l : nList) {
            if ((*nodeMark)[l]) {
                (*coverage)[l]--;
            }
        }
        (*edgeMark)[edgeInfluence[i]] = true;
        this->numberOfRRSetsCovered++;
    }
}

double TIMCoverage::findInfluence(set<int> seedSet, double scalingFactor) {
    // Should not update anything
    // For each vertex in seed set:
    //  Go to each rr set covered. Mark edgeMark[rrSet Covered] as true
    // Go through edgeMark and find number of trues. Scale it as per scaling factor and return influence.
    vector<int> edgeMarkTemporary(this->edgeMark.size(), false);
    int rrSetsCovered = 0;
    for(int seed:seedSet) {
        vector<int> *edgeInfluence = &(*this->lookupTable)[seed];
        for (int rrSetID: *edgeInfluence) {
            if(edgeMarkTemporary[rrSetID]) continue;
            edgeMarkTemporary[rrSetID] = true;
            rrSetsCovered++;
        }
    }
//    return round((double)rrSetsCovered * scalingFactor);
    return rrSetsCovered * scalingFactor;
}



int TIMCoverage::getNumberOfRRSetsCovered() {
    return this->numberOfRRSetsCovered;
}

void TIMCoverage::incrementRRSetsCovered(int number) {
    this->numberOfRRSetsCovered = this->numberOfRRSetsCovered + number;
}

vector<bool>* TIMCoverage::getNodeMark() {
    return &nodeMark;
}
vector<bool>* TIMCoverage::getEdgeMark() {
    return &edgeMark;
}

TIMCoverage::TIMCoverage( const TIMCoverage &obj) {

    queue = obj.queue;
    lookupTable = obj.lookupTable;
    R = obj.R;
    numberOfRRSetsCovered = obj.numberOfRRSetsCovered;
    for(bool x: obj.nodeMark) {
        nodeMark.push_back(x);
    }

    for(bool x: obj.edgeMark) {
        edgeMark.push_back(x);
    }
    for(int x: obj.coverage) {
        coverage.push_back(x);
    }
}

TIMCoverage& TIMCoverage::operator=( const TIMCoverage &obj) {
    if (&obj==this) {
        return *this;
    }
    queue = obj.queue;
    lookupTable = obj.lookupTable;
    R = obj.R;
    numberOfRRSetsCovered = obj.numberOfRRSetsCovered;
    for(bool x: obj.nodeMark) {
        nodeMark.push_back(x);
    }

    for(bool x: obj.edgeMark) {
        edgeMark.push_back(x);
    }
    for(int x: obj.coverage) {
        coverage.push_back(x);
    }
    return *this;
}



