#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stdlib.h>
using namespace std;

//COMPILE : g++ -o assign3 baughd_assign3.cpp
//COMPILE ADD ON MY DESKTOP : -static-libstdc++ -static-libgcc

vector<string> removePunctuationAndCaseFromFile(char *file)
{
    fstream startFile;

    startFile.open(file);

    vector<string> results;
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
    vector<string> words;
    for (int i = 0; i < rawWords.size(); i++)
    {
        istringstream ss(rawWords[i]);

        // Traverse through all words
        do
        {
            // Read a word
            string word;
            ss >> word;

            // Print the read word
            if (!isdigit(word[0]) && (find(words.begin(), words.end(), word) == words.end() || words.size() == 0))
            {
                words.push_back(word);
            }

            // While there is more to read
        } while (ss);
    }
    sort(words.begin(), words.end());
    return words;
}

vector<string> convertIntoFeatures(vector<string> data, vector<string> vocab) {
    int M = vocab.size();
    vector<vector<int>> features;

    for(int i = 0; i < data.size(); i++){
        vector<int> feature;
        features.push_back(feature);
        for(int j=0 ; j < vocab.size(); j++) {
            if()
            {
                feature
                features[i].push_back()
            }
        }
    }
}

int main(int argc, char *argv[])
{
    cout << "Assignment 3 - David Baugh" << endl;
    vector<string> trainingSet = removePunctuationAndCaseFromFile(argv[1]);
    vector<string> testingSet = removePunctuationAndCaseFromFile(argv[2]);
    
    vector<string> trainingVocab = createVocab(trainingSet);


    for( int i = 0; i < trainingVocab.size(); i++){
        cout << trainingVocab[i] << endl;
    }
    return 0;
}
