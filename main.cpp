#include <iostream>
#include <string>
#include "headers/Task.hpp"
#include "headers/Util.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	// show usage
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <command> [arguments...]" << endl;
        cout << "Commands:" << endl;
        cout << "  add <list_name> <task_name> [description] [location] [subject] [priority]" << endl;
        cout << "  edit <task_id> <field> [new_value]" << endl;
        cout << "  edit-interactive <task_id> <field>" << endl;
        cout << "  delete <task_id>" << endl;
        cout << "  show <list_name>" << endl;
        cout << "  show all" << endl;
        return 1;
    }

    // initialise and if fails then return 1
    if (!util::initDatabase()) {
        cerr << "Failed to initialise database" << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "add") {
		// usage
        if (argc < 4) {
            cout << "Usage: add --flag <value> [--flag <value> ...]" << endl;
            cout << "Required flags: --name --subject" << endl;
            cout << "Optional flags: --description, --location, --priority" << endl;
            cout << "Priority: 1-5" << endl;
            return 1;
        }

		// parse args
        unordered_map<string, string> flags;
        bool hasName = false;
        bool hasSubject = false;
        string currentFlag = "";
        string currentValue = "";

        // loop for each argument
        for (int i = 2; i < argc; i++) {
            // if the argument is a flag, add the current flag and value to the map
            if (strcmp(argv[i], "--name") == 0) {
                if (!currentFlag.empty()) {
                    flags[currentFlag] = currentValue;
                }
                currentFlag = "name";
                currentValue = "";
                hasName = true;
            } else if (strcmp(argv[i], "--description") == 0) {
                if (!currentFlag.empty()) {
                    flags[currentFlag] = currentValue;
                }
                currentFlag = "description";
                currentValue = "";
            } else if (strcmp(argv[i], "--location") == 0) {
                if (!currentFlag.empty()) {
                    flags[currentFlag] = currentValue;
                }
                currentFlag = "location";
                currentValue = "";
            } else if (strcmp(argv[i], "--subject") == 0) {
                if (!currentFlag.empty()) {
                    flags[currentFlag] = currentValue;
                }
                hasSubject = true;
                currentFlag = "subject";
                currentValue = "";
            } else if (strcmp(argv[i], "--priority") == 0) {
                if (!currentFlag.empty()) {
                    flags[currentFlag] = currentValue;
                }
                currentFlag = "priority";
                currentValue = "";
            } else {
                // if the argument is not a flag, add it to the current value
                if (!currentValue.empty()) {
                    currentValue += " ";
                }
                currentValue += argv[i];
            }
        }
        // after the loop, add the last flag and value
        if (!currentFlag.empty()) {
            flags[currentFlag] = currentValue;
        }

        // check for name
        if (!hasName) {
            cout << util::colourText("Error: --name is required", "red") << endl;
            return 1;
        }

        // check for subject
        if (!hasSubject) {
            cout << util::colourText("Error: --subject is required", "red") << endl;
            return 1;
        }

        // get values from flags
        string taskName = flags["name"];
        string subject = flags["subject"];
        // if flags exists then get the value, otherwise use empty string
        string description = flags.count("description") > 0 ? flags["description"] : "";
        string location = flags.count("location") > 0 ? flags["location"] : "";
        int priority = flags.count("priority") > 0 ? stoi(flags["priority"])%5 +1  : 0;

        // create task
        Task task = Task(taskName, description, location, subject, priority);
        util::writeTask(task);
    } else if (command == "edit") {
        // usage
        if (argc < 4) {
            cout << "Usage: edit <task_id> <field> [new_value]" << endl;
            cout << "Fields: name, description, location, subject, start_time, end_time, status" << endl;
            cout << "If new_value is omitted, nano editor will open with current value" << endl;
            return 1;
        }

        // args
        int taskId = stoi(argv[2]);
        string field = argv[3];
        vector<string> valid = {"name", "description", "location", "subject", "start_time", "end_time", "status"};
        if (find(valid.begin(), valid.end(), field) == valid.end())
        {
            cout << "Invalid Field" << endl;
            return 0;
        }
        // check if new_value is provided
        if (argc >= 5) {
            // direct edit with provided value
            string newValue = argv[4];
            util::editTask(taskId, field, newValue);
        } else {
            // interactive edit - open editor
            util::editTaskInteractive(taskId, field);
        }

    } else if (command == "delete") {
		// usage
        if (argc < 3) {
            cout << "Usage: delete <task_id>" << endl;
            return 1;
        }

		// args
        int taskId = stoi(argv[2]);
        util::deleteTask(taskId);

    } else if (command == "show") {
		// usage
        if (argc < 3) {
            cout << "Usage: show <list_name> or show all" << endl;
            return 1;
        }

		// args
        string arg = argv[2];
        if (arg == "all") {
            util::showAll();
        } else {
            util::showSubject(arg);
        }

    } else {
        cout << "Unknown command: " << command << endl;
        cout << "Available commands: add, edit, edit-interactive, delete, show, update" << endl;
        return 1;
    }

    // Close database
    util::closeDatabase();
    return 0;
}
