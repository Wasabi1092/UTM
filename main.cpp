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
        cout << "  update <task_id> <field> <value>" << endl;
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
            cout << "Usage: add <task_name> [description] [location] [subject] [priority]" << endl;
            cout << "Priority: 1-5" << endl;
            return 1;
        }

		// args
        string taskName = argv[2];
        string description = (argc > 3) ? argv[3] : "";
        string location = (argc > 4) ? argv[4] : "";
        string subject = (argc > 5) ? argv[5] : "";
        int priority = (argc > 6) ? stoi(argv[6])%5 +1  : 0;
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

    } else if (command == "edit-interactive") {
        // usage
        if (argc < 4) {
            cout << "Usage: edit-interactive <task_id> <field>" << endl;
            cout << "Fields: name, description, location, subject, start_time, end_time" << endl;
            cout << "This will open nano editor with current value for editing" << endl;
            return 1;
        }

        // args
        int taskId = stoi(argv[2]);
        string field = argv[3];

        // edit the task interactively
        util::editTaskInteractive(taskId, field);

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

    } else if (command == "update") {
		// usage
        if (argc < 5) {
            cout << "Usage: update <task_id> <field> <value>" << endl;
            cout << "Fields: status (0=pending, 1=completed), priority (0=low, 1=medium, 2=high)" << endl;
            return 1;
        }

		// args
        int taskId = stoi(argv[2]);
        string field = argv[3];
        int value = stoi(argv[4]);

		// update the task
        util::updateTask(taskId, field, value);

    } else {
        cout << "Unknown command: " << command << endl;
        cout << "Available commands: add, edit, edit-interactive, delete, show, update" << endl;
        return 1;
    }

    // Close database
    util::closeDatabase();
    return 0;
}
