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

vector<vector<string>> useSJF(vector<vector<string>> processes) {

  vector<vector<string>> result;

  // save index
  for (int i = 0; i < (int) processes.size(); i++) {
    processes.at(i).push_back(to_string(i+1));
  }

  int currentTime = 0; // in ns
  int processNum = (int) processes.size();
  vector<vector<string>> processQueue;

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
        int arrival = stoi(processes.at(i).at(0));
        vector<string> temp;
        if (arrival <= currentTime) {
          processQueue.push_back(processes.at(i));
          temp = processes.at(i);
          processes.at(i) = processes.back();
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

    // find the process in queue with the smallest burst
    for (int i = 0; i < (int) processQueue.size(); i++) {
      int burst = stoi(processQueue.at(i).at(1));
      if (burst <= shortestBurst) {
        shortestBurst = burst;
        shortestBurstIndex = i;
      }
    }

    // add next process to result, then remove from queue
    vector<string> resultProcess;
    resultProcess = {to_string(currentTime), processQueue.at(shortestBurstIndex).at(3), processQueue.at(shortestBurstIndex).at(1) + "X"};
    currentTime += stoi(processQueue.at(shortestBurstIndex).at(1));
    result.push_back(resultProcess);

    vector<string> temp;
    temp = processQueue.at(shortestBurstIndex);
    processQueue.at(shortestBurstIndex) = processQueue.back();
    processQueue.back() = temp;
    processQueue.pop_back();

  }

  return result;
}