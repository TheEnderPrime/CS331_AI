#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

//COMPILE : g++ -o assign1 assign1.cpp -static-libstdc++ -static-libgcc
//RUN     : assign1 startState.txt goalState.txt --MODE-- finishedState.txt

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

using namespace std;

struct Node
{
    int rchickens, rwolves, rboat;
    int lchickens, lwolves, lboat;
    struct Node *parentNode;
    string action;
    int depth;
};

enum SearchType
{
    bfs,
    dfs,
    iddfs,
    astar
};

void printNode(struct Node node, string functionName)
{
    //cout << functionName << " NODE: lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
    //cout << functionName << " NODE: rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
}

struct Node getStateFromFile(char *file)
{//cout << "getStateFromFile" << endl;
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

// Checks if Node is the Solution
bool GoalTest(struct Node solution, struct Node node)
{
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

void Solution(struct Node node)
{
    cout << "SOLUTION FOUND! " << endl; //Prints out the solution and pathway to it
    while(true)
    {
        cout << " NODE: lc: " << node.parentNode->lchickens << ", lw: " << node.parentNode->lwolves << ", lb: " << node.parentNode->lboat << endl;
        cout << " NODE: rc: " << node.parentNode->rchickens << ", rw: " << node.parentNode->rwolves << ", rb: " << node.parentNode->rboat << endl;
        cout << " NODE: action: " << node.action << " at depth " << node.depth << &node.parentNode->parentNode << endl;

        node.lchickens = node.parentNode->lchickens;
        node.lwolves = node.parentNode->lwolves;
        node.lboat = node.parentNode->lboat;
        node.rchickens = node.parentNode->rchickens;
        node.rwolves = node.parentNode->rwolves;
        node.rboat = node.parentNode->rboat;
        node.action = node.parentNode->action;
        node.depth = node.parentNode->depth;
        node.parentNode = node.parentNode->parentNode;

        if(node.parentNode) break;
    }
}

struct Node getParentNode(struct Node node)
{//cout << "getParentNode" << endl;
    return *node.parentNode;
}

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
{//cout << "Expand" << endl;
    vector<Node> successors;
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

    struct Node s;
    // Parent-Node[s] = node
    s.parentNode = &node;

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

            s.action = "Moved TWO WOLVES to the RIGHT";
            if(!isClosed(s, closed))
            {
                successors.push_back(s);
                //cout << s.action << endl;
            }
        }
        // Path-Cost[s] = Path-Cast[node] + Step-Cost[node,action,s]
        // NO PATH COST FOR THESE SEARCH ALGOS
        // Depth = Depth[node] + 1
        s.depth = node.depth + 1;
    }
    //cout << "successors size: " << successors.size() << endl;
    // return successors
    return successors;
}

vector<Node> InsertAll(vector<Node> expandedNode, vector<Node> fringe)
{//cout << "Insert All" << endl;
    //cout << "fringeSize: " << fringe.size() << endl;
    //Inserts the next fringe from the current node into the main vector pathway
    for(int i = 0; i < expandedNode.size(); i++)
    {
        fringe.push_back(expandedNode[i]);
    }
    //cout << "fringeSize: " << fringe.size() << endl << endl << endl;
    return fringe;
}

vector<Node> setInitialFringe(struct Node initialState)
{//cout << "InitialFringe" << endl;
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

struct Node getNextNode(vector<Node> fringe, SearchType searchType)
{//cout << "getNextNode" << endl;
    if (searchType == bfs)
    {
        return fringe[0];
    }
    else
        cout << "getNextNode ERROR" << endl;
}

void graphSearch(struct Node problem, struct Node solution, SearchType searchType)
{//cout << "GraphSearch" << endl;
    //empty struct Node that remembers all the nodes
    vector<Node> closed;
    // insert the initial state or node to fringe
    vector<Node> fringe = setInitialFringe(problem);

    while (true)
    {
        struct Node node;
        // this checks if the loop is at the end and the solution was not found
        if (fringe.empty()) // null check
        {
            cout << "Failure" << endl;
            break;
        }
        //node = get first node from fringe // I THINK THIS IS WHERE WE CHANGE THE ALGO FOR SEARCH TYPES
        node = getNextNode(fringe, searchType);
        printNode(node, "GraphSearch");

        //if Goal-Test([problem], node) return solution(node)
        if (GoalTest(solution, node)) 
        {
            Solution(node);
            break;
        }

        //if node !in closed
        int closedSize = closed.size();
        //cout << "closedSize: " << closedSize << endl;
        if (closedSize != 0)
        {
            for (int i = 0; i < closedSize; i++)
            {
                if ((closed[i].lwolves == node.lwolves && closed[i].lchickens == node.lchickens) && closed[i].lboat == node.lboat)
                {
                    cout << "Closed Contains Node Already" << endl;
                } else 
                {
                    //add node to closed
                    closed.push_back(node);
                    
                    //printNode(node, "NOW CLOSED");
                    
                    fringe.erase(fringe.begin());
                    fringe = InsertAll(Expand(node, problem, solution, closed), fringe);
                    /*for(int i = 0; i < fringe.size(); i++)
                    {
                        printNode(fringe[i], ("fringe[]"));
                    }*/
                    break;
                }
            }
        } else
        {
            closed.push_back(node);

            //printNode(node, "FIRST CLOSED");

            fringe.erase(fringe.begin());
            fringe = InsertAll(Expand(node, problem, solution, closed), fringe);
            /*for(int i = 0; i < fringe.size(); i++)
            {
                printNode(fringe[i], ("initial fringe[]"));
            }*/
        }
    }
}

int main(int argc, char *argv[])
{
    cout << "Assignment 1 - baughd" << endl;
    Node initialState;
    Node solutionState;
    vector<Node> statePath;

    // Get initial state from startState.txt file
    initialState = getStateFromFile(argv[1]);
    solutionState = getStateFromFile(argv[2]);

    // Set the initial state in the vector path
    statePath.push_back(initialState);

    // Complete Search - eventually should run each different search type
    graphSearch(initialState, solutionState, bfs);

    cout << "Done" << endl;

    //look at last node
    //find fringe (the children nodes)
    //go to next node (depends on search type)
    //
    return 0;
}