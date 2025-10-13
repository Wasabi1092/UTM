#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Task.hpp"
#include <algorithm>
#include <vector>


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



    bool comparePriority(const Task &a, const Task &b) {
		return (static_cast<int>(a.getPriority()) >
				static_cast<int>(b.getPriority()));
	}




    void listAllTasks() {
		// vector for pending and completed tasks
		std::vector<Task> pendingTasks, completedTasks;

		// separate tasks to two different groups by status
		for (auto t : tasks)
		{
			if (t.getStatus() == Status::pending)
			{
				pendingTasks.push_back(t);
			}
			else if (t.getStatus() == Status::completed)
			{
				completedTasks.push_back(t);
			}
		}

		if (!pendingTasks.empty())
		{
			// sort pending tasks by priority levels
			std::sort(pendingTasks.begin(), pendingTasks.end(), comparePriority);

			// print pending tasks
			std::cout << "========== PENDING TASKS =========\n";
			for (auto p : pendingTasks)
			{
				// print tasks (helper function)
				p.printTask();
			}
		}
		else
		{
			std::cout << "No pending tasks.\n";
		}

		// same for completed tasks
		if (!completedTasks.empty())
		{
			// sort completed tasks by priority levels
			std::sort(completedTasks.begin(), completedTasks.end(), comparePriority);

			// print completed tasks
			std::cout << "========== COMPLETED TASKS =========\n";
			for (auto c : completedTasks)
			{
				// print tasks (helper function)
				c.printTask();
			}
		}
		else
		{
			std::cout << "No completed tasks.\n";
		}
	}


} 