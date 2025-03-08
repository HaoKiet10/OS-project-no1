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
    int turn_around_time;
    int waiting_time;

    queue<int> order;

public:
    int progressTime;
    int endTime;

    process() : arrive_time(0), CPU_1(0), CPU_2(0), R_1(0), R_2(0), RR_time(0), turn_around_time(0), waiting_time(0) {}
    process(int at, int c1, int c2, int r1, int r2, queue<int> o) : arrive_time(at), CPU_1(c1), CPU_2(c2), R_1(r1), R_2(r2), RR_time(0), turn_around_time(0), waiting_time(0), order(o) {}
    process(int at, int c1, int c2, int r1, int r2, int rrt, queue<int> o) : arrive_time(at), CPU_1(c1), CPU_2(c2), R_1(r1), R_2(r2), RR_time(rrt), turn_around_time(0), waiting_time(0), order(o) {}

    void setName(int n)
    {
        name = n;
    }

    int getName()
    {
        return name;
    }

    int getWaitingTime()
    {
        return waiting_time;
    }

    bool isDone()
    {
        if (CPU_1 == 0 && CPU_2 == 0 && R_1 == 0 && raise == 0)
            return true;
        return false;
    }

    int getNextOrder()
    {
        return order.front();
    }

    int getArriveTime()
    {
        return arrive_time;
    }

    void wait()
    {
        waiting_time++;
    }

    int RR_run()
    {
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

    int FCFS_run(int type)
    {
        if (type == CPU)
        {
            if (CPU_1 > 0)
            {
                int temp = CPU_1;
                CPU_1 = 0;
                return temp;
            }
            else
            {
                int temp = CPU_2;
                CPU_2 = 0;
                return temp;
            }
        }
        else if (type == R1)
        {
            int temp = R_1;
            R_1 = 0;
            return temp;
        }
        else if (type == R2)
        {
            int temp = R_2;
            R_2 = 0;
            return temp;
        }
        return 0;
    }
};