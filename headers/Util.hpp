#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#pragma once

#include <iostream>
#include <fstream>
#include "Task.hpp"
#include "Config.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <cstdlib>
#include <map>
#include <ctime>

namespace util {

    // database connection
    sqlite3* db = nullptr;

    // colour map for output
    std::map<std::string, std::vector<std::string> > colours = {
        {"red", {"\033[31m","\033[41m"}},
        {"green", {"\033[32m","\033[42m"}},
        {"yellow", {"\033[33m","\033[43m"}},
        {"blue", {"\033[34m","\033[44m"}},
        {"magenta", {"\033[35m","\033[45m"}},
        {"cyan", {"\033[36m","\033[46m"}},
        {"white", {"\033[37m","\033[47m"}},
        {"default", {"\033[39m","\033[49m"}},
        {"black", {"\033[30m","\033[40m"}},
    };

    // INITIALISE DATABASE
    bool initDatabase() {
        // check if database file exists
        int rc = sqlite3_open("tasks.db", &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        // create tables if they don't exist
        const char* createTables[] = {
            // tasks table (without list_name)
            "CREATE TABLE IF NOT EXISTS tasks ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "description TEXT,"
            "location TEXT,"
            "subject TEXT,"
            "start_time TIMESTAMP,"
            "end_time TIMESTAMP,"
            "status INTEGER DEFAULT 0,"
            "priority INTEGER DEFAULT 1"
            ");",

            // lists table
            "CREATE TABLE IF NOT EXISTS subjects ("
            "name TEXT PRIMARY KEY UNIQUE NOT NULL,"
            "color TEXT NOT NULL"
            ");",
        };


        // execute all create table statements
        char* errMsg = 0;
        for (int i = 0; i < 2; i++) {
            // execute for each table
            rc = sqlite3_exec(db, createTables[i], 0, 0, &errMsg);

            // error handling
            if (rc != SQLITE_OK) {
                std::cerr << "SQL error creating table " << i << ": " << errMsg << std::endl;
                sqlite3_free(errMsg);
                return false;
            }
        }

        // return true if successful
        return true;
    }

    // close database connection
    void closeDatabase() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    // Parse time std::string to time_t
    time_t parseTime(const std::string& str) {
        time_t time;
        struct tm datetime;

        // expected format is dd/mm/yyyy hh:mm
        try {
            int index = str.find(" ");
            std::string date = str.substr(0, index);
            std::string time_string = str.substr(index + 1, str.length() - index);
            index = date.find("/");
            datetime.tm_mday = stoi(date.substr(0, index));
            date = date.substr(index + 1, date.length() - 2);

            index = date.find("/");
            datetime.tm_mon = stoi(date.substr(0, index)) - 1;
            date = date.substr(index + 1, date.length() - 2);
            datetime.tm_year = stoi(date.substr(0, 4)) - 1900;

            index = time_string.find(":");
            datetime.tm_hour = stoi(time_string.substr(0, index));
            datetime.tm_min = stoi(time_string.substr(index + 1, 2));

            datetime.tm_isdst = -1;

            time = mktime(&datetime);

            // returns unix time
            return time;
        }
        catch (...) {
            std::cout << "Invalid Time std::string format" << std::endl;
            std::cout << "Please ensure the format is dd/mm/yyyy hh:mm" << std::endl;
            std::cout << "You provided: " << str << std::endl;
            return 0;
        }
    }

    // check valid colour
    bool isValidColour(std::string colour) {
        return colours.find(colour) != colours.end();
    }

    // allows for name and hex colours
    std::string colourText(std::string text, std::string foreground = "default", std::string background = "default") {
        // check if valid colour
        if (!foreground.empty() && !isValidColour(foreground)) {
            foreground = "white";
        }
        if (!background.empty() && !isValidColour(background)) {
            background = "default";
        }

        std::string result = "";

        //foreground
        if (colours.find(foreground) != colours.end()) {
            result += colours[foreground][0];
        }

        // background
        if (colours.find(background) != colours.end()) {
            result += colours[background][1];
        }

        result += text;
        result += "\033[0m";
        return result;
    }

    // print a given task
    void printTask(Task* task) {
        // max 30 chars in progress bar
        int max = 30;
        int filled = std::floor((task->getStatus() * max) / 100);
        std::string output = "Task ID: " + std::to_string(task->getId()) + "\t";
        output += "Name: " + task->getName() + "\t";
        output += "Subject: " + task->getSubject() + "\t";
        output += "Status: " + std::to_string(task->getStatus()) + "/100\t";
        output += "Priority: " + std::to_string(task->getPriority()) + "/5\t";
        
        // status bar
        for (int i = 0; i < max; i++) {
            if (i < filled) {
                output += "█";
            }
            else {
                output += "▒";
            }
        }
        

        std::string colour = config::getColour(db, task->getSubject());

        // get code for colour
        std::string mod = colours[colour][0];

        // output with colour
        std::cout << colourText(output, colour) << std::endl;
    }

    // add a new task
    bool writeTask(Task task) {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }
        const char* sql = "INSERT INTO tasks (name, description, location, subject, start_time, end_time, status, priority) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        // prepare statement
        sqlite3_bind_text(stmt, 1, task.getName().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, task.getLocation().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, task.getSubject().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 5, task.getStart());
        sqlite3_bind_int64(stmt, 6, task.getEnd());
        sqlite3_bind_int(stmt, 7, task.getStatus()); // Status::pending
        sqlite3_bind_int(stmt, 8, task.getPriority());

        // pass statement into db
        rc = sqlite3_step(stmt);

        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to insert task: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        // ensure subject is added to db
        config::addSubject(db, task.getSubject(), "default");

        std::cout << "Task '" << task.getName() << "' added to subject '" << task.getSubject() << "'" << std::endl;
        return true;
    }

