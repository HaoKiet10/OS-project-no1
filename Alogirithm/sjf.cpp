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
  
vector<string> split(string line) {
    vector<string> result;  
    istringstream iss(line);
    string token;
    while (iss >> token) {result.push_back(token);}
    
    return result;
}

void sjf(string inputPath) {
    ifstream readStream(inputPath);
    vector<Process*> listProcess;

    string tmp;
    int numberOfProcess;
    int time = 0;

    getline(readStream, tmp);
    getline(readStream, tmp);

    numberOfProcess = convertToInt(tmp);
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
    
}