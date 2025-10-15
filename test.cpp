#include <iostream>
#include "headers/Util.hpp"
// // #include <string>
// // #include <sqlite3.h>
// // #include "headers/Config.hpp"

// // using namespace std;

// // int main(int argc, char *argv[])
// // {
// //     // argc is number of args
// //     // argv are the args passed in as a vector

// //     return 0;
// // }

// #include <iostream>
// #include <sqlite3.h>
// #include "headers/Config.hpp" // include your file here

// using namespace std;

// // helper: create the subjects table
// bool createTable(sqlite3 *db)
// {
//     const char *sql =
//         "CREATE TABLE IF NOT EXISTS subjects ("
//         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//         "name TEXT UNIQUE NOT NULL, "
//         "color TEXT NOT NULL);";

//     char *errMsg = nullptr;
//     if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
//     {
//         cerr << "Error creating table: " << errMsg << endl;
//         sqlite3_free(errMsg);
//         return false;
//     }

//     return true;
// }

// // helper: print all subjects
// void showSubjects(sqlite3 *db)
// {
//     const char *sql = "SELECT id, name, color FROM subjects;";
//     sqlite3_stmt *stmt;

//     cout << "\n📘 Current Subjects:\n";
//     if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
//     {
//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             int id = sqlite3_column_int(stmt, 0);
//             string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
//             string color = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
//             cout << "  [" << id << "] " << name << " (" << color << ")\n";
//         }
//     }
//     else
//     {
//         cerr << "⚠️ Failed to fetch subjects: " << sqlite3_errmsg(db) << endl;
//     }

//     sqlite3_finalize(stmt);
// }

// int main()
// {
//     sqlite3 *db;
//     if (sqlite3_open("test.db", &db) != SQLITE_OK)
//     {
//         cerr << "Failed to open database: " << sqlite3_errmsg(db) << endl;
//         return 1;
//     }

//     if (!createTable(db))
//     {
//         cerr << "Failed to initialise table." << endl;
//         sqlite3_close(db);
//         return 1;
//     }

//     cout << "✅ Database ready.\n";

//     // test adding subjects
//     config::addSubject(db, "Math", "Blue");
//     config::addSubject(db, "Science", "Green");
//     config::addSubject(db, "Math", "Red"); // duplicate check

//     showSubjects(db);

//     // test editing color
//     config::editSubjectColor(db, "Math", "Red");
//     config::editSubjectColor(db, "Art", "Purple"); // non-existing subject

//     showSubjects(db);

//     sqlite3_close(db);
//     return 0;
// }


int main () {
    cout << util::colourText("Hello, World!", "red", "blue") << endl;
    cout << util::colourText("wooo!", "red") << endl;
    cout << util::colourText("just green!", "green", "green") << endl;
    cout << util::colourText("grrr missing arguments") << endl;
    cout << util::colourText("no hex", "green") << endl;
    cout << util::colourText("hex", "#FF0000") << endl;
    cout << util::colourText("hex", "#0FF000") << endl;
    cout << util::colourText("hex", "#00FF00") << endl;
    cout << util::colourText("hex", "#000FF0") << endl;
    cout << util::colourText("hex", "#b8007F") << endl;
    cout << util::colourText("hex", "#00FFFF") << endl;
    cout << util::colourText("hex", "#FFFFFF") << endl;
}