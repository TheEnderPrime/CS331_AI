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

void printNode(struct Node node)
{
    cout << "printNode" << endl;
    cout << "NODE: lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
    cout << "NODE: rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
}

struct Node getStateFromFile(char *file)
{
    cout << "getStateFromFile" << endl;
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
                            cout << "GoalTest: true" << endl;
                            return true;
                        }
                        else
                        {
                            cout << "GoalTest: false" << endl;
                            return false;
                        }
                    }
                    else
                    {
                        cout << "GoalTest: false" << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "GoalTest: false" << endl;
                    return false;
                }
            }
            else
            {
                cout << "GoalTest: false" << endl;
                return false;
            }
        }
        else
        {
            cout << "GoalTest: false" << endl;
            return false;
        }
    }
    else
    {
        cout << "GoalTest: false" << endl;
        return false;
    }
}

void Solution(struct Node node)
{
    cout << "Solution" << endl;
    cout << "SOLUTION FOUND! " << endl; //Prints out the solution and pathway to it
}

struct Node getParentNode(struct Node node)
{
    cout << "getParentNode" << endl;
    return *node.parentNode;
}

//Expands the current node to find the next fringe
vector<Node> Expand(struct Node node, struct Node problem)
{
    cout << "Expand" << endl;
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
        printNode(*s.parentNode);
        cout << s.parentNode->rwolves << endl;
        cout << s.parentNode->rchickens - 1 << endl;
        if (s.parentNode->rwolves <= s.parentNode->rchickens - 1 && s.parentNode->lwolves <= s.parentNode->lchickens + 1 && s.parentNode->rchickens - 1 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens + 1;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens - 1;
            s.rwolves = s.parentNode->rwolves;

            s.action = "Moved ONE CHICKEN to the LEFT";
            successors.push_back(s);
            cout << s.action << endl;
        }
        if (s.parentNode->rwolves <= s.parentNode->rchickens - 2 && s.parentNode->lwolves <= s.parentNode->lchickens + 2 && s.parentNode->rchickens - 2 >= 0)
        {
            s.lboat = 1;
            s.lchickens = s.parentNode->lchickens + 2;
            s.lwolves = s.parentNode->lwolves;
            s.rboat = 0;
            s.rchickens = s.parentNode->rchickens - 2;
            s.rwolves = s.parentNode->rwolves;

            s.action = "Moved TWO CHICKENS to the LEFT";
            successors.push_back(s);
            cout << s.action << endl;
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
            successors.push_back(s);
            cout << s.action << endl;
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
            successors.push_back(s);
            cout << s.action << endl;
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
            successors.push_back(s);
            cout << s.action << endl;
        }
        cout << "successors size: " << successors.size() << endl;
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
            successors.push_back(s);
            //cout << s.action << endl;
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
            successors.push_back(s);
            //cout << s.action << endl;
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
            successors.push_back(s);
            //cout << s.action << endl;
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
            successors.push_back(s);
            //cout << s.action << endl;
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
            successors.push_back(s);
            //cout << s.action << endl;
        }
        // Path-Cost[s] = Path-Cast[node] + Step-Cost[node,action,s]
        // NO PATH COST FOR THESE SEARCH ALGOS
        // Depth = Depth[node] + 1
        s.depth = node.depth + 1;
        // successors += s
    }

    cout << "EXPAND TEST successors #: " << successors.size() << endl;

    // return successors
    return successors;
}

vector<Node> InsertAll(vector<Node> expandedNode, vector<Node> fringe)
{
    cout << "Insert All" << endl;
    cout << "fringeSize: " << fringe.size() << endl;
    //Inserts the next fringe from the current node into the main vector pathway
    for(int i = 0; i < expandedNode.size(); i++)
    {
        fringe.push_back(expandedNode[i]);
    }
    cout << "fringeSize: " << fringe.size() << endl;
    return fringe;
}

