#include<iostream>
#include<fstream>  
#include<vector>
#include<string>
#include<sstream>

//COMPILE : g++ -o assign1 assign1.cpp
//RUN     : assign1 startState.txt goalState.txt --MODE-- finishedState.txt

// L [C,W,B]
// R [C,W,B]

using namespace std; 

struct state{
    int rchickens, rwolves, rboat;
    int lchickens, lwolves, lboat;
};

struct state getStateFromFile(char * file)
{
    struct state state;
    fstream startStateFile;

    startStateFile.open(file);
    int x = 0;
    int * arr;
    arr = new int[6];
    string line;
    while(getline(startStateFile, line))
    {
        stringstream toint(line);
        while(getline(toint, line, ','))
        {
            if(x != 6)
            {
                stringstream geek(line);
                geek >> arr[x];
                //cout << "arr[" << x << "]:" << arr[x] << "\n";
                x++;
            }
        }
    }
    state.lchickens =  arr[0];
    state.lwolves   =  arr[1];
    state.lboat     =  arr[2];
    state.rchickens =  arr[3];
    state.rwolves   =  arr[4];
    state.rboat     =  arr[5];
    
    return state;
}

bool GoalTest(struct state solution, struct state node)
{
    return true; // Checks If Node is the Solution
}

struct state Solution(struct state node)
{
    return node; //Prints out the solution and pathway to it
}

vector<state> InsertAll(vector<state> expandedNode, vector<state> fringe)
{
    //Inserts the next fringe from the current node into the main vector pathway
}

struct state getParentNode(struct state node)
{
    return node;
}

//Expands the current node to find the next fringe
vector<state> Expand(struct state node, struct state solution)
{
    // successors < empty set
    //for each action, result in SUCCESSOR-FN[problem(STATE[node])]
}

struct state graphSearch(vector<state> statePath, struct state solution)
{
    //empty struct state -closed-
    vector<state> closed;
    // set fringe nodes into vector path
    vector<state> fringe = statePath; //this probs don't work

    //for loop < 5 (only 5 possible nodes)
    while(true)
    {
        struct state node;
        //if fringe is empty return 1
        if(!fringe[0].lchickens) // null check
            cout << "Failure" << endl;
            break;
        //node = get first node from fringe
        node = fringe[0];
        //if Goal-Test([problem], node) return solution(node)
        if(GoalTest(solution, node))
            return Solution(node);

        //if node !in closed
        int closedSize = closed.size;
        if(closed[closedSize-1].lchickens == node.lchickens && closed[closedSize-1].lwolves == node.lwolves && closed[closedSize-1].lboat == node.lboat)
        {
            //add node to closed
            closed.push_back(node);
            //fringe <- insertall(EXPAND(node, problem), fringe)
            fringe = InsertAll(Expand(node, solution), fringe);
        }
    }
}

int main (int argc, char *argv[]) 
{ 
    state initialState;
    vector<state> statePath;
    
    // Get initial state from startState.txt file
    initialState = getStateFromFile(argv[1]);
    // Set the initial state in the vector path
    statePath.push_back(initialState);
    //cout << statePath[0].rwolves;

    //look at last node
    //find fringe (the children nodes)
    //go to next node (depends on search type)
    //
    return 0; 
}