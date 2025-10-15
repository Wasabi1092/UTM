#ifndef TASK_HPP
#define TASK_HPP

#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct tm datetime;

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
    	std::string name;
    	time_t start;
    	time_t end;
    	string location;
        string description;
        string subject;

    	// adding status and priority levels
    	int status; //0 - 100
    	int priority; // 1-5s

    public:
    	Task(int num)
    	{
    		id = num;
    		name = "Task " + std::to_string(id);
    		time(&start);
    		time(&end);
    		location = "";
    		description = "";
            subject = "";
    		// adding status and priority by default
    		status = 0;
    		priority = Priority::medium;
    	}

    	int getId() const { return id; }
    	void setId(int newId) { id = newId; }
    	void setName(const std::string& newName) { name = newName; }
    	void setDescription(const std::string& newDesc) { description = newDesc; }
    	void setLocation(const std::string& newLoc) { location = newLoc; }
    	void setSubject(const std::string& newSub) { subject = newSub; }
    	void setStart(time_t newStart) { start = newStart; }
    	void setEnd(time_t newEnd) { end = newEnd; }

    	json toJSON() const {
    		json j;
    		j["id"] = id;
    		j["name"] = name;
    		j["description"] = description;
    		j["location"] = location;
    		j["subject"] = subject;
    		j["start"] = start;
    		j["end"] = end;
    		j["status"] = static_cast<int>(status);
    		j["priority"] = static_cast<int>(priority);
    		return j;
    	}
        string getSubject() { return subject; }
        string getLocation() { return location; }
        string getDescription() { return description; }
        string getName() { return name; }
        string getStart() {
            datetime = *localtime(&start);
            string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon+1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += to_string(datetime.tm_min);

            return result;
        }
        string getEnd() {
            datetime = *localtime(&end);
            string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon+1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += to_string(datetime.tm_min);

            return result;
        }

    	int getStatus() const { return status; }
    	Priority getPriority() const { return priority; }
    	void setStatus(int s) { status = s; }
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
            int max = 30;
            int filled = floor(status/100 * max);
            int unfilled = 30-filled;
    		cout
                << "Task ID: " << id << "\n-----------"
                << "Name: " << name << "\n"
                << "Status: \n";
            for (int i=0; i<30; i++)
            {
                if (i < filled)
                {
                    cout << "█";
                }
                else
                {
                    cout << "▒";
                }
            }
            cout << "\n"
                << "Priority: " << priorityString(priority) << "\n\n";
    	}
};

#endif
