#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "headers/ListTasks.hpp"
#include "headers/Task.hpp"

int main(int argc, char *argv[]) {


	ListTasks tasksList;

	// argc is number of args
	// argv are the args passed in as a vector

	// add task, edit task, delete task, show task, update task`
	if (argv[1] == "add") {
		
	} else if (argv[1] == "edit") {
		tasksList.editTask(argv[2], argv[3], argv[4]);
        tasksList.showTask(argv[2]);
	} else if (argv[1] == "delete") {
		tasksList.deleteTask(argv[2]);
	}
	else if (argv[1] == "show") {
		tasksList.showTask(argv[2]);
	} else if (argv[1] == "update") {
		tasksList.updateTask(argv[2], argv[3], argv[4]);
	}

	return 0;
}
