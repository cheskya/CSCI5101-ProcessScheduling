#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <limits.h>
#include <thread>
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

    // find the process with the shortest remaining burst at the current time
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

    int futureArrival = currentTime + currentProcess->burst;

    // find all processes that may interrupt the current process
    for (int i = 0; i < (int) processQueue.size(); i++) {
      int remaining = processQueue.at(i)->remaining;
      int index = processQueue.at(i)->index;
      if (index != currentProcess->index && remaining != 0) {
        processInterruptQueue.push_back(processQueue.at(i));
      }
    }

    int processTime;

    // check next processses
    while (true) {
      // there are next processes that can interrupt
      if (!processInterruptQueue.empty()) {
        int closestArrival = INT_MAX;
        Process* nextProcess = processInterruptQueue.front();

        for (int i = 0; i < (int) processInterruptQueue.size(); i++) {
          int arrival = processQueue.at(i)->arrival;
          if (arrival <= closestArrival) {
            closestArrival = arrival;
            Process* temp = processInterruptQueue.at(i);
            processInterruptQueue.at(i) = processInterruptQueue.front();
            processInterruptQueue.front() = temp;
          }
        }

        int nextArrival = nextProcess->arrival;
        if (nextProcess->arrival <= currentTime) {
          nextArrival = currentTime;
        }
        int futureBurst = currentProcess->remaining - (nextArrival - currentTime);
        processTime = currentProcess->remaining - futureBurst;

        // next process will interrupt
        if (nextProcess->remaining < futureBurst) {
          processTime = nextArrival - currentTime;
          timeToAdd = processTime;
          currentProcess->remaining = futureBurst;
          processQueue.pop_front();
          processQueue.push_back(currentProcess);
          processInterruptQueue.clear();
          break;
        }
        // next process won't interrupt
        else {
          processInterruptQueue.pop_front();
        }
      }
      // there are no next processes that can interrupt
      else {
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
    chart.push_back(Schedule(currentTime, currentProcess->index, processTime, isFinished));
    currentTime += timeToAdd;
  }

  return chart;
}