#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

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

    	// getters and setters for status and priority
    	Status getStatus() const { return status; }
    	Priority getPriority() const { return priority; }
    	void setStatus(Status s) { status = s; }
    	void setPriority(Priority p) { priority = p; }
    	std::string priorityString(Priority p)
    	{
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
        void openFile()
        {
            string cmd = "nano ~/.cache/foo.txt";
            int res = system(cmd.c_str());
            if (res != 0)
            {
                cout << "An error occurred trying to open the text editor" << endl;
                return;
            }
        }
        int edit(int id, string flag)
        {
            openFile();
            ifstream file("~/.cache/foo.txt");
            string input;
            string result = "";
            std::cout << file.is_open() << std::endl;
            while (getline(file, input))
            {
                cout << input << endl;
                result += input;
            }
            cout << result << endl;
            file.close();
            return 0;
        }
};

int Task::next = 0;

#endif
