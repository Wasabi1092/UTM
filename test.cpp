#include <iostream>
#include <string>
// #include "headers/Task.hpp"
#include "headers/Config.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // argc is number of args
    // argv are the args passed in as a vector
    // Task task(0);

    // task.edit(0, argv[1]);
    // cout << task.getName() << endl;
    // cout << "=====" << endl;
    // cout << task.getDescription() << endl;
    // cout << "=====" << endl;
    // cout << task.getStart() << endl;
    // cout << task.getEnd() << endl;
    // cout << "=====" << endl;
    // cout << task.getSubject() << endl;
    // cout << "=====" << endl;
    // cout << task.getLocation() << endl;

    // testing config.hpp:
    config::load();

    if (argc < 2)
    {
        std::cout << "Usage:\n"
                  << " add-subjects <name> <colour>\n"
                  << " show-subjects\n";
        return 0;
    }

    std::string command = argv[1];
    // cout << command << endl;

    if (command == "add-subjects")
    {
        if (argc < 4)
        {
            std::cout << "Usage: add-subjects <name> <colour>\n";
            return 1;
        }
        std::string name = argv[2];
        std::string colour = argv[3];

        config::addSubject(name, colour);
    }
    else if (command == "show-subjects")
    {
        config::printSubjects();
    }
    else
    {
        std::cout << "Unknown command: " << command << std::endl;
    }

    return 0;
}
