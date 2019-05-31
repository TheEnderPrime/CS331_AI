#include <iostream>
using namespace std;


String removePunctuationFromFile(char *file)
{
    fstream startFile;

    startFile.open(file);

    vector <string> results; 
    string line;
    while (getline(startFile, line))
    {
        for (int i = 0, len = str.size(); i < len; i++)
        {
            // check whether parsing character is punctuation or not
            if (ispunct(line[i]))
            {
                line.erase(i--, 1);
                len = str.size();
            }
        }
        results.push_back
    }
    return state;
}

int main()
{
   




    return 0;
}
