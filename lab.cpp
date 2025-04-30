#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> getArgs(string argLine);
vector<vector<string>> useFCFS(vector<vector<string>> processes);
vector<vector<string>> useSJF(vector<vector<string>> processes);
vector<vector<string>> useSRTF(vector<vector<string>> processes);
vector<vector<string>> usePriority(vector<vector<string>> processes);
vector<vector<string>> useRoundRobin(vector<vector<string>> processes, int timeQuantum);

int main(int argc, char* argv[]) {
  string programLine;
  vector<string> programArgs;

  getline(cin, programLine);
  programArgs = getArgs(programLine);

  int caseNum = stoi(programArgs.at(0));

  if (caseNum == 0) {
    printf("No arguments! Exiting...\n");
    exit(1);
  }

  // PROCESSING of each test case
  for (int i = 0; i < caseNum; i++) {
    string caseLine;
    vector<string> caseArgs;
    vector<vector<string>> initProcesses, finalProcesses;
    
    string caseType;
    int timeQuantum;

    getline(cin, caseLine);
    caseArgs = getArgs(caseLine);

    int processNum = stoi(caseArgs.at(0));
    caseType = caseArgs.at(1);
    if (caseType == "RR") {
      timeQuantum = stoi(caseArgs.at(2));
    }

    if (processNum == 0) {
      printf("No processes! Exiting...\n");
      exit(1);
    }
    
    // PROCESSING of each process
    for (int j; j < processNum; j++) {
      string processLine;
      vector<string> processArgs;

      int arrivalTime, burstTime, priority;

      getline(cin, processLine);
      processArgs = getArgs(processLine);

      initProcesses.push_back(processArgs);
    }

    // calculation here
    // insert function (will fix later, just change accrdg to what you're doing)
    finalProcesses = useFCFS(initProcesses);
  }

  return 0;
}

// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<string> getArgs(string argLine) {
  size_t startPos = 0, endPos;
  string delimiter = " ";
  string arg;
  vector<string> args;

  while((endPos = argLine.find(delimiter, startPos)) != string::npos) {
    arg = argLine.substr(startPos, (endPos - startPos));
    startPos = endPos + 1;
    args.push_back(arg);    
  }

  args.push_back(argLine.substr(startPos));
  return args;
}

// put scheduling algorithm functions here