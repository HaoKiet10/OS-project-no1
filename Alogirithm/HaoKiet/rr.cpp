#pragma once

#include "ALGO.h"
#include "read.cpp"

int RoundRobin(string input_file_path, string output_file_path)
{
    int ALGO_type = 0, numOfProcess = 0;
    vector<process *> pc = ReadInput(input_file_path, ALGO_type, numOfProcess);
    if (ALGO_type == 2)
        return RR_run(pc, numOfProcess, output_file_path);
    return 0;
}
bool RR_run(vector<process *> pc, int numOfProcess, string output_file_path)
{
    vector<int> CPU_usage, R1_usage, R2_usage;
    queue<process *> CPU_wait;
    queue<process *> R1_wait;
    queue<process *> R2_wait;

    vector<process *> inProgress;

    int cpu_counter = 0, r1_counter = 0, r2_counter = 0;
    int time = 0;
    bool allProcessDone = 0;
    while (!allProcessDone || !inProgress.empty())
    {
        cout << "\nTIME: " << time << endl;
        for (int i = 0; i < numOfProcess; i++)
        {
            int arriveTime = pc[i]->getArriveTime();
            if (arriveTime == time)
            {
                cout << "Process " << pc[i]->getName() << " arrives at time: " << time << endl;
                int typeOfProcess = pc[i]->getNextOrder();
                // cout << typeOfProcess << endl;
                if (typeOfProcess == 0)
                    CPU_wait.push(pc[i]);
                else if (typeOfProcess == 1)
                    R1_wait.push(pc[i]);
                else if (typeOfProcess == 2)
                    R2_wait.push(pc[i]);
            }
        }

        if (!inProgress.empty())
        {
            int size = inProgress.size();
            for (int i = 0; i < size; i++)
            {
                if (time == inProgress[i]->progressTime)
                {
                    process *temp = inProgress[i];
                    inProgress.erase(inProgress.begin() + i);
                    if (!temp->isDone())
                    {
                        int typeOfProcess = temp->getNextOrder();
                        if (typeOfProcess == 0)
                            CPU_wait.push(temp);
                        else if (typeOfProcess == 1)
                            R1_wait.push(temp);
                        else if (typeOfProcess == 2)
                            R2_wait.push(temp);
                    }
                    else
                    {
                        cout << "Process " << temp->getName() << " done at time: " << time - 1 << endl;
                        temp->endTime = time - 1;
                    }
                }
            }
        }

        if (CPU_wait.empty() && cpu_counter == time)
        {
            CPU_usage.push_back(-1);
            cpu_counter++;
        }
        else if (cpu_counter == time)
        {
            cout << "List process to run (CPU):\n";
            for (int i = 0; i < CPU_wait.size(); i++)
            {
                process *temp2 = CPU_wait.front();
                CPU_wait.pop();
                temp2->print();
                CPU_wait.push(temp2);
            }
            process *temp = CPU_wait.front();
            CPU_wait.pop();
            int size = CPU_wait.size();
            for (int i = 0; i < size; i++)
            {
                process *temp2 = CPU_wait.front();
                CPU_wait.pop();
                temp2->wait();
                CPU_wait.push(temp2);
            }
            int run_time = temp->RR_run();
            int name = temp->getName();
            cpu_counter += run_time;
            temp->progressTime = time + run_time;
            inProgress.push_back(temp);

            for (; run_time > 0; run_time--)
                CPU_usage.push_back(name);
        }
        else
        {
            int size = CPU_wait.size();
            for (int i = 0; i < size; i++)
            {
                process *temp = CPU_wait.front();
                CPU_wait.pop();
                temp->wait();
                CPU_wait.push(temp);
            }
        }

        if (R1_wait.empty() && r1_counter == time)
        {
            R1_usage.push_back(-1);
            r1_counter++;
        }
        else if (r1_counter == time)
        {
            process *temp = R1_wait.front();
            R1_wait.pop();
            int run_time = temp->FCFS_run();
            int name = temp->getName();
            r1_counter += run_time;
            temp->progressTime = time + run_time;
            inProgress.push_back(temp);

            cout << "Process " << name << " runs R1...\n";

            for (; run_time > 0; run_time--)
                R1_usage.push_back(name);
        }
        else
        {
            int size = R1_wait.size();
            for (int i = 0; i < size; i++)
            {
                process *temp = R1_wait.front();
                R1_wait.pop();
                temp->wait();
                R1_wait.push(temp);
            }
        }

        if (R2_wait.empty() && r2_counter == time)
        {
            R2_usage.push_back(-1);
            r2_counter++;
        }
        else if (r2_counter == time)
        {
            process *temp = R2_wait.front();
            R2_wait.pop();
            int run_time = temp->FCFS_run();
            int name = temp->getName();
            r2_counter += run_time;
            temp->progressTime = time + run_time;
            inProgress.push_back(temp);

            cout << "Process " << name << " runs R2...\n";

            for (; run_time > 0; run_time--)
                R2_usage.push_back(name);
        }
        else
        {
            int size = R2_wait.size();
            for (int i = 0; i < size; i++)
            {
                process *temp = R2_wait.front();
                R2_wait.pop();
                temp->wait();
                R2_wait.push(temp);
            }
        }

        time++;
        allProcessDone = 1;
        for (int i = 0; i < numOfProcess; i++)
        {
            allProcessDone &= pc[i]->isDone();
        }
    }

    return WriteOutput(output_file_path, CPU_usage, R1_usage, R2_usage, pc);
}