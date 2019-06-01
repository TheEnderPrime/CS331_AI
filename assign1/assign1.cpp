#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <stdlib.h>

/*****************************
 * David Baugh
 * Travis Larson
 * 
 * Assignment 1
 * 
 * 4/19/19
 * 
 *****************************/

//MAKE : MinGW32-make
//MAKE RUN : MinGW32-make dfs
//COMPILE : g++ -o assign1 assign1.cpp 
//COMPILE ADD ON MY DESKTOP : -static-libstdc++ -static-libgcc
//RUN     : assign1 startState.txt goalState.txt bfs finishedState.txt

// L [C,W,B]
// R [C,W,B]

// 9 moves is the optimal solution for original puzzle
// 2w >
// 1w <
// 2w >
// 1w <
// 2c >
// 1w,1c <
// 2c >
// 1w <
// 2w >

// on side with boat
    // if -1 chicken, w>c?
    // if -2 chicken, w>c (on either side?)? if not send across
    // if -1 wolf, w>c?
    // if -1 wolf & -1 chicken, w>c?
    // if -2 wolf, w>c?

    //for l->r and then again for r->l
    // l w <= c
    // w >= 0
    // (r w <= c) || c = 0

using namespace std;

struct Node
{
    int rchickens, rwolves, rboat;
    int lchickens, lwolves, lboat;
    struct Node *parentNode;
    int parentIterator;
    string action;
    int depth;
    int priority;

    bool operator<(const Node& a) const 
    {
        return  priority < a.priority;
    }
};

enum SearchType
{
    bfs,
    dfs ,
    iddfs,
    astar,
    error
};

// Prints chickens, wolves, and boats within the node
void printNode(struct Node node, string functionName)
{
    cout << functionName << "lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
    cout << functionName << "rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
}

// Pulls data from a file
struct Node getStateFromFile(char *file)
{
    struct Node state;
    fstream startStateFile;

    startStateFile.open(file);
    int x = 0;
    int *arr;
    arr = new int[6];
    string line;
    while (getline(startStateFile, line))
    {
        stringstream toint(line);
        while (getline(toint, line, ','))
        {
            if (x != 6)
            {
                stringstream geek(line);
                geek >> arr[x];
                //cout << "arr[" << x << "]:" << arr[x] << "\n";
                x++;
            }
        }
    }
    state.lchickens = arr[0];
    state.lwolves = arr[1];
    state.lboat = arr[2];
    state.rchickens = arr[3];
    state.rwolves = arr[4];
    state.rboat = arr[5];

    return state;
}

// Sets the enum value from the matching string
SearchType getEnumValue(string searchTypeString)
{
    if(searchTypeString == "bfs")
    {
        return bfs;
    } else if(searchTypeString == "dfs")
    {
        return dfs;
    } else if(searchTypeString == "iddfs")
    {
        return iddfs;
    } else if(searchTypeString == "astar")
    {
        return astar;
    } else return error;
}

