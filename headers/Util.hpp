#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Task.hpp"
#include <algorithm>
#include <vector>
#include <sqlite3.h>
#include <cstdlib>
#include <ctime>

using namespace std;
using json = nlohmann::json;

namespace util {

// database connection
sqlite3* db = nullptr;

// initialise database and create tables
bool initDatabase() {
	int rc = sqlite3_open("tasks.db", &db);
	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	// create table if it doesn't exist
	const char* sql = "CREATE TABLE IF NOT EXISTS tasks ("
					"id INTEGER PRIMARY KEY AUTOINCREMENT,"
					"name TEXT NOT NULL,"
					"description TEXT,"
					"location TEXT,"
					"subject TEXT,"
					"list_name TEXT NOT NULL,"
					"start_time INTEGER,"
					"end_time INTEGER,"
					"status INTEGER DEFAULT 0,"
					"priority INTEGER DEFAULT 1"
					");";
	
	// execute the sql statement
	char* errMsg = 0;
	rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}
	// if successful then return true`
	return true;
}

// close database connection
void closeDatabase() {
	if (db) {
		sqlite3_close(db);
		db = nullptr;
	}
}

// load lists from data.json
json loadLists() {
	ifstream file("data.json");
	json j;
	if (file.is_open()) {
		file >> j;
		file.close();
	}
	return j;
}

// add a new task
bool addTask(const string& listName, const string& taskName, const string& description = "", 
			const string& location = "", const string& subject = "", 
			Priority priority = Priority::medium) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	json lists = loadLists();
	if (lists.find(listName) == lists.end()) {
		cerr << "List '" << listName << "' not found in data.json" << endl;
		return false;
	}
	
	const char* sql = "INSERT INTO tasks (name, description, location, subject, list_name, start_time, end_time, status, priority) "
					"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	time_t now;
	time(&now);
	
	sqlite3_bind_text(stmt, 1, taskName.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, location.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, subject.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, listName.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int64(stmt, 6, now);
	sqlite3_bind_int64(stmt, 7, now);
	sqlite3_bind_int(stmt, 8, 0); // Status::pending
	sqlite3_bind_int(stmt, 9, static_cast<int>(priority));
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to insert task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task '" << taskName << "' added to list '" << listName << "'" << endl;
	return true;
}

// edit a task
bool editTask(int taskId, const string& field, const string& newValue) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_text(stmt, 1, newValue.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, taskId);
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to update task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task " << taskId << " updated" << endl;
	return true;
}

// delete a task
bool deleteTask(int taskId) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	const char* sql = "DELETE FROM tasks WHERE id = ?;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_int(stmt, 1, taskId);
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to delete task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task " << taskId << " deleted" << endl;
	return true;
}

// show tasks from a specific list
bool showList(const string& listName) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	json lists = loadLists();
	if (lists.find(listName) == lists.end()) {
		cerr << "List '" << listName << "' not found in data.json" << endl;
		return false;
	}
	
	const char* sql = "SELECT * FROM tasks WHERE list_name = ? ORDER BY priority DESC, id;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_text(stmt, 1, listName.c_str(), -1, SQLITE_STATIC);
	
	cout << "========== " << listName << " TASKS ==========" << endl;
	bool found = false;
	
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		found = true;
		int id = sqlite3_column_int(stmt, 0);
		const char* name = (const char*)sqlite3_column_text(stmt, 1);
		const char* description = (const char*)sqlite3_column_text(stmt, 2);
		const char* location = (const char*)sqlite3_column_text(stmt, 3);
		const char* subject = (const char*)sqlite3_column_text(stmt, 4);
		int status = sqlite3_column_int(stmt, 8);
		int priority = sqlite3_column_int(stmt, 9);
		
		cout << "Task ID: " << id << endl;
		cout << "Name: " << (name ? name : "") << endl;
		cout << "Description: " << (description ? description : "") << endl;
		cout << "Location: " << (location ? location : "") << endl;
		cout << "Subject: " << (subject ? subject : "") << endl;
		cout << "Status: " << (status == 0 ? "Pending" : "Completed") << endl;
		cout << "Priority: ";
		switch (priority) {
			case 0: cout << "Low"; break;
			case 1: cout << "Medium"; break;
			case 2: cout << "High"; break;
			default: cout << "Unknown"; break;
		}
		cout << endl << endl;
	}
	
	sqlite3_finalize(stmt);
	
	if (!found) {
		cout << "No tasks found in list '" << listName << "'" << endl;
	}
	
	return true;
}

