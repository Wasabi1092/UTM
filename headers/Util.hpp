#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Task.hpp"

using namespace std;

using json = nlohmann::json;

namespace util {

    // read file
    Task readTask(int id) {
        string path = "tasks/" + to_string(id) + ".json";
        ifstream f(path);
        json j = json::parse(f);
        
        Task task;
        task.setID(j["id"]);
        task.setName(j["name"]);
        task.setStatus(j["status"]);
        task.setPriority(j["priority"]);
        return task;
    }

    bool writeTask(Task task) {
        string path = "tasks/" + to_string(task.getID()) + ".json";
        ofstream f(path);
        json j = task.toJSON();
        f << j.dump();
        f.close();
        return true;
    }



} 