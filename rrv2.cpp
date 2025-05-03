#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

struct Process {
    int index;
    int arrival;
    int burst;
    int remaining;
    int priority;

    Process(int i, int a, int b, int p) {
        index = i;
        arrival = a;
        burst = b;
        remaining = b;
        priority = p;
    }
};

struct Schedule {
    int time;
    int num;
    int duration;
    bool finished;

    Schedule(int t, int p, int d, bool f) {
        time = t;
        num = p;
        duration = d;
        finished = f;
    }
};


bool compareArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}
//https://stackoverflow.com/questions/49147095/sort-according-to-arrival-time-in-fcfs-scheduling-algorithm
vector<Schedule> round_robin(vector<Process>& processes, int quantum) {
    sort(processes.begin(), processes.end(), compareArrival); 
    vector<Schedule> chart;
    deque<Process*> queue;
    int current_time = 0; 
    int completed = 0;

    vector<bool> arrived(processes.size(), false);  //has alr arrived before?
    while (completed < processes.size()) {

        for (auto& process : processes) {
            if (process.arrival <= current_time && !arrived[process.index]) {
                queue.push_back(&process);
                arrived[process.index] = true;
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
        process->remaining -= duration;
        current_time += duration;

        chart.push_back(Schedule(current_time - duration, process->index, duration, process->remaining == 0));

        if (process->remaining > 0)
            queue.push_back(process);
        else
            completed++;

    }

    return chart;
}

void output(int test_case, const vector<Schedule>& chart) {
    cout << test_case << endl;
    for (const auto& block : chart) {
        cout << block.time << " " << block.num << " " << block.duration << (block.finished ? "X" : "") << endl;
    }
}

int main() {
    int T; // test case
    cin >> T;
    while (T--) {
        int x, quantum;
        string algo;
        cin >> x >> algo >> quantum;
        vector<Process> processes;
        
        for (int i = 1; i <= x; i++) {
            
            int a;
            int b;
            int p;

            cin >> a >> b >> p;
            processes.push_back(Process(i, a, b, p));
        }

        if (algo == "RR") {
            vector<Schedule> chart = round_robin(processes, quantum);
            output(T, chart);
        } else {
            cout << "Algorithm " << algo << " does not exist." << endl;
        }
    }
    return 0;
}
