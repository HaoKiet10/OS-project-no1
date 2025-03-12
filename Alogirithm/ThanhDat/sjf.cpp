#include "implementing.cpp"

void sjf(string inputPath, string outputPath) {
    priority_queue<pair<int, Process*>, vector<pair<int, Process*>>, greater<>> cpuQueue;
    Process* cpuRunning = nullptr;

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
        if (checkFinish(listProcess)) {break;} // If all process are done, break the loop

        // Push process to queue if it arrives
        for (int i = 0; i < listProcess.size(); i++) {
            if (listProcess[i]->getArrivalTime() == time) {
                int index = resourceTable[listProcess[i]->getRunningResource()->getType()];
                if (index == 0) cpuQueue.push({listProcess[i]->getRunningResource()->getBurstTime(), listProcess[i]});
                else resourceQueue[index].push(listProcess[i]);
            }
        }   

        for (int i = 0; i < listRunning.size(); i++) {
            if (listRunning[i] == nullptr) continue;
            if (listRunning[i]->isDone()) { 
                if (listRunning[i]->nextResource()) {
                    int index = resourceTable[listRunning[i]->getRunningResource()->getType()];
                    if (index == 0) cpuQueue.push({listRunning[i]->getRunningResource()->getBurstTime(), listRunning[i]});
                    else resourceQueue[index].push(listRunning[i]);
                }
                else listRunning[i]->setRunningResource(nullptr);
                listRunning[i] = nullptr;
            }
        }

        for (int i = 0; i < listRunning.size(); i++) {
            if (listRunning[i] != nullptr) continue;
            if (i == 0) {
                if (cpuQueue.empty()) continue;
                listRunning[i] = cpuQueue.top().second;
                cpuQueue.pop();
            }
            else {
                if (resourceQueue[i].empty()) continue;
                listRunning[i] = resourceQueue[i].front();
                resourceQueue[i].pop();
            }
        }

        for (int i = 0; i < listRunning.size(); i++) {
            if (i >= resourceTable.size()) break;
            if (listRunning[i] == nullptr) line[i] += "_";
            else {
                line[i] += to_string(listRunning[i]->getId());
                listRunning[i]->minusBurstTime();
                if (i == 0) {
                    queue<Process*> copy = copyQueueFromPQ(cpuQueue);
                    updateWaitingTime(copy);
                    if (!listRunning[i]->isDone()) {
                        cpuQueue.push({listRunning[i]->getRunningResource()->getBurstTime(), listRunning[i]});
                        listRunning[i] = nullptr;
                    }
                }
            }
        }

        time++;
    }
    // Delete the last character of each line
    for (int i = 0; i < line.size(); i++) {if (line[i] == "") break;line[i].pop_back();}
    writeFile(line, outputPath, listProcess);

    // Test
    
}