    // edit a task
    bool editTask(int taskId, const std::string& field, const std::string& newValue) {
        // check database
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        std::string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        // bind the appropriate type of value depending on field
        if (field == "priority" || field == "status") {
            sqlite3_bind_int(stmt, 1, stoi(newValue));
        }
        else if (field == "start_time" || field == "end_time") {
            time_t time = parseTime(newValue);
            sqlite3_bind_int64(stmt, 1, time);
        }
        else {
            sqlite3_bind_text(stmt, 1, newValue.c_str(), -1, SQLITE_STATIC);
        }

        sqlite3_bind_int(stmt, 2, taskId);

        rc = sqlite3_step(stmt);

        std::cout << sql << std::endl;
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to update task: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::cout << "Task " << taskId << " updated" << std::endl;
        return true;
    }

    // edit task overload for time
    bool editTask(int taskId, const std::string& field, time_t newValue) {
        // check database
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        std::string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        // for time we dont need to parse it, just bind it immeditately as 64-bit int
        time_t time = newValue;
        sqlite3_bind_int64(stmt, 1, time);

        sqlite3_bind_int(stmt, 2, taskId);

        rc = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to update task: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::cout << "Task " << taskId << " updated" << std::endl;
        return true;
    }

    // delete a task
    bool deleteTask(int taskId) {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        const char* sql = "DELETE FROM tasks WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, taskId);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to delete task: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::cout << "Task " << taskId << " deleted" << std::endl;
        return true;
    }

    // read all tasks with the provided query
    std::vector<Task* > readTasks(int rc, sqlite3_stmt* stmt) {
        std::vector<Task*> tasks;
        Task* curr_task;
        // loop while we still have a row
           while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);

