#ifndef __LISTTASKS_H__
#define __LISTTASKS_H__

#include <vector>

#include "Task.hpp"

class ListTasks {
 private:
  std::vector<Task> tasks;

 public:
  //  list all task, grouped by status (pending / completed)
  void listAllTasks() const;
};


#endif  // __LISTTASKS_H__