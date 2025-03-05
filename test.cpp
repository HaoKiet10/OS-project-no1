#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

struct Process {
    int arrival_time;
    vector<string> bursts;
};

void fcfs_scheduling(vector<Process> &processes) {
    string cpu_timeline = "", r1_timeline = "", r2_timeline = "";
    queue<Process> ready_queue;
    queue<pair<int, int>> r1_queue, r2_queue; // (process_id, burst_time)
    
    int time = 0, process_index = 0, running_process = -1, cpu_burst_time = 0;
    int r1_running = -1, r1_burst_time = 0;
    int r2_running = -1, r2_burst_time = 0;
    
    while (process_index < processes.size() || !ready_queue.empty() || running_process != -1 || !r1_queue.empty() || r1_running != -1 || !r2_queue.empty() || r2_running != -1) {
        // Thêm process vào hàng đợi ready nếu tới thời gian arrival
        while (process_index < processes.size() && processes[process_index].arrival_time == time) {
            ready_queue.push(processes[process_index]);
            process_index++;
        }
        
        // Xử lý CPU burst
        if (running_process == -1 && !ready_queue.empty()) {
            running_process = process_index - ready_queue.size() + 1;
            auto process = ready_queue.front();
            ready_queue.pop();
            cpu_burst_time = stoi(process.bursts[0]);
            process.bursts.erase(process.bursts.begin());
            processes[running_process - 1] = process;
        }
        
        // Xử lý R1 burst
        if (r1_running == -1 && !r1_queue.empty()) {
            r1_running = r1_queue.front().first;
            r1_burst_time = r1_queue.front().second;
            r1_queue.pop();
        }
        
        // Xử lý R2 burst
        if (r2_running == -1 && !r2_queue.empty()) {
            r2_running = r2_queue.front().first;
            r2_burst_time = r2_queue.front().second;
            r2_queue.pop();
        }
        
        // Ghi nhận trạng thái CPU
        if (running_process != -1) {
            cpu_timeline += to_string(running_process);
            cpu_burst_time--;
            if (cpu_burst_time == 0) {
                if (!processes[running_process - 1].bursts.empty()) {
                    string next_burst = processes[running_process - 1].bursts[0];
                    processes[running_process - 1].bursts.erase(processes[running_process - 1].bursts.begin());
                    if (next_burst.find("(R1)") != string::npos) {
                        r1_queue.push({running_process, stoi(next_burst.substr(0, next_burst.find("(R1)")))});
                    } else if (next_burst.find("(R2)") != string::npos) {
                        r2_queue.push({running_process, stoi(next_burst.substr(0, next_burst.find("(R2)")))});
                    }
                }
                running_process = -1;
            }
        } else {
            cpu_timeline += " ";
        }
        
        // Ghi nhận trạng thái R1
        if (r1_running != -1) {
            r1_timeline += to_string(r1_running);
            r1_burst_time--;
            if (r1_burst_time == 0) {
                ready_queue.push(processes[r1_running - 1]);
                r1_running = -1;
            }
        } else {
            r1_timeline += " ";
        }
        
        // Ghi nhận trạng thái R2
        if (r2_running != -1) {
            r2_timeline += to_string(r2_running);
            r2_burst_time--;
            if (r2_burst_time == 0) {
                ready_queue.push(processes[r2_running - 1]);
                r2_running = -1;
            }
        } else {
            r2_timeline += " ";
        }
        
        time++;
    }
    
    cout << cpu_timeline << endl;
    cout << r1_timeline << endl;
    cout << r2_timeline << endl;
}

int main() {
    vector<Process> processes = {
        {0, {"5", "3(R1)", "4"}},
        {2, {"3", "2(R2)"}}
    };
    
    fcfs_scheduling(processes);
    return 0;
}
