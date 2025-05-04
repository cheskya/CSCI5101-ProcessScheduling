using namespace std;

void output(int test, const vector<Schedule>& chart) {
    cout << test << endl;
    for (const auto& block : chart) {
        cout << block.time << " " << block.num << " " << block.duration;
        if (block.finished)
            cout << "X";
        cout << endl;
    }
}

void printcriteria(const vector<Process>& processes, const vector<Schedule>& chart) {
    int active_time = 0, total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0; //cumulative

    int first_arrival = INT_MAX;
    int last_termination = 0;

    for (auto& process : processes) {
        active_time += process.burst;
        total_waiting_time += process.waiting;
        total_turnaround_time += process.turnaround;
        total_response_time += process.response_time;

        if (process.arrival < first_arrival)               //for CPU utilisation
            first_arrival = process.arrival;
        if (process.termination > last_termination)
            last_termination = process.termination;
    }
    double total_time = last_termination - first_arrival;
    double cpu_utilization = (double) active_time / total_time * 100;
    double throughput = (double) processes.size() / total_time;
    double avg_waiting_time = (double) total_waiting_time / processes.size();
    double avg_turnaround_time = (double) total_turnaround_time / processes.size();
    double avg_response_time = (double) total_response_time / processes.size();

    cout << setprecision(0) << fixed;
    cout << "CPU Utilization: " << cpu_utilization << "%" << endl;
    cout << setprecision(2) << fixed;
    cout << "Throughput: " << throughput << " processes/second" << endl;
    for (auto& process : processes) {
        cout << "Process " << process.index << ": " << process.waiting << "ns" <<endl;
    }
    cout << "Average waiting time = " << avg_waiting_time << "ns" << endl;
    for (auto& process : processes) {
        cout << "Process " << process.index << ": " << process.turnaround << "ns" <<endl;
    }
    cout << "Average turnaround time = " << avg_turnaround_time << "ns" << endl;
    for (auto& process : processes) {
        cout << "Process " << process.index << ": " << process.response_time << "ns" <<endl;
    }
    cout << "Average response time = " << avg_response_time << "ns" << endl;
}

bool compareArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

bool comparePriority(const Process& a, const Process& b) {
    return a.priority > b.priority;
}