#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <iomanip>

#include <limits.h>

#include "structs.h"
#include "utilfuncs.h"
#include "fcfs.h"
#include "roundrobin.h"
#include "sjf.h"
#include "srtf.h"

using namespace std;

int main(int argc, char* argv[]) {
  int caseNum;
  cin >> caseNum;

  for (int t = 1; t <= caseNum; t++) {
    int testNum;
    int quantum;
    string algo;

    cin >> testNum >> algo;

    if (algo == "RR") {
        cin >> quantum;
    }

    vector<Process> processes;

    for (int i = 1; i <= testNum; i++) {
      int arrival;
      int burst; 
      int priority;
      
      cin >> arrival >> burst >> priority;
      processes.emplace_back(i, arrival, burst, priority);
    }

    if (algo == "RR") {
      vector<Schedule> chart = roundRobin(processes, quantum);
      output(t, chart);
      // printcriteria(processes, chart);
    } else if (algo == "FCFS") {
      vector<Schedule> chart = fcfs(processes);
      output(t, chart);
      printcriteria(processes, chart);
    } else if (algo == "SJF") {
      vector<Schedule> chart = sjf(processes);
      output(t, chart);
    } else if (algo == "SRTF") {
      vector<Schedule> chart = srtf(processes);
      output(t, chart);
      printcriteria(processes, chart);
    } else {
      cout << "Algorithm " << algo << " does not exist." << endl;
    }
  }

  return 0;
}