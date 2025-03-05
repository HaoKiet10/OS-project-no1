#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

class Resource {
private:
    string type;
    int burstTime;
public:
    Resource():
    type(""), burstTime(0) {}
    Resource(string a, int b):
    type(a), burstTime(b) {}

    string getType() {return type;}
    int getBurstTime() {return burstTime;}

    void setType(string a) {type = a;}
    void setBurstTime(int a) {burstTime = a;}
    void minusBurstTime() {burstTime--;}
};

class Process {
private:
    int id;
    int arrivalTime;
    int completeTime = 0;
    int waitingTime = 0;
    queue<Resource*> listResource;
    Resource* runningResource = nullptr;
public:
    Process():
    id(0), arrivalTime(0) {}
    Process(int a, int b):
    id(a), arrivalTime(b) {}

    // Getter
    int getId() {return id;}
    int getArrivalTime() {return arrivalTime;}
    int getTurnAroundTime() {return completeTime - arrivalTime;}
    int getWaitingTime() {return waitingTime;}
    Resource* getRunningResource() {return runningResource;}
    bool isDone() {return runningResource->getBurstTime() == 0;}
    bool isFinish() {return listResource.empty() && runningResource == nullptr;}
    bool nextResource() {
        if (listResource.empty()) {
            runningResource = nullptr;
            return false;
        } // Finish all resources
        runningResource = listResource.front();
        listResource.pop();
        return true;
    }

    // Setter
    void setId(int a) {id = a;}
    void setArrivalTime(int a) {arrivalTime = a;}
    void setCompleteTime(int a) {completeTime = a;}
    void addResource(Resource* a) {listResource.push(a);}
    void removeResource() {listResource.pop();}
    void minusBurstTime() {runningResource->minusBurstTime();}
    void updateWaitingTime() {waitingTime++;}
};

int convertToInt(string str) {return stoi(str);}

void updateWaitingTime(queue<Process*> _) {
    queue<Process*> listProcess(_);
    while(!listProcess.empty()) {
        listProcess.front()->updateWaitingTime();
        listProcess.pop();
    }
}

vector<string> split(string line) {
    vector<string> result;  
    istringstream iss(line);
    string token;
    while (iss >> token) {result.push_back(token);}
    
    return result;
}

bool checkFinish(vector<Process*> listProcess) {
    for (int i = 0; i < listProcess.size(); i++) {
        if (!listProcess[i]->isFinish()) return false;
    }
    return true;
}

