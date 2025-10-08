#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
    // argc is number of args
    // argv are the args passed in as a vector
    
    while (true) {
        string line;
        cout << "> ";
        getline(cin, line);

        if (line == "exit") break;
        
        
        istringstream iss(line);
        vector<string> tokens;
        string word;


        while (iss >> word) {
            tokens.push_back(word);
        }

        if (tokens.empty()) continue; // skip empty lines


        


    }
    
    return 0;
}
