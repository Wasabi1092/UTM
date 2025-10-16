#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <map>

struct tm datetime;

class Task {
    private:
    	int id;
    	std::string name;
    	time_t start;
    	time_t end;
    	std::string location;
		std::string description;
		std::string subject;
        int status;   // 0 - 100
        int priority; // 1-5s

    public:
        Task(std::string name, std::string description, std::string location, std::string subject, int priority) {
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
        std::string getSubject() { return subject; }
        std::string getLocation() { return location; }
        std::string getDescription() { return description; }
        std::string getName() { return name; }
        int getStatus() const { return status; }
        int getPriority() const { return priority; }
        void setStatus(int s) { status = s; }
        void setPriority(int p) { priority = p % 5 + 1; }
        time_t getStart() { return start; }
        time_t getEnd() { return end; }
        std::string getStartString() {
            datetime = *localtime(&start);
            std::string result = std::to_string(datetime.tm_mday) + "/" + std::to_string(datetime.tm_mon + 1) + "/" + std::to_string(datetime.tm_year + 1900) + " " + std::to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += std::to_string(datetime.tm_min);

            return result;
        }
        std::string getEndString() {
            datetime = *localtime(&end);
            std::string result = std::to_string(datetime.tm_mday) + "/" + std::to_string(datetime.tm_mon + 1) + "/" + std::to_string(datetime.tm_year + 1900) + " " + std::to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += std::to_string(datetime.tm_min);

            return result;
        }
};

#endif
