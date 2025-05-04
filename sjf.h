#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <limits.h>

using namespace std;

vector<Schedule> sjf(vector<Process>& processes) {
  vector<Schedule> result;

  int currentTime = 0; // in ns
  int processNum = (int) processes.size();
  deque<Process*> processQueue;

  while (true) {
    if ((int) result.size() == processNum) {
      break;
    }

    for (int i = 0; i < (int) processes.size(); i++) {
      int arrival = processes.at(i).arrival;
      int remaining = processes.at(i).remaining;
      if (arrival <= currentTime && remaining > 0) {
        processQueue.push_back(&processes.at(i));
      }
    }

    if ((int) processQueue.size() <= 0) {
      currentTime += 1;
      continue;
    }

    sort(processQueue.begin(), processQueue.end(), [](Process* a, Process* b) { return a->burst < b->burst; });
    Process* currentProcess = processQueue.front();

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