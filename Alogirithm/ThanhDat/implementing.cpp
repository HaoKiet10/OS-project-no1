#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

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

queue<Process*> copyQueueFromPQ(priority_queue<pair<int, Process*>, vector<pair<int, Process*>>, greater<>> _) {
    priority_queue<pair<int, Process*>, vector<pair<int, Process*>>, greater<>> pq(_);
    queue<Process*> result;
    while(!pq.empty()) {
        result.push(pq.top().second);
        pq.pop();
    }
    return result;

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