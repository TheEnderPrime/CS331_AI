#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <stdlib.h>
using namespace std;

//COMPILE : g++ -o assign3 baughd_assign3.cpp 
//COMPILE ADD ON MY DESKTOP : -static-libstdc++ -static-libgcc

vector<string> removePunctuationFromFile(char *file)
{
    fstream startFile;

    startFile.open(file);

    vector <string> results; 
    string line;
    while (getline(startFile, line))
    {
        for (int i = 0, len = line.size(); i < len; i++)
        {
            // check whether parsing character is punctuation or not
            if (ispunct(line[i]))
            {
                line.erase(i--, 1);
                len = line.size();
            }
            line[i] = tolower(line[i]);
        }
        results.push_back(line);
    }
    return results;
}

vector<string> createVocab(vector<string> rawWords)
{
    
}

int main(int argc, char *argv[])
{
    cout << "Assignment 3 - David Baugh" << endl;
    vector<string> trainingSet = removePunctuationFromFile(argv[1]);
    vector<string> testingSet = removePunctuationFromFile(argv[2]);
    cout << trainingSet[0] << endl;



    return 0;
}
