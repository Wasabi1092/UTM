#ifndef TASK_HPP
#define TASK_HPP

#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;

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
        string getSubject()
        {
            return subject;
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
        string getStart()
        {
            datetime = *localtime(&start);
            string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon+1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += to_string(datetime.tm_min);

            return result;
        }
        string getEnd()
        {
            datetime = *localtime(&end);
            string result = to_string(datetime.tm_mday) + "/" + to_string(datetime.tm_mon+1) + "/" + to_string(datetime.tm_year + 1900) + " " + to_string(datetime.tm_hour) + ":";
            if (datetime.tm_min < 10)
            {
                result += "0";
            }
            result += to_string(datetime.tm_min);

            return result;
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
    		cout << "Task ID: " << id << "\n"
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
            else if (flag == "--start")
            {
                datetime = *localtime(&start);
                exist = getStart();
            }
            else if (flag == "--end")
            {
                datetime = *localtime(&start);
                exist = getEnd();
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
            try
            {
                int index = result.find(" ");
                string date = result.substr(0, index);
                string time_string = result.substr(index +1, result.length()-index);

                index = date.find("/");
                datetime.tm_mday = stoi(date.substr(0, index));
                date = date.substr(index+1, date.length()-2);
                index = date.find("/");
                datetime.tm_mon = stoi(date.substr(0, index))-1;
                date = date.substr(index+1, date.length()-2);
                datetime.tm_year = stoi(date.substr(0, 4)) - 1900;

                index = time_string.find(":");
                datetime.tm_hour = stoi(time_string.substr(0, index));
                datetime.tm_min = stoi(time_string.substr(index+1, 2));

                datetime.tm_isdst = -1;

                time = mktime(&datetime);
                return time;
            }
            catch (...)
            {
                cout << "Invalid Time string format" << endl;
                cout << "Please ensure the format is dd/mm/yyyy hh:mm" << endl;
                cout << "You provided: " << result << endl;
                return NULL;
            }

        }
        int edit(int id, string flag)
        {
            openFile(flag);
            string home = getenv("HOME");
            ifstream file( home + "/.cache/foo.txt");
            string input;
            string result = "";
            time_t temp;
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
            else if (flag == "--start")
            {
                temp = parseTime(result);
                if (!temp)
                {
                    return 1;
                }
                start = temp;
            }
            else if (flag == "--end")
            {
                temp = parseTime(result);
                if (!temp)
                {
                    return 1;
                }
                end = temp;
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