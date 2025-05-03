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

bool compareArrival(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}
