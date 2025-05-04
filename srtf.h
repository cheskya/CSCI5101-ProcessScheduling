#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <limits.h>
#include <cmath>

using namespace std;

vector<Schedule> srtf(vector<Process>& processes) {
  sort(processes.begin(), processes.end(), compareArrival);

  vector<Schedule> chart;
  deque<Process*> processQueue;
  deque<Process*> processInterruptQueue;

  int currentTime = 0;
  int timeToAdd = 0;
  int processCompleted = 0;
  int processNum = (int) processes.size();

  for (int i = 0; i < processNum; i++) {
    processes.at(i).remaining = processes.at(i).burst;
    processQueue.push_back(&processes.at(i));
  }

  while (true) {
    if (processCompleted == processNum) {
      break;
    }

    int shortestBurst = INT_MAX;
    bool hasProcess = false;

    for (int i = 0; i < (int) processQueue.size(); i++) {
      int burst = processQueue.at(i)->remaining;
      int arrival = processQueue.at(i)->arrival;
      if (arrival <= currentTime) {
        if (burst < shortestBurst) {
          shortestBurst = burst;
          Process* temp = processQueue.at(i);
          processQueue.at(i) = processQueue.front();
          processQueue.front() = temp;
          hasProcess = true;
        }
      }
    }

    if (hasProcess == false) {
      currentTime += 1;
      continue;
    }

    Process* currentProcess = processQueue.front();

    int futureArrival = currentTime + currentProcess->remaining;

    for (int i = 0; i < (int) processQueue.size(); i++) {
      int remaining = processQueue.at(i)->remaining;
      int index = processQueue.at(i)->index;
      if (index != currentProcess->index && remaining != 0) {
        processInterruptQueue.push_back(processQueue.at(i));
      }
    }

    int processTime;
    bool hasJustStarted = false;

    while (true) {
      if (!processInterruptQueue.empty()) {
        sort(processInterruptQueue.begin(), processInterruptQueue.end(), [](Process* a, Process* b) { return a->arrival < b->arrival; });
        Process* nextProcess = processInterruptQueue.front();
        
        int nextArrival = nextProcess->arrival;
        if (nextProcess->arrival <= currentTime) {
          nextArrival = currentTime;
        }
        int futureBurst = currentProcess->remaining - (nextArrival - currentTime);
        processTime = currentProcess->remaining - futureBurst;

        if (nextProcess->remaining < futureBurst) {
          hasJustStarted = (currentProcess->remaining == currentProcess->burst) ? true : false;
          processTime = nextArrival - currentTime;
          timeToAdd = processTime;
          currentProcess->remaining = futureBurst;
          processQueue.pop_front();
          processQueue.push_back(currentProcess);
          processInterruptQueue.clear();
          break;
        }
        else {
          processInterruptQueue.pop_front();
        }
      }
      else {
        hasJustStarted = (currentProcess->remaining == currentProcess->burst) ? true : false;
        processCompleted += 1;
        processTime = currentProcess->remaining;
        timeToAdd = processTime;
        currentProcess->remaining = 0;
        processQueue.pop_front();
        processInterruptQueue.clear();
        break;
      }
    }

    bool isFinished = (currentProcess->remaining <= 0) ? true : false;
    if (hasJustStarted) {
      currentProcess->first_response = currentTime;
    }
    if (isFinished) {
      currentProcess->termination = currentTime + timeToAdd;
      currentProcess->turnaround = currentProcess->termination - currentProcess->arrival; // compute turnaround
      currentProcess->waiting = currentProcess->turnaround - currentProcess->burst; // compute waiting
      currentProcess->response_time = currentProcess->first_response - currentProcess->arrival; // compute response
    }

    chart.push_back(Schedule(currentTime, currentProcess->index, processTime, isFinished));
    currentTime += timeToAdd;
  }

  return chart;
}