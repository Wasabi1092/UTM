#include <iostream>
#include <string>
#include "headers/ListTasks.hpp"
#include "headers/Task.hpp"

using namespace std;


int main(int argc, char* argv[])
{
    // argc is number of args
    // argv are the args passed in as a vector
    Task task(0);

    task.edit(0, argv[1]);
    cout << task.getName() << endl;
    cout << "=====" << endl;
    cout << task.getDescription() << endl;
    cout << "=====" << endl;
    cout << task.getStart() << endl;
    cout << task.getEnd() << endl;
    cout << "=====" << endl;
    cout << task.getSubject() << endl;
    cout << "=====" << endl;
    cout << task.getLocation() << endl;
    return 0;
}
