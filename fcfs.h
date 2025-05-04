#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // For sort

using namespace std;

vector<Schedule> fcfs(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival); 
    vector<Schedule> chart;
    int time_elapsed = 0;

    for (auto& process : processes) {
        if (time_elapsed < process.arrival) {
            time_elapsed = process.arrival;
        }

        process.first_response = time_elapsed;
        process.termination = time_elapsed + process.burst;
        process.waiting = process.termination - process.arrival - process.burst;
        process.turnaround = process.termination - process.arrival;
        process.response_time = process.first_response - process.arrival;
        
        chart.push_back(Schedule(time_elapsed, process.index, process.burst, true));
        time_elapsed += process.burst;
    }

    return chart;
}