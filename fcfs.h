#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // For sort

using namespace std;

vector<Schedule> fcfs(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival); 
    vector<Schedule> chart;
    int current_time = 0;

    for (auto& process : processes) {
        if (current_time < process.arrival) {
            current_time = process.arrival;
        }
        chart.push_back(Schedule(current_time, process.index, process.burst, true));
        current_time += process.burst;
    }

    return chart;
}