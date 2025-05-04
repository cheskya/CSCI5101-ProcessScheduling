#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> 

using namespace std;

vector<Schedule> roundRobin(vector<Process>& processes, int quantum) {
    sort(processes.begin(), processes.end(), compareArrival);

    vector<Schedule> chart;
    deque<Process*> queue;
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    int active_time = 0; 
    int last_termination = 0;
    vector<bool> arrived(n + 1, false);  

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= current_time && !arrived[processes[i].index]) {
                
                bool inserted = false;
                for (auto j = queue.begin(); j != queue.end(); ++j) {
                    queue.insert(j, &processes[i]);
                    inserted = true;
                    break;
                    
                }
                if (!inserted)
                    queue.push_back(&processes[i]);
                arrived[processes[i].index] = true;
            }
        }

        if (queue.empty()) {
            current_time++;
            continue;
        }

        Process* process = queue.front();
        queue.pop_front();

        if (process->first_response == -1) {
            process->first_response = current_time; 
        }

        int duration = min(quantum, process->remaining);
        int end_time = current_time + duration;
        bool finished = (process->remaining <= quantum);

        active_time += duration;

        process->turnaround = end_time - process->arrival;
        process->waiting = process->turnaround - process->burst;
        process->termination = end_time; 
        process->response_time = process->first_response - process->arrival;

        chart.push_back(Schedule(current_time, process->index, duration, finished));

        process->remaining -= duration;
        current_time = end_time;
        last_termination = max(last_termination, end_time);

        if (process->remaining > 0) {
            queue.push_back(process);
        }  else {
            completed++;
        }
    }

    return chart;
}
