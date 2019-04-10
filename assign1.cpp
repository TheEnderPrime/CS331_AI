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

int main (int argc, char *argv[]) 
{ 
    state initialState;
    vector<state> statePath;
    
    initialState = getStateFromFile(argv[1]);
    statePath.push_back(initialState);
    cout << statePath[0].rwolves;
    
    return 0; 
}