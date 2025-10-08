#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;


struct tm datetime;

class Task
{
    private:
        int id;
        static int next;
        string name;
        time_t start;
        time_t end;
        string location;
        string description;
        string subject;
    public:
        Task()
        {
            next ++;
            id = next;
            name = "Task " + to_string(id);
            time(&start);
            time(&end);
            location = "";
            description = "";
            subject = "Default";
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
            string home = getenv("HOME");
            ifstream file( home + "/.cache/foo.txt");
            string input;
            string result = "";
            while (getline(file, input))
            {
                result += input + "\n";
            }
            if (flag == "--desc")
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
            file.close();
            return 0;
        }
};

int Task::next = 0;

#endif
