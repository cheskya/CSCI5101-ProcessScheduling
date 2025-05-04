struct Process {
    int index;
    int arrival;
    int burst;
    int remaining;
    int priority;

    int first_response = -1; // -1 = not yet set
    int termination = 0;
    int waiting = 0;
    int turnaround = 0;
    int response_time = -1; // -1 = not yet set

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