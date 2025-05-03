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