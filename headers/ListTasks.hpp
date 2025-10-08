#ifndef __LISTTASKS_H__
#define __LISTTASKS_H__

#include <algorithm>
#include <vector>

#include "Task.hpp"

class ListTasks
{
private:

	//	static helper function to compare priority
	static bool comparePriority(const Task &a, const Task &b)
	{
		return (static_cast<int>(a.getPriority()) >
				static_cast<int>(b.getPriority()));
	}

public:
	//  temporary add task function for testing only
	void addTasks(const Task &task) { tasks.push_back(task); }

	//  list all task, grouped by status (pending / completed), ordered by priority levels
	void listAllTasks() const
	{
		// vector for pending and completed tasks
		std::vector<Task> pendingTasks, completedTasks;

		// separate tasks to two different groups by status
		for (auto t : tasks)
		{
			if (t.getStatus() == Status::pending)
			{
				pendingTasks.push_back(t);
			}
			else if (t.getStatus() == Status::completed)
			{
				completedTasks.push_back(t);
			}
		}

		if (!pendingTasks.empty())
		{
			// sort pending tasks by priority levels
			std::sort(pendingTasks.begin(), pendingTasks.end(), comparePriority);

			// print pending tasks
			std::cout << "========== PENDING TASKS =========\n";
			for (auto p : pendingTasks)
			{
				// print tasks (helper function)
				p.printTask();
			}
		}
		else
		{
			std::cout << "No pending tasks.\n";
		}

		// same for completed tasks
		if (!completedTasks.empty())
		{
			// sort completed tasks by priority levels
			std::sort(completedTasks.begin(), completedTasks.end(), comparePriority);

			// print completed tasks
			std::cout << "========== COMPLETED TASKS =========\n";
			for (auto c : completedTasks)
			{
				// print tasks (helper function)
				c.printTask();
			}
		}
		else
		{
			std::cout << "No completed tasks.\n";
		}
	}
};

#endif // __LISTTASKS_H__