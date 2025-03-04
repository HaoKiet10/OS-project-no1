#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>   // for std::setprecision
using namespace std;

// Define a structure to hold process information
struct Process {
    int id;         // Process ID (label as P1, P2, ...)
    int arrival;    // Arrival time of the process
    int burst;      // Burst time (CPU time required) for the process
    int waiting;    // Waiting time (time process waits in ready queue)
    int turnaround; // Turnaround time (time from arrival to completion)
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    
    // Check for valid number of processes
    if (n <= 0) {
        cout << "Number of processes must be positive." << endl;
        return 1;
    }
    
    // Read arrival times and burst times
    vector<int> arrival_times(n);
    vector<int> burst_times(n);
    cout << "Enter arrival times for each process: ";
    for (int i = 0; i < n; ++i) {
        cin >> arrival_times[i];
    }
    cout << "Enter burst times for each process: ";
    for (int i = 0; i < n; ++i) {
        cin >> burst_times[i];
    }
    
    // Create a list of processes with their data
    vector<Process> processes;
    processes.reserve(n);
    for (int i = 0; i < n; ++i) {
        Process p;
        p.id = i + 1;                 // process labeled as P1, P2, ... Pn
        p.arrival = arrival_times[i];
        p.burst = burst_times[i];
        p.waiting = 0;
        p.turnaround = 0;
        processes.push_back(p);
    }
    
    // Sort processes by arrival time (and by ID if arrival times are the same)
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrival == b.arrival)
            return a.id < b.id;
        return a.arrival < b.arrival;
    });
    
    // Calculate waiting and turnaround times for each process in order
    int current_time = 0;
    double total_waiting_time = 0;
    double total_turnaround_time = 0;
    
    for (Process &p : processes) {
        // If CPU is idle until this process arrives, advance current_time
        if (current_time < p.arrival) {
            current_time = p.arrival;
        }
        // Waiting time is the difference between start time and arrival time
        p.waiting = current_time - p.arrival;
        // Turnaround time is total time from arrival to completion (waiting + burst)
        p.turnaround = p.waiting + p.burst;
        // Update current time to when this process finishes executing
        current_time += p.burst;
        // Accumulate total waiting and turnaround times
        total_waiting_time += p.waiting;
        total_turnaround_time += p.turnaround;
    }
    
    // Display the results
    cout << "\nProcess execution order: ";
    for (size_t i = 0; i < processes.size(); ++i) {
        cout << "P" << processes[i].id;
        if (i < processes.size() - 1) cout << " -> ";
    }
    cout << endl;
    
    // Print table header
    cout << "Process\tArrival\tBurst\tWaiting\tTurnaround\n";
    // Print waiting and turnaround time for each process
    for (const Process &p : processes) {
        cout << "P" << p.id << "\t" << p.arrival << "\t" << p.burst 
             << "\t" << p.waiting << "\t" << p.turnaround << endl;
    }
    
    // Calculate and display average waiting and turnaround times
    cout.setf(std::ios::fixed);
    cout.precision(2);
    cout << "Average Waiting Time: " << (total_waiting_time / n) << endl;
    cout << "Average Turnaround Time: " << (total_turnaround_time / n) << endl;
    
    return 0;
}
