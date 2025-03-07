#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

#define MAX_RESOURCE 3

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
    int getWaitingTime() {return waitingTime;}
    int getCompleteTime() {return completeTime;}
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
    void setRunningResource(Resource* a) {runningResource = a;}
    void addResource(Resource* a) {listResource.push(a);}
    void removeResource() {listResource.pop();}
    void minusBurstTime() {runningResource->minusBurstTime();}
    void updateWaitingTime() {waitingTime++;}
};

int convertToInt(string str) {return stoi(str);}

int getBurstTime(string str) {
    string result = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '(') break;
        result += str[i];
    }
    return convertToInt(result);
}

void updateWaitingTime(queue<Process*> _) {
    queue<Process*> listProcess(_);
    while(!listProcess.empty()) {
        listProcess.front()->updateWaitingTime();
        listProcess.pop();
    }
}

void writeFile(vector<string>lines, string outputPath, vector<Process*> listProcess) {
    ofstream writeStream(outputPath);
    for (auto it : lines) {if (it == "") break; writeStream << it << endl;}
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == '_') continue;
            int rTime = j+1;
            int id = convertToInt(lines[i].substr(j, 1));
            if (listProcess[id-1]->getCompleteTime() < rTime) listProcess[id-1]->setCompleteTime(rTime);
        }
    } 
    for (auto it : listProcess) {writeStream << it->getCompleteTime() - it->getArrivalTime() << ' ';}
    writeStream << endl;
    for (auto it : listProcess) {writeStream << it->getWaitingTime() << ' ';}

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

string getResourceType(string str) {
    size_t index = str.find("(");
    string type = "";
    if (index == string::npos) return "CPU";
    for (int i = index+1; i < str.size(); i++) {
        if (str[i] == ')') break;
        type += str[i];
    }
    return type;
}

