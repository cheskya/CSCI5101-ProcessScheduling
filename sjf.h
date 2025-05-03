#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>

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
  vector<Process> processQueue;

  // go through each point in time
  while (true) {
    // if resulting array has all processes, break the loop
    if ((int) result.size() == processNum) {
      break;
    }

    // check processes available at the current time
    // if no more processes available, skip this step
    if ((int) processes.size() > 0) {
      for (int i = 0; i < (int) processes.size(); i++) {
        int arrival = processes[i].arrival;
        if (arrival <= currentTime) {
          processQueue.push_back(processes[i]);
          Process temp = processes[i];
          processes[i] = processes.back();
          processes.back() = temp;
          processes.pop_back();
          i--;
        }
      }
    }

    // check if there are any processes in queue
    // if no processes available, skip to next point in time
    if ((int) processQueue.size() <= 0) {
      currentTime += 1;
      continue;
    }

    int shortestBurst = INT_MAX;
    int shortestBurstIndex;

    Process* nextProcess;

    // find the process in queue with the smallest burst
    for (int i = 0; i < (int) processQueue.size(); i++) {
      int burst = processQueue[i].burst;
      if (burst <= shortestBurst) {
        shortestBurst = burst;
        shortestBurstIndex = i;
        nextProcess = &processQueue[i];
      }
    }

    // add next process to result, then remove from queue
    result.push_back(Schedule(currentTime, nextProcess->index, nextProcess->burst, true));
    currentTime += nextProcess->burst;

    Process temp = *nextProcess;
    processQueue[shortestBurstIndex] = processQueue.back();
    processQueue.back() = temp;
    processQueue.pop_back();
  }

  return result;
}