            // deconstruct row into task object
               const char* name = (const char*)sqlite3_column_text(stmt, 1);
               const char* description = (const char*)sqlite3_column_text(stmt, 2);
               const char* location = (const char*)sqlite3_column_text(stmt, 3);
               const char* subject = (const char*)sqlite3_column_text(stmt, 4);
            uint64_t start_time = sqlite3_column_int64(stmt, 5);
            uint64_t end_time = sqlite3_column_int64(stmt, 6);
               int status = sqlite3_column_int(stmt, 7);
               int priority = sqlite3_column_int(stmt, 8);
            curr_task = new Task(name, description, location, subject, priority);
            curr_task->setId(id);
            curr_task->setStatus(status);
            curr_task->setStart(static_cast<time_t>(start_time));
            curr_task->setEnd(static_cast<time_t>(end_time));
            tasks.push_back(curr_task);
        }
           sqlite3_finalize(stmt);
        return tasks;
    }

    // show tasks from a specific subject
    bool showSubject(const std::string& subjectName) {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        const char* sql = "SELECT * FROM tasks WHERE subject = ? ORDER BY priority DESC, id;";

           sqlite3_stmt* stmt;
           int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
           if (rc != SQLITE_OK) {
              std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
              return false;
           }

           sqlite3_bind_text(stmt, 1, subjectName.c_str(), -1, SQLITE_STATIC);

        std::vector<Task* > tasks = readTasks(rc, stmt);

        std::cout << "========== " << subjectName << " TASKS ==========" << std::endl;

        for (int i=0; i<(int)tasks.size() ; i++) {
            printTask(tasks[i]);
        }
        return true;
    }

    // show all tasks
    bool showAll() {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        const char* sql = "SELECT * FROM tasks ORDER BY subject, priority DESC, id;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        std::vector<Task *> tasks = readTasks(rc, stmt);
        for (int i=0; i<(int)tasks.size(); i++) {
            printTask(tasks[i]);
        }
        if (tasks.size() == 0) {
            std::cout << "No Tasks Found" << std::endl;
        }
        return true;
    }

    // Get current value of a task field
    std::string getTaskField(int taskId, const std::string& field) {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return "";
        }

        std::string sql = "SELECT " + field + " FROM tasks WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return "";
        }

        sqlite3_bind_int(stmt, 1, taskId);

        std::string result = "";
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* value = (const char*)sqlite3_column_text(stmt, 0);
            if (value) {
                if (field == "start_time" || field == "end_time") {
                    long time = (std::stol(value, NULL, 10));
                    datetime = *localtime(&time);
                    result = std::to_string(datetime.tm_mday) + "/"
                        + std::to_string(datetime.tm_mon + 1) + "/"
                        + std::to_string(datetime.tm_year + 1900) + " "
                        + std::to_string(datetime.tm_hour) + ":";
                    if (datetime.tm_min < 10) {
                        result += "0";
                    }
                    result += std::to_string(datetime.tm_min);
                }
                else {
                    result = value;
                }

            }
        }

        sqlite3_finalize(stmt);
        return result;
    }

    // Open file for editing with current value
    bool openFileForEdit(const std::string& currentValue) {
        std::string cmd = "touch ~/.cache/foo.txt";
        int res = system(cmd.c_str());
        if (res != 0) {
            std::cout << "An error occurred when creating the file" << std::endl;
            return false;
        }
        // get user dir
        std::string home = getenv("HOME");
        std::ofstream file(home + "/.cache/foo.txt");
        file << currentValue;
        file.close();

        cmd = "nano ~/.cache/foo.txt";
        res = system(cmd.c_str());
        if (res != 0) {
            std::cout << "An error occurred trying to open the text editor" << std::endl;
            return false;
        }

        return true;
    }

    // Read edited content from file
    std::string readEditedContent() {
        std::string home = getenv("HOME");
        std::ifstream file(home + "/.cache/foo.txt");
        std::string input;
        std::string result = "";
        while (getline(file, input)) {
            result += input + "\n";
        }
        file.close();

        // Remove trailing newline
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    }

    // Interactive edit task function
    bool editTaskInteractive(int taskId, const std::string& field) {
        if (!db) {
            std::cerr << "Database not initialized" << std::endl;
            return false;
        }

        // Get current value
        std::string currentValue = getTaskField(taskId, field);
        if (currentValue.empty() && field != "description") {
            std::cerr << "Task " << taskId << " not found or field '" << field << "' is empty" << std::endl;
            return false;
        }
        // Open editor with current value
        if (!openFileForEdit(currentValue)) {
            return false;
        }

        // Read edited content
        std::string newValue = readEditedContent();

        // Handle special cases for time fields
        if (field == "start_time" || field == "end_time") {
            time_t parsedTime = parseTime(newValue);
            if (parsedTime == 0) {
                return false;
            }
            return editTask(taskId, field, parsedTime);
        }

        // Update the field
        std::string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, newValue.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, taskId);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to update task: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        std::cout << "Task " << taskId << " field '" << field << "' updated" << std::endl;
        return true;
    }
};

#endif
