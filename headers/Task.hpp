#ifndef TASK_HPP
#define TASK_HPP

#include <ctime>
#include <iostream>

using namespace std;

struct tm datetime;

// define status of a task
enum struct Status
{
	pending,
	completed
};

// define priority levels
enum struct Priority
{
	low,
	medium,
	high
};

class Task
{
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
	Task()
	{
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

	Task(string name)

		//   getters and setters for status and priority
		Status getStatus() const
	{
		return status;
	}
	Priority getPriority() const { return priority; }
	void setStatus(Status s) { status = s; }
	void setPriority(Priority p) { priority = p; }

	//   print task
	void printTask()
	{
		cout << name
	}

	bool edit(int id, std::string flag, std::string value);
};

int Task::next = 0;

#endif