// Checks if Node is the Solution
bool GoalTest(struct Node solution, struct Node node)
{
    cout << "in goal test" << endl;
    if (solution.lchickens == node.lchickens)
    {
        if (solution.lwolves == node.lwolves)
        {
            if (solution.lboat == node.lboat)
            {
                if (solution.rchickens == node.rchickens)
                {
                    if (solution.rwolves == node.rwolves)
                    {
                        if (solution.rboat == node.rboat)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

// Writes solution to output file and prints solution to screen
void Solution(struct Node node, vector<Node> closed, struct Node problem, char *outputFile)
{
    cout << "SOLUTION FOUND! " << endl; //Prints out the solution and pathway to it

    ofstream myfile;
    myfile.open (outputFile);
     
    while(!GoalTest(closed[0], node))
    {
        myfile << "lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
        myfile << "rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
        myfile << node.action << " at Depth " << node.depth << endl;

        //cout << "lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
        //cout << "rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
        cout << node.action << " at Depth " << node.depth << endl;
        
        node = closed[node.parentIterator];
    }
    
    myfile << "lc: " << problem.lchickens << ", lw: " << problem.lwolves << ", lb: " << problem.lboat << endl;
    myfile << "rc: " << problem.rchickens << ", rw: " << problem.rwolves << ", rb: " << problem.rboat << endl;
    myfile << "Creation at Depth 0" << endl;
    myfile << "Initial Node" << endl << endl;
    
    myfile << "Nodes Expanded: " << closed.size() << endl << endl;

    myfile.close();

    //cout << "lc: " << problem.lchickens << ", lw: " << problem.lwolves << ", lb: " << problem.lboat << endl;
    //cout << "rc: " << problem.rchickens << ", rw: " << problem.rwolves << ", rb: " << problem.rboat << endl;
    cout << "Creation at Depth 0" << endl;
    cout << "Inital Node" << endl << endl;
    
    cout << "Nodes Expanded: " << closed.size() << endl << endl;

}

// checks if the given node is within the closed array and as such has already been referenced before
bool isClosed(struct Node node, vector<Node> closed)
{
    for(int i = 0; i < closed.size(); i++)
    {
        if(GoalTest(closed[i], node))
        {
            return true;
        }
    }
    return false;
}

//Expands the current node to find the next fringe
vector<Node> Expand(struct Node node, struct Node problem, struct Node solution, vector<Node> closed)
{
    vector<Node> successors;

    struct Node s;

    // Parent-Node[s] = node
    s.parentIterator = closed.size() - 1;
    s.parentNode = &closed[s.parentIterator];

    if (s.parentNode->rboat == 1)
    { //trues means failure, false means good action
        //printNode(*s.parentNode, "Expand Print");
        //cout << "Possible Successors: " << endl;
        if (s.parentNode->rwolves <= s.parentNode->rchickens - 1 && s.parentNode->lwolves <= s.parentNode->lchickens + 1 && s.parentNode->rchickens - 1 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens + 1;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens - 1;
            s.rwolves = s.parentNode->rwolves;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved ONE CHICKEN to the LEFT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if (((s.parentNode->rwolves <= s.parentNode->rchickens - 2 && s.parentNode->lwolves <= s.parentNode->lchickens + 2) || s.parentNode->rchickens - 2 == 0) && s.parentNode->rchickens - 2 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens + 2;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens - 2;
            s.rwolves = s.parentNode->rwolves;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved TWO CHICKENS to the LEFT";
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if ((s.parentNode->lwolves + 1 <= s.parentNode->lchickens || s.parentNode->lchickens == 0) && s.parentNode->rwolves - 1 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens;
            s.lwolves = s.parentNode->lwolves + 1;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens;
            s.rwolves = s.parentNode->rwolves - 1;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;
            
            s.action = "Moved ONE WOLF to the LEFT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if (s.parentNode->lwolves + 1 <= s.parentNode->lchickens + 1 && s.parentNode->rwolves - 1 >= 0 && s.parentNode->rchickens - 1 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens + 1;
            s.lwolves = s.parentNode->lwolves + 1;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens - 1;
            s.rwolves = s.parentNode->rwolves - 1;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved ONE WOLF and ONE CHICKEN to the LEFT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if ((s.parentNode->lwolves + 2 <= s.parentNode->lchickens || s.parentNode->lchickens == 0 ) && s.parentNode->rwolves - 2 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens;
            s.lwolves = s.parentNode->lwolves + 2;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens;
            s.rwolves = s.parentNode->rwolves - 2;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved TWO WOLVES to the LEFT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
    }
    else
    {
        if (s.parentNode->lwolves <= s.parentNode->lchickens - 1 && s.parentNode->rwolves <= s.parentNode->rchickens + 1 && s.parentNode->lchickens - 1 >= 0)
        {
            s.lboat = 0;
            s.lchickens = s.parentNode->lchickens - 1;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 1;
            s.rchickens = s.parentNode->rchickens + 1;
            s.rwolves = s.parentNode->rwolves;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved ONE CHICKEN to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if (s.parentNode->lwolves <= s.parentNode->lchickens - 2 && s.parentNode->rwolves <= s.parentNode->rchickens + 2 && s.parentNode->lchickens - 2 >= 0)
        {
            s.lboat = 0;
            s.lchickens = s.parentNode->lchickens - 2;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 1;
            s.rchickens = s.parentNode->rchickens + 2;
            s.rwolves = s.parentNode->rwolves;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved TWO CHICKENS to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if ((s.parentNode->rwolves + 1 <= s.parentNode->rchickens || s.parentNode->rchickens == 0) && s.parentNode->lwolves - 1 >= 0)
        {
            s.lboat = 0;
            s.lchickens = s.parentNode->lchickens;
            s.lwolves = s.parentNode->lwolves - 1;
            s.rboat = 1;
            s.rchickens = s.parentNode->rchickens;
            s.rwolves = s.parentNode->rwolves + 1;
            // Depth = Depth[node] + 1
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved ONE WOLF to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if (s.parentNode->rwolves + 1 <= s.parentNode->rchickens + 1 && s.parentNode->lwolves - 1 >= 0 && s.parentNode->lchickens - 1 >= 0)
        {
            s.lboat = 0;
            s.lchickens = s.parentNode->lchickens - 1;
            s.lwolves = s.parentNode->lwolves - 1;
            s.rboat = 1;
            s.rchickens = s.parentNode->rchickens + 1;
            s.rwolves = s.parentNode->rwolves + 1;
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved ONE WOLF and ONE CHICKEN to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        if ((s.parentNode->rwolves + 2 <= s.parentNode->rchickens || s.parentNode->rchickens == 0) && s.parentNode->lwolves - 2 >= 0)
        {
            s.lboat = 0;
            s.lchickens = s.parentNode->lchickens;
            s.lwolves = s.parentNode->lwolves - 2;
            s.rboat = 1;
            s.rchickens = s.parentNode->rchickens;
            s.rwolves = s.parentNode->rwolves + 2;
            s.depth = s.parentNode->depth + 1;

            s.action = "Moved TWO WOLVES to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        // Path-Cost[s] = Path-Cast[node] + Step-Cost[node,action,s]
        // NO PATH COST FOR THESE SEARCH ALGOS
    }
    //cout << "successors size: " << successors.size() << endl;
    // return successors
    return successors;
}

// Adds the expanded nodes to the fringe either in front or back based on the searchType
vector<Node> InsertAll(vector<Node> expandedNode, vector<Node> fringe, SearchType searchType)
{
    //Inserts the next fringe from the current node into the main vector pathway
    if(searchType == bfs)
    {
        for(int i = 0; i < expandedNode.size(); i++)
        {
            fringe.push_back(expandedNode[i]);
        }
    }

    if(searchType == dfs || searchType == iddfs)
    {
        for(int i = 0; i < expandedNode.size(); i++)
        {
            fringe.insert(fringe.begin(), expandedNode[i]);
        }
    }

    return fringe;
}

// Sets the first fringe with the first Node
vector<Node> setInitialFringe(struct Node initialState)
{
    vector<Node> initialFringe;

    initialFringe.push_back(Node());
    initialFringe[0].lchickens = initialState.lchickens;
    initialFringe[0].lwolves = initialState.lwolves;
    initialFringe[0].lboat = initialState.lboat;
    initialFringe[0].rchickens = initialState.rchickens;
    initialFringe[0].rwolves = initialState.rwolves;
    initialFringe[0].rboat = initialState.rboat;
    initialFringe[0].action = "Creation";
    initialFringe[0].depth = 0;

    return initialFringe;
}

// The main function that loops through and completes a certain search functionality based on the given enum
void graphSearch(struct Node problem, struct Node solution, SearchType searchType, char *outputFile)
{
    //empty struct Node that remembers all the nodes ever touched
    vector<Node> closed;
    // insert the initial state or node to fringe
    vector<Node> fringe = setInitialFringe(problem);

    int currentDepth = 0;
    int iterativeDepth = 0;
    int maxIterativeDepth = 1400;

    while (true)
    {
        //resets for each iterative depth loop DURING IDDFS
        if(currentDepth == iterativeDepth && searchType == iddfs)
        {
            closed.erase(closed.begin(), closed.end());
            fringe.erase(fringe.begin(), fringe.end());
            fringe = setInitialFringe(problem);
            currentDepth = 0;
            iterativeDepth += 1;
            cout << "IDDFS RESET - New IterativeDepth: " << iterativeDepth << endl;
        }

        struct Node node;
        if (fringe.empty()) // null check
        {
            cout << "Failure" << endl;
            break;
        }

        node = fringe[0]; // Get Next Node

        if (GoalTest(solution, node)) 
        {
            Solution(node, closed, problem, outputFile);
            break;
        }

        int closedSize = closed.size();
        if (closedSize != 0)
        {
            for (int i = 0; i < closedSize; i++)
            {
                if ((closed[i].lwolves == node.lwolves && closed[i].lchickens == node.lchickens) && closed[i].lboat == node.lboat)
                {
                    cout << "Closed[] Contains Node Already" << endl;
                } else 
                {
                    closed.push_back(node);
                    
                    fringe.erase(fringe.begin());
                    fringe = InsertAll(Expand(node, problem, solution, closed), fringe, searchType);
                    currentDepth += 1;
                    break;
                }
            }
        } else
        {
            closed.push_back(node);

            fringe.erase(fringe.begin());
            fringe = InsertAll(Expand(node, problem, solution, closed), fringe, searchType);
            currentDepth += 1;
        }
    }
}

// A* 
// Sets the Heuristic based on the priority given by the solution and current node
struct Node setHeuristic(struct Node node, struct Node solution)
{
    node.priority = abs(solution.lwolves - node.lwolves) + abs(solution.lchickens - node.lchickens);
    if(solution.lboat == 1){
        node.priority++;
    }
    return node;
}

// Sets the initial fringe including the A* logic for Heuristics
priority_queue<Node> setInitialFringePQ(struct Node initialState, struct Node solution)
{//cout << "InitialFringe" << endl;
    priority_queue<Node> initialFringe;
    struct Node node;
    node.lchickens = initialState.lchickens;
    node.lwolves = initialState.lwolves;
    node.lboat = initialState.lboat;
    node.rchickens = initialState.rchickens;
    node.rwolves = initialState.rwolves;
    node.rboat = initialState.rboat;
    node = setHeuristic(node, solution);
    initialFringe.push(node);
    return initialFringe;
}

// Main logic for A*
void aStar(struct Node problem, struct Node solution, char* outputFile)
{
    cout << "In A*" << endl;
    //priority queue built in
    priority_queue<Node> fringe;
    vector<Node> closed;
    vector<Node> succ;
    fringe = setInitialFringePQ(problem, solution);
    cout << "Set initial fringe size: " << fringe.size() << endl;

    while(true)
    {
        if(fringe.size() == 0)
        {
            cout << "A* empty node." << endl;
            break;
        }
        struct Node node;
        node = fringe.top(); // grabs node at front of priority queue
        fringe.pop();
        cout << "after first pop" << endl;
        if(GoalTest(solution, node)){
            Solution(node, closed, problem, outputFile);
            break;
        }
        
        if(closed.size() != 0)
        {
            cout << "In expansion statement" << std::endl;
            for (int i = 0; i < closed.size() - 1; i++)
            {
                if ((closed[i].lwolves == node.lwolves && closed[i].lchickens == node.lchickens) && closed[i].lboat == node.lboat)
                {
                    cout << "Closed[] Contains Node Already" << endl;
                } else 
                {
                    succ = Expand(node, problem, solution, closed);
                    cout << "Succ size: " << succ.size() << endl;
                    for(int i = 0; i < succ.size()-1; i++)
                    {
                        succ[i] = setHeuristic(succ[i], solution);
                        fringe.push(succ[i]);
                        closed.push_back(succ[1]);
                    }
                }
            }
        }
        else
        {  
            closed.push_back(node);
        }     
    }
}

int main(int argc, char *argv[])
{
    cout << "Assignment 1 - baughd" << endl;
    Node initialState;
    Node solutionState;
    string searchTypeString;
    SearchType searchType;
    char *outputFile;
    vector<Node> statePath;

    // Get initial state from startState.txt file
    initialState = getStateFromFile(argv[1]);
    solutionState = getStateFromFile(argv[2]);
    searchTypeString = argv[3];
    searchType = getEnumValue(searchTypeString);
    outputFile = argv[4];

    // Set the initial state in the vector path
    statePath.push_back(initialState);
    if(searchType == astar){
        aStar(initialState, solutionState, outputFile);
    }
    else{
    // Complete Search - eventually should run each different search type
        graphSearch(initialState, solutionState, searchType, outputFile);
    }   
    cout << "Done" << endl;

    return 0;
}