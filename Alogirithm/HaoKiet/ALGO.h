#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

#define CPU 0
#define R1 1
#define R2 2

class process
{
    int name;
    int arrive_time, CPU_1, CPU_2, R_1, R_2;
    int RR_time;
    queue<int> order;

public:
    int turn_around_time = 0;
    int waiting_time = 0;
    int progressTime = 0;
    int endTime = 0;

    process() : arrive_time(-1), CPU_1(-1), CPU_2(-1), R_1(0), R_2(0), RR_time(0), turn_around_time(0), waiting_time(0) {}
    process(int rrt) : arrive_time(-1), CPU_1(-1), CPU_2(-1), R_1(0), R_2(0), RR_time(rrt), turn_around_time(0), waiting_time(0) {}
    bool operator>(const process &p2) const
    {
        if (this->CPU_1 > 0 && p2.CPU_1 > 0)
            return this->CPU_1 > p2.CPU_1;
        else if (this->CPU_1 > 0 && p2.CPU_2 > 0)
            return this->CPU_1 > p2.CPU_2;
        else if (this->CPU_2 > 0 && p2.CPU_1 > 0)
            return this->CPU_2 > p2.CPU_1;
        else if (this->CPU_2 > 0 && p2.CPU_2 > 0)
            return this->CPU_2 > p2.CPU_2;
        return 0;
    }
    bool operator<=(const process &p2) const
    {
        return !(*this > p2);
    }

    void setName(int n)
    {
        name = n;
    }

    void setArriveTime(int at)
    {
        arrive_time = at;
    }

    void setCPU1(int c1)
    {
        CPU_1 = c1;
    }

    void setCPU2(int c2)
    {
        CPU_2 = c2;
    }

    void setR1(int r1)
    {
        R_1 = r1;
    }

    void setR2(int r2)
    {
        R_2 = r2;
    }

    void setRRT(int rrt)
    {
        RR_time = rrt;
    }

    void setOrder(queue<int> o)
    {
        order = o;
    }

    int getName()
    {
        return name;
    }

    int getArriveTime()
    {
        return arrive_time;
    }

    int getCPU1()
    {
        return CPU_1;
    }

    int getCPU2()
    {
        return CPU_2;
    }

    int getR1()
    {
        return R_1;
    }

    int getR2()
    {
        return R_2;
    }

    int getWaitingTime()
    {
        return waiting_time;
    }

    bool isDone()
    {
        if (CPU_1 <= 0 && CPU_2 <= 0 && R_1 <= 0 && R_2 <= 0)
            return true;
        return false;
    }

    int getNextOrder()
    {
        return order.front();
    }

    void wait()
    {
        waiting_time++;
    }

    int RR_run()
    {
        cout << "Process " << name << " runs CPU...\n";
        if (CPU_1 > RR_time)
        {
            CPU_1 -= RR_time;
            return RR_time;
        }
        else if (CPU_1 > 0)
        {
            int value = CPU_1;
            CPU_1 = 0;
            order.pop();
            return value;
        }
        else if (CPU_2 > RR_time)
        {
            CPU_2 -= RR_time;
            return RR_time;
        }
        else if (CPU_2 > 0)
        {
            int value = CPU_2;
            CPU_2 = 0;
            order.pop();
            return value;
        }
        else
            return 0;
    }

    int FCFS_run()
    {
        // if (type == CPU)
        // {
        //     if (CPU_1 > 0)
        //     {
        //         int temp = CPU_1;
        //         CPU_1 = 0;
        //         order.pop();
        //         return temp;
        //     }
        //     else if (CPU_2 > 0)
        //     {
        //         int temp = CPU_2;
        //         CPU_2 = 0;
        //         order.pop();
        //         return temp;
        //     }
        //     else
        //         return 0;
        // }
        // else if (type == R1)
        // {
        //     int temp = R_1;
        //     R_1 = 0;
        //     order.pop();
        //     return temp;
        // }
        // else if (type == R2)
        // {
        //     int temp = R_2;
        //     R_2 = 0;
        //     order.pop();
        //     return temp;
        // }
        // return 0;

        if (R_1 > 0)
        {
            order.pop();
            int value = R_1;
            R_1 = 0;
            return value;
        }
        else if (R_2 > 0)
        {
            order.pop();
            int value = R_2;
            R_2 = 0;
            return value;
        }
        else
            return 0;
    }

    int SRTF_run()
    {
        cout << "Process " << name << " runs CPU...\n";
        if (CPU_1 > 1)
        {
            CPU_1 -= 1;
        }
        else if (CPU_1 == 1)
        {
            CPU_1 = 0;
            order.pop();
        }
        else if (CPU_2 > 1)
            CPU_2 -= 1;
        else if (CPU_2 == 1)
        {
            CPU_2 = 0;
            order.pop();
        }
        else
            return 0;
        return 1;
    }

    void print()
    {
        cout << "Name: " << name;
        cout << "\tC1: " << CPU_1 << "\tC2: " << CPU_2 << "\tR1: " << R_1 << "\tR2: " << R_2 << endl;
    }
};

bool WriteOutput(string output_file_path, vector<int> CPU_usage, vector<int> R1_usage, vector<int> R2_usage, vector<process *> pc)
{
    cout << "WRITTING OUTPUT...\n";
    fstream output(output_file_path, ios::out);
    if (!output)
    {
        cout << "ERROR";
        return 0;
    }
    int cpu_len = CPU_usage.size();
    int r1_len = R1_usage.size();
    int r2_len = R2_usage.size();

    for (int i = 0; i < cpu_len; i++)
    {
        if (CPU_usage[i] == -1)
            output << "_";
        else
            output << CPU_usage[i];
    }
    output << endl;
    for (int i = 0; i < r1_len; i++)
        if (R1_usage[i] == -1)
            output << "_";
        else
            output << R1_usage[i];
    output << endl;
    for (int i = 0; i < r2_len; i++)
        if (R2_usage[i] == -1)
            output << "_";
        else
            output << R2_usage[i];
    output << endl;

    for (int i = 0; i < pc.size(); i++)
    {
        output << pc[i]->endTime - pc[i]->getArriveTime() << " ";
    }
    output << endl;
    for (int i = 0; i < pc.size(); i++)
    {
        output << pc[i]->getWaitingTime() << " ";
    }

    output.close();
    cout << "DONE...\n";
    return 1;
}