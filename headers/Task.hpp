#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <ctime>

struct tm datetime;

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
        std::string subject;
    public:
        Task() {
            next ++;
            id = next;
            name = "Task " + std::to_string(id);
            time(&start);
            time(&end);
            location = "";
            description = "";
            subject = "Default";
        }
        bool edit(int id, std::string flag, std::string value) {
            
        };
};

int Task::next = 0;

#endif