void fcfs(string inputPath) {
    ifstream readStream(inputPath);
    queue<Process*> cpuQueue;
    queue<Process*> r1Queue;
    queue<Process*> r2Queue;

    vector<Process*> listProcess;
    Process* cpuRunning = nullptr;
    Process* r1Running = nullptr;
    Process* r2Running = nullptr;

    string tmp;
    string cpuLine;
    string r1Line;
    string r2Line;
    int numberOfProcess;
    int time = 0;

    getline(readStream, tmp);
    getline(readStream, tmp);
    
    numberOfProcess = convertToInt(tmp);
    bool visited[numberOfProcess] = {false};  
    
    // Load data to each process
    for (int i = 0; i < numberOfProcess; i++) {
        getline(readStream, tmp);
        Process* newProcess = new Process();
        vector<string> processInfo = split(tmp);
        int at = convertToInt(processInfo[0]); // arrival time
        newProcess->setId(i + 1);
        newProcess->setArrivalTime(at);

        for (int i = 1; i < processInfo.size(); i++) {
            size_t index = processInfo[i].find("(");
            if (index == string::npos)  // Cpu resource
                newProcess->addResource(new Resource("CPU", convertToInt(processInfo[i])));
            else {
                string type = processInfo[i].substr(index+1, 2);
                int burst = convertToInt(processInfo[i].substr(0, index));
                
                if (type == "R1") 
                    newProcess->addResource(new Resource("R1", convertToInt(processInfo[i].substr(0, index))));
                else 
                    newProcess->addResource(new Resource("R2", convertToInt(processInfo[i].substr(0, index))));
            }
        }
        listProcess.push_back(newProcess);
    }

    for (int i = 0; i < listProcess.size(); i++) {listProcess[i]->nextResource();} // Start with first resource

    while(true) {
        for (int i = 0; i < listProcess.size(); i++) {
            if (listProcess[i]->isFinish() && !visited[i]) {listProcess[i]->setCompleteTime(time);}
        }

        if (checkFinish(listProcess)) {break;}
        for (int i = 0; i < listProcess.size(); i++) {
            if (listProcess[i]->getArrivalTime() == time) {
                if (listProcess[i]->getRunningResource()->getType() == "CPU") {cpuQueue.push(listProcess[i]);}
                if (listProcess[i]->getRunningResource()->getType() == "R1") {r1Queue.push(listProcess[i]);}
                if (listProcess[i]->getRunningResource()->getType() == "R2") {r2Queue.push(listProcess[i]);}
            }
        }

        if (cpuRunning == nullptr) {
            if (!cpuQueue.empty()) {
                cpuRunning = cpuQueue.front();
                cpuQueue.pop();
            }
        }
        if (r1Running == nullptr) {
            if (!r1Queue.empty()) {
                r1Running = r1Queue.front();
                r1Queue.pop();
            }
        }
        if (r2Running == nullptr) {
            if (!r2Queue.empty()) {
                r2Running = r2Queue.front();
                r2Queue.pop();
            }
        }

        if (cpuRunning != nullptr) {
            cpuLine += to_string(cpuRunning->getId());
            cpuRunning->minusBurstTime();
            if (cpuRunning->isDone()) {
                if (cpuRunning->nextResource()) {
                    if (cpuRunning->getRunningResource()->getType() == "CPU") {cpuQueue.push(cpuRunning);}
                    if (cpuRunning->getRunningResource()->getType() == "R1") {r1Queue.push(cpuRunning);}
                    if (cpuRunning->getRunningResource()->getType() == "R2") {r2Queue.push(cpuRunning);}
                }
                cpuRunning = nullptr;
            }
        }
        else cpuLine += "_";
        if (r1Running != nullptr) {
            r1Line += to_string(r1Running->getId());
            r1Running->minusBurstTime();
            if (r1Running->isDone()) {
                if (r1Running->nextResource()) {
                    if (r1Running->getRunningResource()->getType() == "CPU") {cpuQueue.push(r1Running);}
                    if (r1Running->getRunningResource()->getType() == "R1") {r1Queue.push(r1Running);}
                    if (r1Running->getRunningResource()->getType() == "R2") {r2Queue.push(r1Running);}
                }
                r1Running = nullptr;
            }
        }
        else r1Line += "_";
        if (r2Running != nullptr) {
            r2Line += to_string(r2Running->getId());
            r2Running->minusBurstTime();
            if (r2Running->isDone()) {
                if (r2Running->nextResource()) {
                    if (r2Running->getRunningResource()->getType() == "CPU") {cpuQueue.push(r2Running);}
                    if (r2Running->getRunningResource()->getType() == "R1") {r1Queue.push(r2Running);}
                    if (r2Running->getRunningResource()->getType() == "R2") {r2Queue.push(r2Running);}
                }   
                r2Running = nullptr;
            }
        }
        else r2Line += "_";

        updateWaitingTime(cpuQueue);
        time++;
    }   
    cout << cpuLine << endl;
    cout << r1Line << endl;
    cout << r2Line << endl;
    for (int i = 0; i < listProcess.size(); i++) 
        cout << listProcess[i]->getTurnAroundTime() << " ";
    cout << endl;
    for (int i = 0; i < listProcess.size(); i++) 
        cout << listProcess[i]->getWaitingTime() << " ";
}