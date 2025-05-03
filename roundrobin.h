#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // For sort

using namespace std;

//https://stackoverflow.com/questions/49147095/sort-according-to-arrival-time-in-fcfs-scheduling-algorithm
vector<Schedule> roundRobin(vector<Process>& processes, int quantum) {
    sort(processes.begin(), processes.end(), compareArrival);

    vector<Schedule> chart;
    deque<Process*> queue;
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    vector<bool> arrived(n + 1, false);  //has alr arrived before?

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= current_time && !arrived[processes[i].index]) {
                
                bool inserted = false;
                for (auto it = queue.begin(); it != queue.end(); ++it) {
                    if (processes[i].priority > (*it)->priority) {
                        queue.insert(it, &processes[i]);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted)
                    queue.push_back(&processes[i]);
                arrived[processes[i].index] = true;
            }
        }

        //just in case there's nothing
        if (queue.empty()) {
            current_time++;
            continue;
        }

        Process* process = queue.front();
        queue.pop_front();

        int duration = min(quantum, process->remaining); // set quantum len
        int end_time = current_time + duration;
        bool finished = (process->remaining <= quantum);

        chart.push_back(Schedule(current_time, process->index, duration, finished));

        process->remaining -= duration;
        current_time = end_time;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival > (current_time - duration) &&
                processes[i].arrival <= current_time &&
                !arrived[processes[i].index]) {
                bool inserted = false;
                for (auto it = queue.begin(); it != queue.end(); ++it) {
                    if (processes[i].priority > (*it)->priority) {
                        queue.insert(it, &processes[i]);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted)
                    queue.push_back(&processes[i]);
                arrived[processes[i].index] = true;
            }
        }

        if (process->remaining > 0)
            queue.push_back(process);
        else
            completed++;
    }

    return chart;
}