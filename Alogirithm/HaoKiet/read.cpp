#pragma once

#include "ALGO.h"
#include <sstream>
#include <string>
#include <regex>

regex pattern(R"(\([A-Z][0-9]\))");
smatch match;
string nameR1 = "none", nameR2 = "none";

vector<process *> ReadInput(string input_file_path, int &ALGO_type, int &num_of_process)
{
    cout << "READING\n";
    fstream input(input_file_path, ios::in);
    vector<process *> pc;

    if (!input)
    {
        cout << "EROOR";
        return pc;
    }

    int RR_time = 0;

    input >> ALGO_type;
    if (ALGO_type == 2)
        input >> RR_time;

    input >> num_of_process;
    input.ignore();
    for (int i = 0; i < num_of_process; i++)
    {
        string line = "";
        getline(input, line);
        istringstream iss(line);

        queue<int> order;
        int r1 = 0, r2 = 0;

        string token;
        process *temp;
        if (ALGO_type == 2)
            temp = new process{RR_time};
        else
            temp = new process{};
        pc.push_back(temp);
        pc[i]->setName(i + 1);
        iss >> token;
        pc[i]->setArriveTime(stoi(token));
        while (iss >> token)
        {
            if (regex_search(token, match, pattern))
            {
                if (nameR1 == "none")
                    nameR1 = match[0];
                else if (match[0] != nameR1 && nameR2 == "none")
                    nameR2 = match[0];
            }

            if (token.find(nameR1) != string::npos)
            {
                order.push(1);
                sscanf(token.c_str(), "%d(R)", &r1);
                if (pc[i]->getR1() == 0)
                    pc[i]->setR1(r1);
                else
                    pc[i]->setR2(r1);
            }
            else if (token.find(nameR2) != string::npos)
            {
                order.push(2);
                sscanf(token.c_str(), "%d(R)", &r2);
                if (pc[i]->getR1() == 0)
                    pc[i]->setR1(r2);
                else
                    pc[i]->setR2(r2);
            }
            else
            {
                order.push(0);
                if (pc[i]->getCPU1() == -1)
                    pc[i]->setCPU1(stoi(token));
                else if (pc[i]->getCPU2() == -1)
                    pc[i]->setCPU2(stoi(token));
            }
        }
        pc[i]->setOrder(order);
        pc[i]->print();
    }
    cout << "FINISHED READING\n";
    return pc;
}