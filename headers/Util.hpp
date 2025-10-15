#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "Task.hpp"
#include <algorithm>
#include <vector>
#include <sqlite3.h>
#include <cstdlib>
#include <ctime>

using namespace std;

namespace util {

// database connection
sqlite3* db = nullptr;

// INITIALISE DATABASE
bool initDatabase() {
	// check if database file exists
	int rc = sqlite3_open("tasks.db", &db);
	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
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
		"name PRIMARY KEY TEXT UNIQUE NOT NULL,"
		"color TEXT NOT NULL"
		");",
	};
	

	// execute all create table statements
	char* errMsg = 0;
	for (int i = 0; i < 2; i++) {
		// execute for each table
		rc = sqlite3_exec(db, createTables[i], 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			cerr << "SQL error creating table " << i << ": " << errMsg << endl;
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













































// add a new task
bool addTask(string& listName, string& taskName, string& description, string& location, string& subject, Priority priority = Priority::medium) {
	if (!db) {
		cerr << "Database not initialized" << endl;
		return false;
	}
	
	// prepare statement
	string sql = "SELECT COUNT(*) FROM lists WHERE name = ?;";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare check statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	// bind values
	sqlite3_bind_text(stmt, 1, listName.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(checkStmt) == SQLITE_ROW) {
		int count = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
		if (count == 0) {
			cerr << "List '" << listName << "' not found" << endl;
			return false;
		}
	}
	
	// insert task
	const char* sql = "INSERT INTO tasks (name, description, location, subject, start_time, end_time, status, priority) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
	
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	time_t now;
	time(&now);
	
	// replacing each question mark with the value
	sqlite3_bind_text(stmt, 1, taskName.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, location.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, subject.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int64(stmt, 5, now);
	sqlite3_bind_int64(stmt, 6, now);
	sqlite3_bind_int(stmt, 7, 0); // Status::pending
	sqlite3_bind_int(stmt, 8, static_cast<int>(priority));
	
	rc = sqlite3_step(stmt);
	int taskId = sqlite3_last_insert_rowid(db);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to insert task: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	// insert into task_lists junction table
	const char* junctionSql = "INSERT INTO task_lists (task_id, list_name) VALUES (?, ?);";
	rc = sqlite3_prepare_v2(db, junctionSql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare junction statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_int(stmt, 1, taskId);
	sqlite3_bind_text(stmt, 2, listName.c_str(), -1, SQLITE_STATIC);
	
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	if (rc != SQLITE_DONE) {
		cerr << "Failed to link task to list: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	cout << "Task '" << taskName << "' added to list '" << listName << "'" << endl;
	return true;
}

// edit a task
bool editTask(int taskId, const string& field, const string& newValue) {
	// check database
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
	
	// delete from junction table first
	const char* junctionSql = "DELETE FROM task_lists WHERE task_id = ?;";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, junctionSql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare junction statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_int(stmt, 1, taskId);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	
	// delete from tasks table
	const char* sql = "DELETE FROM tasks WHERE id = ?;";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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
	
	// check if list exists
	const char* checkSql = "SELECT COUNT(*) FROM lists WHERE name = ?;";
	sqlite3_stmt* checkStmt;
	int rc = sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, 0);
	if (rc != SQLITE_OK) {
		cerr << "Failed to prepare check statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	
	sqlite3_bind_text(checkStmt, 1, listName.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(checkStmt) == SQLITE_ROW) {
		int count = sqlite3_column_int(checkStmt, 0);
		sqlite3_finalize(checkStmt);
		if (count == 0) {
			cerr << "List '" << listName << "' not found" << endl;
			return false;
		}
	}
	
	const char* sql = "SELECT t.id, t.name, t.description, t.location, t.subject, t.status, t.priority "
					"FROM tasks t "
					"JOIN task_lists tl ON t.id = tl.task_id "
					"WHERE tl.list_name = ? "
					"ORDER BY t.priority DESC, t.id;";
	
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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
		int status = sqlite3_column_int(stmt, 5);
		int priority = sqlite3_column_int(stmt, 6);
		
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
	
	const char* sql = "SELECT t.id, t.name, t.description, t.location, t.subject, t.status, t.priority, tl.list_name "
					"FROM tasks t "
					"JOIN task_lists tl ON t.id = tl.task_id "
					"ORDER BY tl.list_name, t.priority DESC, t.id;";
	
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
		const char* listName = (const char*)sqlite3_column_text(stmt, 7);
		
		if (currentList != listName) {
			currentList = listName;
			cout << "========== " << listName << " TASKS ==========" << endl;
		}
		
		int id = sqlite3_column_int(stmt, 0);
		const char* name = (const char*)sqlite3_column_text(stmt, 1);
		const char* description = (const char*)sqlite3_column_text(stmt, 2);
		const char* location = (const char*)sqlite3_column_text(stmt, 3);
		const char* subject = (const char*)sqlite3_column_text(stmt, 4);
		int status = sqlite3_column_int(stmt, 5);
		int priority = sqlite3_column_int(stmt, 6);
		
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