vector<Node> setInitialFringe()
{
    cout << "InitialFringe" << endl;
    vector<Node> initialFringe;

    initialFringe.push_back(Node());
    initialFringe[0].lchickens = 0;
    initialFringe[0].lwolves = 0;
    initialFringe[0].lboat = 0;
    initialFringe[0].rchickens = 3;
    initialFringe[0].rwolves = 3;
    initialFringe[0].rboat = 1;
    initialFringe[0].action = "Creation";
    initialFringe[0].depth = 0;
    /*for (int i = 0; i < 3; i++)
    {
        initialFringe.push_back(Node());
        initialFringe[i].lboat = 0;
        initialFringe[i].lchickens = 0;
        initialFringe[i].lwolves = 0;
        initialFringe[i].rboat = 0;
        initialFringe[i].rchickens = 0;
        initialFringe[i].rwolves = 0;
    }

    // 0 chicken, 1 wolf, 1 boat moved
    initialFringe[0].lchickens = 0;
    initialFringe[0].lwolves = 0;
    initialFringe[0].lboat = 0;
    initialFringe[0].rchickens = 3;
    initialFringe[0].rwolves = 3;
    initialFringe[0].rboat = 1;
    initialFringe[0].action = "Creation";
    initialFringe[0].depth = 0;
    // 1 chicken, 1 wolf, 1 boat moved
    initialFringe[1].lchickens = 1;
    initialFringe[1].lwolves = 1;
    initialFringe[1].lboat = 1;
    initialFringe[1].rchickens = 2;
    initialFringe[1].rwolves = 2;
    initialFringe[1].rboat = 0;
    initialFringe[1].action = "Creation";
    initialFringe[1].depth = 0;
    // 0 chicken, 2 wolf, 1 boat moved
    initialFringe[2].lchickens = 0;
    initialFringe[2].lwolves = 2;
    initialFringe[2].lboat = 1;
    initialFringe[2].rchickens = 3;
    initialFringe[2].rwolves = 1;
    initialFringe[2].rboat = 0;
    initialFringe[2].action = "Creation";
    initialFringe[2].depth = 0;
*/
    cout << initialFringe[0].action << endl;

    return initialFringe;
}

struct Node getNextNode(vector<Node> fringe, SearchType searchType)
{
    cout << "getNextNode" << endl;
    if (searchType == bfs)
    {
        fringe.erase(fringe.begin());
        return fringe[0];
    }
    else
        cout << "getNextNode ERROR" << endl;
}

void graphSearch(struct Node problem, struct Node solution, SearchType searchType)
{
    cout << "GraphSearch" << endl;
    //empty struct Node that remembers all the nodes
    vector<Node> closed;
    // insert the initial state or node to fringe
    vector<Node> fringe = setInitialFringe();

    //for loop < 5 (only 5 possible nodes)
    while (true)
    {
        struct Node node;
        //if fringe is empty breaks loop
        // this checks if the loop is at the end and the solution was not found
        if (fringe.empty()) // null check
        {
            cout << "Failure" << endl;
            break;
        }
        //node = get first node from fringe // I THINK THIS IS WHERE WE CHANGE THE ALGO FOR SEARCH TYPES
        node = getNextNode(fringe, searchType);

        cout << "NODE: lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
        cout << "NODE: rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;

        //if Goal-Test([problem], node) return solution(node)
        if (GoalTest(solution, node))
        {
            Solution(node);
            break;
        }

        //if node !in closed
        int closedSize = closed.size();
        cout << "closedSize: " << closedSize << endl;
        if (closedSize != 0)
        {
            for (int i = 0; i < closedSize; i++)
            {
                if ((closed[i].lwolves == node.lwolves && closed[i].lchickens == node.lchickens) && closed[i].lboat == node.lboat)
                {
                    cout << "Closed Contains Node Already" << endl;
                }
                else
                {
                    //add node to closed
                    closed.push_back(node);
                    //cout << "CLOSED: lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
                    //cout << "CLOSED: rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;
                    //fringe <- insertall(EXPAND(node, problem), fringe)
                    fringe = InsertAll(Expand(node, problem), fringe);
                    break;
                }
            }
        }
        else
        {
            closed.push_back(node);

            cout << "FIRST RUN THROUGH OF CLOSED:" << endl;
            //cout << "CLOSED: lc: " << node.lchickens << ", lw: " << node.lwolves << ", lb: " << node.lboat << endl;
            //cout << "CLOSED: rc: " << node.rchickens << ", rw: " << node.rwolves << ", rb: " << node.rboat << endl;

            //fringe <- insertall(EXPAND(node, problem), fringe)
            fringe = InsertAll(Expand(node, problem), fringe);
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