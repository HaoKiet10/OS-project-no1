#include "ALGO.h"
#include <sstream>

vector<process> ReadInput(string input_file_path, int &ALGO_type, int &RR_time, int &num_of_process)
{
    fstream input(input_file_path, ios::in);
    vector<process> pc;

    if (!input)
        return pc;

    input >> ALGO_type;
    if (ALGO_type == 2)
        input >> RR_time;

    input >> num_of_process;
    for (int i = 0; i < num_of_process; i++)
    {
        string line = "";
        getline(input, line);
        istringstream iss(line);

        vector<int> value;
        queue<int> order;
        int r1 = 0, r2 = 0;

        string token;
        while (iss >> token)
        {
            if (token.find("(R)") != string::npos || token.find("(R1)") != string::npos)
            {
                order.push(1);
                sscanf(token.c_str(), "%d(R)", &r1);
            }
            else if (token.find("(R2)") != string::npos)
            {
                order.push(2);
                sscanf(token.c_str(), "%d(R)", &r2);
            }
            else
            {
                order.push(0);
                value.push_back(stoi(token));
            }
        }
        if (ALGO_type == 2)
            pc.push_back(process{value[0], value[1], value[2], r1, r2, RR_time, order});
        else
            pc.push_back(process{value[0], value[1], value[2], r1, r2, order});
    }
    return pc;
}