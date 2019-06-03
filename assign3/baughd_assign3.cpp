#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include <iterator>
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

vector<string> getClassLabels(vector<string> data)
{
    vector<string> results;
    for (int i = 0; i < data.size(); i++)
    {
        vector<string> temp;
        istringstream ss(data[i]);

        // Traverse through all words
        do
        {
            // Read a word
            string word;
            ss >> word;

            // Print the read word
            if (isdigit(word[0]))
            {
                temp.push_back(word);
            }

            // While there is more to read
        } while (ss);
        results.push_back(temp.back());
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
            if (!isdigit(word[0]) && (find(words.begin(), words.end(), word) == words.end() || words.size() == 0) && word != "\t" && word != " " && word != "")
            {
                words.push_back(word);
            }

            // While there is more to read
        } while (ss);
    }
    sort(words.begin(), words.end());
    return words;
}

vector<vector<string>> convertIntoFeatures(vector<string> data, vector<string> vocab, vector<string> classLabels)
{
    int M = vocab.size();
    vector<vector<string>> features;
    features.push_back(vocab);
    for (int i = 0; i < data.size(); i++)
    {
        vector<string> feature;
        string value;
        features.push_back(feature);
        for (int j = 0; j < vocab.size(); j++)
        {
            string vocabWord = vocab[j];
            int size = data[i].size();
            //cout << data[i][size-2] << endl;
            if (data[i].find(vocabWord) == vocabWord[0])
            {
                cout << "convert: YES to 1. Found -" << vocabWord << "- in " << data[i] << endl;
                value = "1";
            } else
            {
                value = "0";
            }
            features[i].push_back(value);
        }
        features[i].push_back(classLabels[i]);
    }

    return features;
}

void outputToFile(vector<vector<string>> preprocessedVector, string fileName)
{
    ofstream output_file(fileName);
    ostream_iterator<string> output_iterator(output_file, ",");
    for (int i = 0; i < preprocessedVector.size(); i++)
    {
        copy(preprocessedVector.at(i).begin(), preprocessedVector.at(i).end(), output_iterator);
        output_file << '\n';
    }
}



// # assign probability of good/bad vocab
// def assignVocabProb(vocab, training, truth):
//     tRecords = 0 # num of trues ( good )
//     fRecords = 0 # num of false ( Bad )
//     for value in truth:
//         if value == 1:
//             tRecords += 1
//         else:
//             fRecords += 1
//     result = {}
//     for index, entry in enumerate(vocab):
//         ttCount = 0 # true/true
//         ftCount = 0 # false/true
//         ffCount = 0 # false/false
//         tfCount = 0 # true/false
//         for sen in training:
//             if sen[index] == 1:
//                 if sen[-1] == 1:
//                     ttCount += 1
//                 else:
//                     tfCount += 1
//             else:
//                 if sen[-1] == 1:
//                     ftCount += 1
//                 else:
//                     ffCount += 1
//         probs = (ttCount / tRecords, ftCount/tRecords, tfCount/fRecords, ffCount/fRecords)
//         result[entry] = probs
//     return result, (tRecords/len(truth), fRecords/len(truth)) # result array and prob

// #
// def Test(sentences, trained, probs, vocab):
//     res = []
//     probTrue = probs[0]
//     probFalse = probs[1]
//     for sentence in sentences:
//         probT = probTrue
//         # go through every word and check to see if its in given trained data for both t/f
//         for index, word in enumerate(sentence):
//             if vocab[index] in trained:
//                 if word == 1:
//                     probT *= trained[vocab[index]][0]
//                 else:
//                     probT *= trained[vocab[index]][1]
//         probF = probFalse
//         for index, word in enumerate(sentence):
//             if vocab[index] in trained:
//                 if word == 1:
//                     probF *= trained[vocab[index]][2]
//                 else:
//                     probF *= trained[vocab[index]][3]
//         if probT >= probF:
//             res.append(1)
//         else:
//             res.append(0)
//     return res

// # compares accuracy of pos or neg review with real value
// def checkAccuracy(calc, real):
//     correct = 0
//     for i in range(len(real)):
//         if calc[i] == real[i]:
//             correct += 1
//     return correct / len(real)


int main(int argc, char *argv[])
{
    cout << "Assignment 3 - David Baugh" << endl;
    cout << "Get Data" << endl;
    vector<string> trainingSet = removePunctuationAndCaseFromFile(argv[1]);
    vector<string> testingSet = removePunctuationAndCaseFromFile(argv[2]);

    cout << "Pre-Processing" << endl;
    vector<string> classLabels = getClassLabels(trainingSet);
    vector<string> trainingVocab = createVocab(trainingSet);

    vector<vector<string>> preprocessed_train = convertIntoFeatures(trainingSet, trainingVocab, classLabels);
    vector<vector<string>> preprocessed_test = convertIntoFeatures(testingSet, trainingVocab, classLabels);

    outputToFile(preprocessed_train, "preprocessed_train.txt");
    outputToFile(preprocessed_test, "preprocessed_test.txt");

    // print("training...\n")
    // trainVocab, (p1, p2) = assignVocabProb(vocab, training, trainTruth)
    
    // print("Classifing...\n")
    // trainClassification = Test(training, trainVocab, (p1, p2), vocab)
    // testClassification = Test(testing, trainVocab,(p1, p2), vocab)

    // print("Checking accuracy...\n")
    // res1 = checkAccuracy(trainClassification, trainTruth)
    // res2 = checkAccuracy(testClassification, testTruth)


    int test = 0;
    for (int i = 0; i < preprocessed_train.size(); i++)
    {
        for (int j = 0; j < preprocessed_train[i].size(); j++)
        {
            if (preprocessed_train[i][j] == "1")
            {
                test++;
            }
            //cout << preprocessed_train[i][j] << endl;
        }
    }
    cout << "test: " << test << endl;
    return 0;
}