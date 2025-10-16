#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

// manages subject data (name & color) in SQLite

namespace config
{
	// helper to convert string to lowercase (check for both name and color)
	std::string toLower(const std::string &str) {
		std::string res = "";
		for (char c : str) {
			res.push_back(tolower(c));
		}
		return res;
	}

	// helper to check if a subject already exists in the database
	bool subjectExists(sqlite3 *db, const std::string &name) {
		std::string sub = toLower(name);
		const char *sql = "SELECT name FROM subjects;";
		sqlite3_stmt *stmt;
		bool exists = false;

		// -1 means use the full length of sql string, &stmt is where the prepared statment stored
		if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
			// runs the statement and steps through each row in the result set
			// step moves to the next row till SQLITE_DONE
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				// get first column which returns pointer to a C-style string
				std::string existingName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
				if (toLower(existingName) == sub) {
					exists = true;
					break;
				}
			}
		}

		// cleans up and frees memory used by the prepared statement
		sqlite3_finalize(stmt);
		return exists;
	}
	std::string getColour(sqlite3 *db, std::string subject) {
	    std::string sql = "SELECT color FROM subjects where name=?";
		sqlite3_stmt *stmt;
		int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
		std::string res;
		if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return "default";
		}
		sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_step(stmt);

		if (rc != SQLITE_ROW) {
			std::cerr << "Failed to grab color: " << sqlite3_errmsg(db) << std::endl;
			return "default";
		}
		res = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		sqlite3_finalize(stmt);
		return res;
	}
	// add subject & color
	void addSubject(sqlite3 *db, const std::string &name, const std::string &color) {
		// input validation
		if (name.empty() || color.empty())
		{
			std::cout << "⚠️  Subject name and color cannot be empty.\n";
			return;
		}

		// check for duplicates using subjectExists
		if (subjectExists(db, name))
		{
			std::cout << "⚠️  Subject '" << name << "' already exists.\n";
			return;
		}

		// insert subject
		const char *sql = "INSERT INTO subjects (name, color) VALUES (?,?);";
		sqlite3_stmt *stmt;

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
		{
			std::cerr << "⚠️  Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
			return;
		}

		// bind text
		sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, color.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(stmt) != SQLITE_DONE)
		{
			std::cerr << "⚠️  Failed to add subject: " << sqlite3_errmsg(db) << std::endl;
		}
		else
		{
			std::cout << "✅ Added subject: " << name << " (" << color << ")\n";
		}

		sqlite3_finalize(stmt);
	}

	// edit color
	void editSubjectColor(sqlite3 *db, const std::string &name, const std::string &newColor) {
		if (!subjectExists(db, name)) {
			std::cout << "⚠️  Subject '" << name << "' does not exists.\n";
			return;
		}

		// update table (subject's colour)
		const char *sql = "UPDATE subjects SET color = ? WHERE LOWER(name) = LOWER(?);";
		sqlite3_stmt *stmt;

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
			std::cerr << "⚠️  Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
			return;
		}

		// bind text to ? in the query
		sqlite3_bind_text(stmt, 1, newColor.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			std::cerr << "⚠️  Failed to update subject color: " << sqlite3_errmsg(db) << std::endl;
		}
		else {
			std::cout << "🎨 Updated: '" << name << "' to colour: " << newColor << "\n";
		}

		sqlite3_finalize(stmt);
	}
};

#endif
