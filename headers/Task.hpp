#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

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
    	std::string name;
    	time_t start;
    	time_t end;
    	string location;
        string description;
        string subject;

    	// adding status and priority levels
    	Status status;
    	Priority priority;

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
    		status = Status::pending;
    		priority = Priority::medium;
    	}

        string getLocation()
        {
            return location;
        }
        string getDescription()
        {
            return description;
        }
        string getName()
        {
            return name;
        }

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
        void openFile(string flag)
        {
            string exist;
            if (flag == "--name")
            {
                exist = name;
            }
            else if (flag == "--desc")
            {
                exist = description;
            }
            else if (flag == "--loc")
            {
                exist = location;
            }
            else if (flag == "--sub")
            {
                exist = subject;
            }
            else if (flag == "--start-time")
            {
                datetime = *localtime(&start);
                exist = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon) + "/" + to_string(datetime.tm_year) + " " + to_string(datetime.tm_hour) + ":" + to_string(datetime.tm_min);
            }
            else if (flag == "--end-time")
            {
                datetime = *localtime(&start);
                exist = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon) + "/" + to_string(datetime.tm_year) + " " + to_string(datetime.tm_hour) + ":" + to_string(datetime.tm_min);
            }
            else
            {
                cout << "Invalid flag: " << flag << endl;
            }
            string cmd = "touch ~/.cache/foo.txt";
            int res = system(cmd.c_str());
            if (res != 0)
            {
                cout << "An error occurred when creating the file" << endl;
            }


            string home = getenv("HOME");
            ofstream file( home + "/.cache/foo.txt");
            file << exist;
            file.close();
            cmd = "nano ~/.cache/foo.txt";
            res = system(cmd.c_str());
            if (res != 0)
            {
                cout << "An error occurred trying to open the text editor" << endl;
                return;
            }
        }
        time_t parseTime(string result)
        {
            time_t time;

            // expected format is dd/mm/yyyy hh:mm
            int index = result.find(" ");
            string date = result.substr(0, index);
            string time_string = result.substr(index +1, result.length()-index);

            datetime.tm_mday = stoi(date.substr(0, 2));
            datetime.tm_mon = stoi(date.substr(3, 2));
            datetime.tm_year = stoi(date.substr(6, 4));

            datetime.tm_hour = stoi(time_string.substr(0, 2));
            datetime.tm_min = stoi(time_string.substr(3, 2));

            datetime.tm_isdst = -1;

            time = mktime(&datetime);
            return time;
        }
        int edit(int id, string flag)
        {
            openFile(flag);
            string home = getenv("HOME");
            ifstream file( home + "/.cache/foo.txt");
            string input;
            string result = "";
            while (getline(file, input))
            {
                result += input + "\n";
            }
            result = result.substr(0, result.length()-1);
            if (flag == "--name")
            {
                name = result;
            }
            else if (flag == "--desc")
            {
                description = result;
            }
            else if (flag == "--loc")
            {
                location = result;
            }
            else if (flag == "--sub")
            {
                subject = result;
            }
            else if (flag == "--start-time")
            {
                start = parseTime(result);
            }
            else if (flag == "--end-time")
            {
                end = parseTime(result);
            }
            else
            {
                cout << "Invalid flag: " << flag << endl;
            }
            file.close();
            return 0;
        }
};

#endif
