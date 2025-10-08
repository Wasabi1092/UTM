#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

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