// show all tasks
bool showAll() {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	const char* sql = "SELECT * FROM tasks ORDER BY list_name, priority DESC, id;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	string currentList = "";
	bool found = false;
	
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		found = true;
		const char* listName = (const char*)sqlite3_column_text(stmt, 5);
		
		if (currentList != listName) {
			currentList = listName;
			cout << "========== " << listName << " TASKS ==========" << endl;
		}
		
		int id = sqlite3_column_int(stmt, 0);
		const char* name = (const char*)sqlite3_column_text(stmt, 1);
		const char* description = (const char*)sqlite3_column_text(stmt, 2);
		const char* location = (const char*)sqlite3_column_text(stmt, 3);
		const char* subject = (const char*)sqlite3_column_text(stmt, 4);
		int status = sqlite3_column_int(stmt, 8);
		int priority = sqlite3_column_int(stmt, 9);
		
		cout << "Task ID: " << id << endl;
		cout << "Name: " << (name ? name : "") << endl;
		cout << "Description: " << (description ? description : "") << endl;
		cout << "Location: " << (location ? location : "") << endl;
		cout << "Subject: " << (subject ? subject : "") << endl;
		cout << "Status: " << (status == 0 ? "Pending" : "Completed") << endl;
		cout << "Priority: ";
		switch (priority) {
			case 0: cout << "Low"; break;
			case 1: cout << "Medium"; break;
			case 2: cout << "High"; break;
			default: cout << "Unknown"; break;
		}
		cout << endl << endl;
	}
	
	sqlite3_finalize(stmt);
	
	if (!found) {
		cout << "No tasks found" << endl;
	}
	
	return true;
}

// update task status or priority
bool updateTask(int taskId, const string& field, int value) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_int(stmt, 1, value);
	sqlite3_bind_int(stmt, 2, taskId);
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to update task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task " << taskId << " updated" << endl;
	return true;
}

// Get current value of a task field
string getTaskField(int taskId, const string& field) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return "";
	}
	
	string sql = "SELECT " + field + " FROM tasks WHERE id = ?;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return "";
	}
	
	sqlite3_bind_int(stmt, 1, taskId);
	
	string result = "";
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		const char* value = (const char*)sqlite3_column_text(stmt, 0);
		if (value) {
			result = value;
		}
	}
	
	sqlite3_finalize(stmt);
	return result;
}

// Parse time string to time_t
time_t parseTime(const string& result) {
	time_t time;
	struct tm datetime;
	
	// expected format is dd/mm/yyyy hh:mm
	try {
		int index = result.find(" ");
		string date = result.substr(0, index);
		string time_string = result.substr(index + 1, result.length() - index);
		
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
		return time;
	}
	catch (...) {
		cout << "Invalid Time string format" << endl;
		cout << "Please ensure the format is dd/mm/yyyy hh:mm" << endl;
		cout << "You provided: " << result << endl;
		return 0;
	}
}

// Open file for editing with current value
bool openFileForEdit(const string& currentValue) {
	string cmd = "touch ~/.cache/foo.txt";
	int res = system(cmd.c_str());
	if (res != 0) {
		cout << "An error occurred when creating the file" << endl;
		return false;
	}
	
	string home = getenv("HOME");
	ofstream file(home + "/.cache/foo.txt");
	file << currentValue;
	file.close();
	
	cmd = "nano ~/.cache/foo.txt";
	res = system(cmd.c_str());
	if (res != 0) {
		cout << "An error occurred trying to open the text editor" << endl;
		return false;
	}
	
	return true;
}

// Read edited content from file
string readEditedContent() {
	string home = getenv("HOME");
	ifstream file(home + "/.cache/foo.txt");
	string input;
	string result = "";
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
bool editTaskInteractive(int taskId, const string& field) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	// Get current value
	string currentValue = getTaskField(taskId, field);
	if (currentValue.empty() && field != "description") {
		cerr << "Task " << taskId << " not found or field '" << field << "' is empty" << endl;
		return false;
	}
	
	// Open editor with current value
	if (!openFileForEdit(currentValue)) {
		return false;
	}
	
	// Read edited content
	string newValue = readEditedContent();
	
	// Handle special cases for time fields
	if (field == "start_time" || field == "end_time") {
		time_t parsedTime = parseTime(newValue);
		if (parsedTime == 0) {
			return false;
		}
		return updateTask(taskId, field, static_cast<int>(parsedTime));
	}
	
	// Update the field
	string sql = "UPDATE tasks SET " + field + " = ? WHERE id = ?;";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_text(stmt, 1, newValue.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, taskId);
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to update task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task " << taskId << " field '" << field << "' updated" << endl;
	return true;
}

}