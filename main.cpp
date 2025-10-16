#include <iostream>
#include <string>
#include "headers/Task.hpp"
#include "headers/Util.hpp"
#include "headers/Config.hpp"


int main(int argc, char *argv[]) {
	// show usage
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <command> [arguments...]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  add <list_name> <task_name> [description] [location] [subject] [priority]" << std::endl;
        std::cout << "  edit <task_id> <field> [new_value]" << std::endl;
        std::cout << "  delete <task_id>" << std::endl;
        std::cout << "  show <subject_name>" << std::endl;
        std::cout << "  show all" << std::endl;
        return 1;
    }

    // initialise and if fails then return 1
    if (!util::initDatabase()) {
        std::cerr << "Failed to initialise database" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "add") {
		// usage
        if (argc < 4) {
            std::cout << "Usage: add --flag <value> [--flag <value> ...]" << std::endl;
            std::cout << "Required flags: --name --subject" << std::endl;
            std::cout << "Optional flags: --description, --location, --priority" << std::endl;
            std::cout << "Priority: 1-5" << std::endl;
            return 1;
        }

		// parse args
        std::unordered_map<std::string, std::string> flags;
        bool hasName = false;
        bool hasSubject = false;
        std::string currentFlag = "";
        std::string currentValue = "";

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
            std::cout << util::colourText("Error: --name is required", "red") << std::endl;
            return 1;
        }

        // check for subject
        if (!hasSubject) {
            std::cout << util::colourText("Error: --subject is required", "red") << std::endl;
            return 1;
        }

        // get values from flags
        std::string taskName = flags["name"];
        std::string subject = flags["subject"];
        // if flags exists then get the value, otherwise use empty string
        std::string description = flags.count("description") > 0 ? flags["description"] : "";
        std::string location = flags.count("location") > 0 ? flags["location"] : "";
        int priority = flags.count("priority") > 0 ? stoi(flags["priority"])%5 +1  : 0;

        // create task
        Task task = Task(taskName, description, location, subject, priority);
        util::writeTask(task);
    } else if (command == "edit") {
        // usage
        if (argc < 4) {
            std::cout << "Usage: edit <task_id> <field> [new_value]" << std::endl;
            std::cout << "Fields: name, description, location, subject, start_time, end_time, status" << std::endl;
            std::cout << "If new_value is omitted, nano editor will open with current value" << std::endl;
            return 1;
        }

        // args
        int taskId = std::stoi(argv[2]);
        std::string field = argv[3];
        std::vector<std::string> valid = {"name", "description", "location", "subject", "start_time", "end_time", "status"};
        if (find(valid.begin(), valid.end(), field) == valid.end())
        {
            std::cout << "Invalid Field" << std::endl;
            return 0;
        }
        // check if new_value is provided
        if (argc >= 5) {
            // direct edit with provided value
            std::string newValue = argv[4];
            util::editTask(taskId, field, newValue);
        } else {
            // interactive edit - open editor
            util::editTaskInteractive(taskId, field);
        }

    } else if (command == "delete") {
		// usage
        if (argc < 3) {
            std::cout << "Usage: delete <task_id>" << std::endl;
            return 1;
        }

		// args
        int taskId = std::stoi(argv[2]);
        util::deleteTask(taskId);

    } else if (command == "show") {
		// usage
        if (argc < 3) {
            std::cout << "Usage: show <subject_name> or show all" << std::endl;
            return 1;
        }

		// args
        std::string arg = argv[2];
        if (arg == "all") {
            util::showAll();
        }
        else {
            util::showSubject(arg);
        }

    }
    else if (command == "set") {
        if (argc < 4) {
            std::cout << "Usage: set <subject_name> <colour> or show all" << std::endl;
            std::cout << "Valid colours: red, green, yellow, blue, magenta, cyan, white, default, black" << std::endl;
            return 1;
        }
        std::string subject = argv[2];
        std::string colour = argv[3];
        if (!util::isValidColour(colour)) {
            std::cout << "Invalid Colour! Please choose from the list below" << std::endl;
            std::cout << "Valid colours: red, green, yellow, blue, magenta, cyan, white, default, black" << std::endl;
        }
        config::editSubjectColor(util::db, subject, colour);
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Available commands: add, edit, delete, show" << std::endl;
        return 1;
    }

    // Close database
    util::closeDatabase();
    return 0;
}
