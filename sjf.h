#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <limits.h>

using namespace std;

/*
1. modify processes to have their indexes stored
2. go to the first point in time
3. for the current point in time, add the processes available
4. after adding processes, find the process with the shortest burst time
5. execute the process with the shortest burst time
6. go to point in time after the process (if no process executed, go to the next point in time (+1ns))
7. repeat steps 2-6
*/

vector<Schedule> sjf(vector<Process>& processes) {

  vector<Schedule> result;

  int currentTime = 0; // in ns
  int processNum = (int) processes.size();
  deque<Process*> processQueue;

  // go through each point in time
  while (true) {
    printf("Current time: %d\n", currentTime);
    // if resulting array has all processes, break the loop
    if ((int) result.size() == processNum) {
      break;
    }

    // check processes available at the current time
    for (int i = 0; i < (int) processes.size(); i++) {
      int arrival = processes.at(i).arrival;
      int remaining = processes.at(i).remaining;
      if (arrival <= currentTime && remaining > 0) {
        processQueue.push_back(&processes.at(i));
      }
    }

    // check if there are any processes in queue
    // if no processes available, skip to next point in time
    if ((int) processQueue.size() <= 0) {
      currentTime += 1;
      continue;
    }

    sort(processQueue.begin(), processQueue.end(), [](Process* a, Process* b) { return a->burst < b->burst; });
    Process* currentProcess = processQueue.front();

    printf("Processes in queue:\n");
    for (int i = 0; i < (int) processQueue.size(); i++) {
      printf("Index %d with arrival %d and remaining %d\n", processQueue.at(i)->index, processQueue.at(i)->arrival, processQueue.at(i)->remaining);
    }

    printf("Current process is index %d with arrival %d and remaining %d\n", currentProcess->index, currentProcess->arrival, currentProcess->remaining);

    // add next process to result, then remove from queue
    result.push_back(Schedule(currentTime, currentProcess->index, currentProcess->burst, true));
    currentProcess->remaining = 0;
    currentProcess->first_response = currentTime;
    currentProcess->termination = currentTime + currentProcess->burst;
    currentProcess->turnaround = currentProcess->termination - currentProcess->arrival; // compute turnaround
    currentProcess->waiting = currentProcess->turnaround - currentProcess->burst; // compute waiting
    currentProcess->response_time = currentProcess->first_response - currentProcess->arrival; // compute response
    currentTime += currentProcess->burst;
    processQueue.clear();
  }

  return result;
}