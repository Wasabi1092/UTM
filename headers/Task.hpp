#ifndef TASK_HPP
#define TASK_HPP

#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <cmath>

using namespace std;

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
    int status;   // 0 - 100
    int priority; // 1-5s

public:
    Task(string name, string description, string location, string subject, int priority)
    {
        id = 0;
        this->name = name;
        time(&start);
        time(&end);
        this->location = location;
        this->description = description;
        this->subject = subject;
        // adding status and priority by default
        status = 0;
        this->priority = priority;
    }

    int getId() const { return id; }
    void setId(int newId) { id = newId; }
    void setName(const std::string &newName) { name = newName; }
    void setDescription(const std::string &newDesc) { description = newDesc; }
    void setLocation(const std::string &newLoc) { location = newLoc; }
    void setSubject(const std::string &newSub) { subject = newSub; }
    void setStart(time_t newStart) { start = newStart; }
    void setEnd(time_t newEnd) { end = newEnd; }
    string getSubject() { return subject; }
    string getLocation() { return location; }
    string getDescription() { return description; }
    string getName() { return name; }
    time_t getStart() { return start; }
    time_t getEnd() { return end; }
    string getStartString()
    {
        datetime = *localtime(&start);
        string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon + 1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
        if (datetime.tm_min < 10)
        {
            result += "0";
        }
        result += to_string(datetime.tm_min);

        return result;
    }
    string getEndString()
    {
        datetime = *localtime(&end);
        string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon + 1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
        if (datetime.tm_min < 10)
        {
            result += "0";
        }
        result += to_string(datetime.tm_min);

        return result;
    }

    int getStatus() const { return status; }
    int getPriority() const { return priority; }
    void setStatus(int s) { status = s; }
    void setPriority(int p) { priority = p % 5 + 1; }

    // print task
    void printTask()
    {
        int max = 30;
        int filled = std::floor((status * max) / 100);
        int unfilled = 30 - filled;
        cout
            << "Task ID: " << id << "\n-----------\n"
            << "Name: " << name << "\n"
            << "Start Time: " << getStartString() << "\n"
            << "End Time: " << getEndString() << "\n"
            << "Status: \n";
        for (int i = 0; i < 30; i++)
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
             << "Priority: " << priority << "/5\n\n";
    }
};

#endif