void fcfs(string inputPath) {
    string tmp;
    int numberOfProcess;
    int time = 0;

    map<string, int> resourceTable;
    ifstream readStream(inputPath);

    getline(readStream, tmp);
    getline(readStream, tmp);
    numberOfProcess = convertToInt(tmp);

    vector<queue<Process*>> resourceQueue(MAX_RESOURCE);
    vector<Process*> listProcess;
    vector<Process*> listRunning(MAX_RESOURCE);
    vector<string> line(MAX_RESOURCE, "");
    
    resourceTable["CPU"] = 0;
    // Load data to each process
    for (int i = 0; i < numberOfProcess; i++) {
        getline(readStream, tmp);
        Process* newProcess = new Process();
        vector<string> processInfo = split(tmp);

        newProcess->setId(i + 1);
        newProcess->setArrivalTime(convertToInt(processInfo[0]));

        // Load resource to each process
        for (int i = 1; i < processInfo.size(); i++) {
            string type = getResourceType(processInfo[i]);
            if (type == "CPU") 
                newProcess->addResource(new Resource("CPU", convertToInt(processInfo[i])));
            else {
                if (resourceTable.find(type) == resourceTable.end()) {resourceTable[type] = resourceTable.size();}
                int burstTime = getBurstTime(processInfo[i]);
                newProcess->addResource(new Resource(type, burstTime));
            }
        }
        listProcess.push_back(newProcess);
    }

    for (int i = 0; i < listProcess.size(); i++) {listProcess[i]->nextResource();} // Start with first resource

    while(true) {
        if (checkFinish(listProcess)) {break;}
        for (int i = 0; i < listProcess.size(); i++) {
            if (listProcess[i]->getArrivalTime() == time) {
                int index = resourceTable[listProcess[i]->getRunningResource()->getType()];
                resourceQueue[index].push(listProcess[i]);
            }
        }

        for (int i = 0; i < listRunning.size(); i++) {
            if (listRunning[i] == nullptr) continue;
            if (listRunning[i]->isDone()) { 
                if (listRunning[i]->nextResource()) {
                    int index = resourceTable[listRunning[i]->getRunningResource()->getType()];
                    resourceQueue[index].push(listRunning[i]);
                }
                else listRunning[i]->setRunningResource(nullptr);
                listRunning[i] = nullptr;
            }
        }
        // if (cpuRunning != nullptr)
        //     if (cpuRunning->isDone()) {
        //         if (cpuRunning->nextResource()) {
        //             if (cpuRunning->getRunningResource()->getType() == "CPU") {cpuQueue.push(cpuRunning);}
        //             if (cpuRunning->getRunningResource()->getType() == "R1") {r1Queue.push(cpuRunning);}
        //             if (cpuRunning->getRunningResource()->getType() == "R2") {r2Queue.push(cpuRunning);}
        //         }
        //         else cpuRunning->setRunningResource(nullptr);
        //         cpuRunning = nullptr;
        //     }
        // if (r1Running != nullptr)
        //     if (r1Running->isDone()) {
        //         if (r1Running->nextResource()) {
        //             if (r1Running->getRunningResource()->getType() == "CPU") {cpuQueue.push(r1Running);}
        //             if (r1Running->getRunningResource()->getType() == "R1") {r1Queue.push(r1Running);}
        //             if (r1Running->getRunningResource()->getType() == "R2") {r2Queue.push(r1Running);}
        //         }
        //         else r1Running->setRunningResource(nullptr);
        //         r1Running = nullptr;
        //     }
        // if (r2Running != nullptr)
        //     if (r2Running->isDone()) {
        //         if (r2Running->nextResource()) {
        //             if (r2Running->getRunningResource()->getType() == "CPU") {cpuQueue.push(r2Running);}
        //             if (r2Running->getRunningResource()->getType() == "R1") {r1Queue.push(r2Running);}
        //             if (r2Running->getRunningResource()->getType() == "R2") {r2Queue.push(r2Running);}
        //         }   
        //         else r2Running->setRunningResource(nullptr);
        //         r2Running = nullptr;
        //     }


        for (int i = 0; i < listRunning.size(); i++) {
            if (listRunning[i] != nullptr) continue;
            if (resourceQueue[i].empty()) continue;
            listRunning[i] = resourceQueue[i].front();
            resourceQueue[i].pop();
        }
        // if (cpuRunning == nullptr) {
        //     if (!cpuQueue.empty()) {
        //         cpuRunning = cpuQueue.front();
        //         cpuQueue.pop();
        //     }
        // }
        // if (r1Running == nullptr) {
        //     if (!r1Queue.empty()) {
        //         r1Running = r1Queue.front();
        //         r1Queue.pop();
        //     }
        // }
        // if (r2Running == nullptr) {
        //     if (!r2Queue.empty()) {
        //         r2Running = r2Queue.front();
        //         r2Queue.pop();
        //     }
        // }

        for (int i = 0; i < listRunning.size(); i++) {
            if (i >= resourceTable.size()) break;
            if (listRunning[i] == nullptr) line[i] += "_";
            else {
                line[i] += to_string(listRunning[i]->getId());
                listRunning[i]->minusBurstTime();
            }
        }
        // if (cpuRunning != nullptr) {
        //     cpuLine += to_string(cpuRunning->getId());
        //     cpuRunning->minusBurstTime();
        // }
        // else cpuLine += "_";
        // if (r1Running != nullptr) {
        //     r1Line += to_string(r1Running->getId());
        //     r1Running->minusBurstTime();
        // }
        // else r1Line += "_";
        // if (r2Running != nullptr) {
        //     r2Line += to_string(r2Running->getId());
        //     r2Running->minusBurstTime();
        // }
        // else r2Line += "_";
        updateWaitingTime(resourceQueue[0]);
        time++;
    }   
    // Delete the last character of each line
    for (int i = 0; i < line.size(); i++) {if (line[i] == "") break;line[i].pop_back();}
    writeFile(line, "output.txt", listProcess);
}