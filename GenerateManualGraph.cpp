////
//// Created by Xiaoyun Fu on 10/6/2022.
////
///*
// * For every pair of vertices (u, v), place and edge between u and v with probability p.
//Expected number of edges would around n(n-1)p. You can decide how dense the graph can be by tweaking p.
//      We try graph sizes about n = 100
// */
//#include <iostream>
//#include <time.h>
//#include <vector>
//#include <fstream>
//
//using namespace std;
//
//
//int main()
//{
//    srand(time(0));
//    int groundsetSize = 100;
//    vector<double> P{0.2,0.4,0.6};
//
//
//    //generate edges by checking nodes pair by pair
//    int numEdges = 0;
//    vector<vector<int>> graph;
//    for(int i = 0; i < groundsetSize; i++)
//    {
//        vector<int> empty;
//        graph.push_back(empty);
//    }
//    for(int i = 0; i < groundsetSize; i++)
//    {
//        for(int j = 0; j < groundsetSize; j++)
//        {
//            if(i!=j)
//            {
//                double p = P[rand()%P.size()]; //randomly pick a probability value p from P
//                double randomNum = 1.0 * rand()/RAND_MAX;
//                if(randomNum <= p) //with probability p add edge i->j
//                {
//                    graph[i].push_back(j);
//                    numEdges++;
//                }
//            }
//        }
//    }
//    //output the graph
//    ofstream myfile;
//    string filename = "ManualGraph.txt";
//    string path = "/home/me/Desktop/NIPS/graphs/"; //"E:\\DataFiles\\Clion\\FairSubmodularOptimization\\graphs\\" + fileName);
//    myfile.open (path + filename);
//    myfile << groundsetSize << " " << numEdges<<endl;
//    for(int i = 0; i < groundsetSize; i++)
//    {
//        for(int j = 0; j < graph[i].size(); j++)
//        {
//            myfile << i << " " << graph[i][j] << endl;
//        }
//    }
//
//    myfile.close();
//
//    return 0;
//}
