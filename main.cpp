#include <iostream>

#include "headers/ListTasks.hpp"
#include "headers/Task.hpp"

int main(int argc, char* argv[]) {
  // argc is number of args
  // argv are the args passed in as a vector
  ListTasks tasksList;

  Task t1;
  Task t2;
  Task t3;
  Task t4;

  t1.setPriority(Priority::high);
  t2.setPriority(Priority::medium);
  t3.setPriority(Priority::low);
  t4.setPriority(Priority::high);

  t1.setStatus(Status::completed);
  t2.setStatus(Status::pending);
  t3.setStatus(Status::pending);
  t4.setStatus(Status::completed);

  tasksList.addTasks(t1);
  tasksList.addTasks(t2);
  tasksList.addTasks(t3);
  tasksList.addTasks(t4);

  tasksList.listAllTasks();

  return 0;
}
