#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Resource {
protected:
    string type;
    int burstTime;
public:
    Resource(string type, int burst): 
    type(type), burstTime(burst) {}

    string getType() {return type;}
    int getBurstTime() {return burstTime;}

    void setType(string type) {this->type = type;}
    void setBurstTime(int burstTime) {this->burstTime = burstTime;}
};

class Process {
private:
    int id;
    int arrivalTime;
    int waitingTime;
    int turnaroundTime;
    queue<Resource> resources;
public:
    Process():
    id(0), arrivalTime(0), waitingTime(0), turnaroundTime(0) {}
    
    int getId() {return id;}
    int getArrivalTime() {return arrivalTime;}
    int getWaitingTime() {return waitingTime;}
    int getTurnaroundTime() {return turnaroundTime;}

    void setId(int id) {this->id = id;}
    void setArrivalTime(int arrivalTime) {this->arrivalTime = arrivalTime;}
    void setWaitingTime(int waitingTime) {this->waitingTime = waitingTime;}
    void setTurnaroundTime(int turnaroundTime) {this->turnaroundTime = turnaroundTime;}
    void pushResource(Resource resource) {resources.push(resource);}

    bool isDone() {return resources.empty();}
    Resource popResource() {
        Resource resource = resources.front();
        resources.pop();
        return resource;
    }

    string test() {
        string tmp = ""; 
        queue<Resource> cloneResource(resources);
        while (!cloneResource.empty()) {
            Resource resource = cloneResource.front();
            cloneResource.pop();
            if (resource.getType() == "cpu") tmp += to_string(resource.getBurstTime()) + " ";
            if (resource.getType() == "R1") tmp += to_string(resource.getBurstTime()) + "(R1) ";
            if (resource.getType() == "R2") tmp += to_string(resource.getBurstTime()) + "(R2) ";
        }
        return tmp;
    }
};

void fcfs() {
    int numberOfPC;
    int time = 0;
    vector<Process> processes;
    queue<Process> cpuQueue;
    queue<Process> r1Queue;
    queue<Process> r2Queue;

    cout << "Enter number of processes: ";
    cin >> numberOfPC;
    bool visited[numberOfPC] = {false};

    for (int i = 0; i < numberOfPC; i++) {
        int arrivalTime;
        int option;
        int burstTime;
        Process process;
        process.setId(i + 1);

        cout << "Enter arrival time of process " << i + 1 << ": ";
        cin >> arrivalTime;
        process.setArrivalTime(arrivalTime);

        while(true) {
            cout << "Choose cpu(0), resource R1(1), resource R2(2) or Exit(-1): ";
            cin >> option;

            if (option == -1) break;
            if (option == 0) {
                cout << "Enter burst: ";
                cin >> burstTime;
                Resource cpu("cpu", burstTime);
                process.pushResource(cpu);
            }
            if (option == 1) {
                cout << "Enter burst: ";
                cin >> burstTime;
                Resource r1("R1", burstTime);
                process.pushResource(r1);
            }
            if (option == 2) {
                cout << "Enter burst: ";
                cin >> burstTime;
                Resource r2("R2", burstTime);
                process.pushResource(r2);
            }
        }
        processes.push_back(process);
    }

    while (processes.size() > 0) {
        for (int i = 0; i < processes.size(); i++) {
            if (!visited[i]) continue;
            if (time == processes[i].getArrivalTime()) {
                cpuQueue.push(processes[i]);
                visited[i] = true;
            }
        }
    }
}