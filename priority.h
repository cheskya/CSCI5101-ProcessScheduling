#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // For sort

using namespace std;

vector<Schedule> priority(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);
    deque<Process> ready_queue;
    vector<Schedule> chart;
    vector<Process> remaining_processes = processes;

    int time_elapsed = 0;
    int last_pid = -1; // last running process
    int block_start = 0; // for preemption
    int block_duration = 0;
    bool block_finished = false;

    while (!ready_queue.empty() || !remaining_processes.empty()) {
        // iterate over remaining processes to add to the ready queue
        for (auto p = remaining_processes.begin(); p != remaining_processes.end();) {  // using iterators to erase elements while looping without invalidating the loop
            if (p->arrival <= time_elapsed && !p->in_ready_queue) {
                ready_queue.push_back(*p);
                p->in_ready_queue = true;
                p = remaining_processes.erase(p);
            } else {
                ++p;  // skip the process if it hasn't arrived yet or is already in the queue 
            }
        }

        // CPU is idle
        if (ready_queue.empty()) {
            time_elapsed++;
            continue;
        }

        // sort ready queue: higher number = higher priority
        sort(ready_queue.begin(), ready_queue.end(), comparePriority);
        Process& current_process = ready_queue.front();

        // first_response time
        if (current_process.first_response == -1) {
            current_process.first_response = time_elapsed;
        }

        // in the case of a context switch: save the previous block
        if (current_process.index != last_pid && last_pid != -1) {
            chart.push_back(Schedule(block_start, last_pid, block_duration, block_finished));
            block_start = time_elapsed;
            block_duration = 0;
            block_finished = false;
        }

        // run current process for one unit
        current_process.remaining--;
        block_duration++;
        last_pid = current_process.index;

        // if process finishes, update stats and remove it from ready queue
        if (current_process.remaining == 0) {
            Process completed_process = current_process;
            completed_process.termination = time_elapsed + 1;
            completed_process.turnaround = completed_process.termination - completed_process.arrival;
            completed_process.waiting = completed_process.turnaround - completed_process.burst;
            completed_process.response_time = completed_process.first_response - completed_process.arrival;

            block_finished = true;
            ready_queue.pop_front();  // remove completed process from ready queue

            // update the processes vector with the completed process stats
            for (auto& p : processes) {
                if (p.index == completed_process.index) {
                    p = completed_process;
                    break;
                }
            }
        }

        time_elapsed++;
    }

    // add the last block to the chart
    if (block_duration > 0 && last_pid != -1) {
        chart.push_back(Schedule(block_start, last_pid, block_duration, block_finished));
    }

    return chart;
}
