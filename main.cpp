#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	// argc is number of args
	// argv are the args passed in as a vector  

	std::cout << "\033[1;32m" << "✓ Success!" << "\033[0m\n";
	std::cout << "\033[1;31m" << "✗ Error!" << "\033[0m\n";
	std::cout << "\033[1;34m" << "ℹ Info message" << "\033[0m\n";
	// std::cout << "\033[2J\033[H"; // clear screen & move cursor to top



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



	// add


	// edit


	// delete









	}

	return 0;
}
