#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> // For sort
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

void output(int test, const vector<Schedule>& chart) {
    cout << test << endl;
    for (const auto& block : chart) {
        cout << block.time << " " << block.num << " " << block.duration;
        if (block.finished)
            cout << "X";
        cout << endl;
    }
}

int main() {
    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int x;
        int quantum;
        string algo;
        cin >> x >> algo;

        if (algo == "RR") {
            cin >> quantum;
        }
 
        vector<Process> processes;
        for (int i = 1; i <= x; i++) {
            
            int a;
            int b; 
            int p;
            
            cin >> a >> b >> p;
            processes.emplace_back(i, a, b, p);
        }

        if (algo == "RR") {
            vector<Schedule> chart = round_robin(processes, quantum);
            output(t, chart);
        } else if (algo == "FCFS") {
            vector<Schedule> chart = fcfs(processes);
            output(T, chart);
        } else {
            cout << "Algorithm " << algo << " does not exist." << endl;
        }
    }

    return 0;
}
