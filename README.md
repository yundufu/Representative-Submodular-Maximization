# **Representative Submodular Maximization**

In this project, we study the representative submodular maximization problem. Given a ground set V with k disjoint groups, and a monotone submodular function f that mapps from the power set of V to real numbers, find a set S with maximized f(S) subject to S intersects with each of the k group by 1 element. The details can be found in the file.......................................................

We developed a k-greedy algorithm that achieves k/(2k-1) approximation ratio with runtime of O(kn) measured in terms of the number of calls to the objective function f. We implemented this algorithm, validated the approximation guarantee by comparing it with the brute force algorithm, and compared it with other strategies, natural greedy algorithm [1], k!-greedy algorithm, and local search algorithm [2].

The algorithms work assuming the user will provide the objective function and group information.   
- For the objective function, we used two functions: 
  -	INFLUENCE_FUNCTION: the function value of a set S is the expected number of people influenced by S under Independent Cascade model.
  -	COVER_FUNCTION:     the function value of a set S is the number of outgoing neighbors of S.  

- For the group information, we tried 2 group Assignment Strategies.  
  -	RANDOM_STRATEGY:    randomly assign the elements to groups
  -	SCREEN_INFLUENCERS: put elements with high function values in a group, and randomly assign the rest of the elements to groups.



## Graphs
The graphs/networks used in the experiments are in the <graphs> directory.



## How to run the program  
- To regenerate experiment results in the thesis............................., run the following programs (in C++) one at a time.  
1). CompareKGreedyWithBruteForce.cpp  
2). CompareKGreedyWithNatualGreedy.cpp  
3). CompareKGreedyWithKFactorialGreedy.cpp  
4). CompareKGreedyWithLocalSearch.cpp  

- In order to run the above programs properly, you need to open the files and set up the parameter values of   
1) graphName  
2) graphPath: absolute path where your test graphs are stored  
3) numOfGroups  
4) outputPath for logger information  
5) functionName  
6) edgeProbability if the functionName you chose is INFLUENCE_FUNCTION  
7) groupStrategy  
8) percent if the groupStrategy you chose is SCREEN_INFLUENCERS  


- Recommend parameters:  

1) For CompareKGreedyWithBruteForce.cpp  

functionName  = COVER_FUNCTION  

|graphName         |       percent  | groupStrategy        |   numOfGroups|
|---               |       ---      | ---                  |   ---        |
|CA-GrQc.txt       |         0.2    | SCREEN_INFLUENCERS   |      2       |
|CA-HepPH.txt      |         0.2    | SCREEN_INFLUENCERS   |      2       |
|NetHept.txt       |         0.2    | SCREEN_INFLUENCERS   |      2       |

functionName  = COVER_FUNCTION  

|graphName               |   percent   | groupStrategy     |      numOfGroups|
|---               |       ---      | ---                  |   ---        |
|CA-GrQc.txt            |     N/A     |   RANDOM_STRATEGY       |  2|
|CA-HepPH.txt         |       N/A    |    RANDOM_STRATEGY      |   2|
|NetHept.txt           |      N/A     |   RANDOM_STRATEGY     |    2|  
 
2) For CompareKGreedyWithNatualGreedy.cpp  

functionName  = COVER_FUNCTION  

|graphName   |               percent  |  groupStrategy      |     numOfGroups|
|---               |       ---      | ---                  |   ---        |
|CA-GrQc.txt           |      0.2    |   SCREEN_INFLUENCERS  |    2, 3, 5, 10|
|CA-HepPH.txt          |      0.2    |   SCREEN_INFLUENCERS  |    2, 3, 5, 10|
|NetHept.txt            |     0.2    |   SCREEN_INFLUENCERS   |   2, 3, 5, 10|
|twitter_combined.txt  |      0.2     |  SCREEN_INFLUENCERS   |   2, 3, 5, 10|
|com-dblp.ungraph.txt     |   0.3    |   SCREEN_INFLUENCERS   |   2, 3, 5, 10|

functionName = INFLUENCE_FUNCTION  

|graphName            | edgeProbability     | percent  |  groupStrategy      |  numOfGroups|
|---                  |       ---           | ---       |   ---              |---    |
|CA-GrQc.txt            |  0.1              |  0.05     | SCREEN_INFLUENCERS |  2, 3, 5, 10|
|twitter_combined.txt   |  0.02            |   0.01    |  SCREEN_INFLUENCERS |  2, 3, 5, 10|
|NetHept.txt             | 0.1              |  0.05    |  SCREEN_INFLUENCERS  | 2, 3, 5, 10|
|com-amazon.txt          | 0.5             |   0.1     |  SCREEN_INFLUENCERS |  2, 3, 5, 10|


3) For CompareKGreedyWithKFactorialGreedy.cpp  

functionName  = COVER_FUNCTION  

|graphName    |              percent   | groupStrategy     |      numOfGroups|
|---               |       ---      | ---                  |   ---        |
|CA-GrQc.txt          |       0.2 |     SCREEN_INFLUENCERS   |   3, 4, 5|
|CA-HepPH.txt         |       0.2  |     SCREEN_INFLUENCERS    |  3, 4, 5|
|NetHept.txt           |      0.2   |    SCREEN_INFLUENCERS     | 3, 4, 5|
|twitter_combined.txt   |     0.2    |   SCREEN_INFLUENCERS      |3, 4, 5|
|com-dblp.ungraph.txt    |    0.3     |  SCREEN_INFLUENCERS      |3, 4, 5|


4) CompareKGreedyWithLocalSearch.cpp  

functionName  = COVER_FUNCTION  

|graphName        |          percent   | groupStrategy      |     numOfGroups|
|---               |       ---      | ---                  |   ---        |
|ManualGraph1     |          0.5    |    SCREEN_INFLUENCERS  |    2, 3|
|ManualGraph2       |        0.5     |   SCREEN_INFLUENCERS   |   2, 3|
|ManualGraph3       |        0.5     |   SCREEN_INFLUENCERS    |  2, 3|


## References  
[1] Halabi, M. E., Mitrovic, S., Norouzi-Fard, A., Tardos, J., and Tarnawski, J. Fairness in streaming submodular maximization: Algorithms and hardness. In NeurIPS, 2020. URL https://proceedings.neurips.cc/paper/2020/hash/9d752cb08ef466fc480fba981cfa44a1-Abstract.html.  

[2] Filmus, Y. and Ward, J. Monotone submodular maximization over a matroid via non-oblivious local search. SIAM Journal on Computing, 43(2):514–542, 2014. doi:10.1137/130920277. URL https://doi.org/10.1137/130920277.  



## Contact  
For any queries, please email Xiaoyun at xfu@iastate.edu
