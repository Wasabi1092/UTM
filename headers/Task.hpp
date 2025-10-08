#ifndef TASK_HPP
#define TASK_HPP

#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct tm datetime;

// define status of a task
enum class Status
{
	pending,
	completed
};

// define priority levels
enum class Priority
{
	low,
	medium,
	high
};

class Task {
private:
	int id;
	static int next;
	std::string name;
	time_t start;
	time_t end;
	std::string location;
	std::string description;

	// adding status and priority levels
	Status status;
	Priority priority;

public:
	Task() {
		next++;
		id = next;
		name = "Task " + std::to_string(id);
		time(&start);
		time(&end);
		location = "";
		description = "";

		// adding status and priority by default
		status = Status::pending;
		priority = Priority::medium;
	}

	// getters and setters for status and priority
	Status getStatus() const { return status; }
	Priority getPriority() const { return priority; }
	void setStatus(Status s) { status = s; }
	void setPriority(Priority p) { priority = p; }
	std::string priorityString(Priority p) {
		switch (p)
		{
		case Priority::high:
			return "High";
		case Priority::medium:
			return "Medium";
		case Priority::low:
			return "Low";
		}
		return "Unknown";
	}

	// print task
	void printTask()
	{
		std::cout << "Task ID: " << id << "\n"
				  << "Name: " << name << "\n"
				  << "Status: "
				  << (status == Status::pending ? "Pending" : "Completed")
				  << "\n"
				  << "Priority: " << priorityString(priority) << "\n\n";
	}

	bool edit(int id, std::string flag, std::string value);



    json toJSON() {
        json j;
        j["id"] = id;
        j["name"] = name;
        j["status"] = status;
        j["priority"] = priority;
        return j;
    }











};

int Task::next = 0;

#endif