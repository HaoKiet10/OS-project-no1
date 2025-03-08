#include "ALGO.h"
#include "Read.cpp"
#include "Algorithm/RR.cpp"
#include "Algorithm/SRTN.cpp"

int main(int argc, char *argv[])
{
    string input_file, output_file;
    if (argc == 3)
    {
        input_file = argv[1];
        output_file = argv[2];
    }
    else
    {
        cout << "Enter input file: ";
        getline(cin, input_file);
        cout << "Enter output file: ";
        getline(cin, output_file);
    }

    cout << output_file;
    int ALGO_type = 0, RR_time = 0, num_of_process = 0;
    vector<process> pc;
    pc = ReadInput(input_file, ALGO_type, RR_time, num_of_process);

    if (ALGO_type == 1)
    {
        return 0;
    }
    else if (ALGO_type == 2)
    {
        return RR_ALGO(pc, num_of_process, output_file);
    }
    else if (ALGO_type == 3)
    {
        return 0;
    }
    else if (ALGO_type == 4)
    {
        return 0;
    }
    else
        return 0;
}