#include ".\Alogirithm\ThanhDat\fcfs.cpp"
#include ".\Alogirithm\ThanhDat\sjf.cpp"
#include ".\Alogirithm\HaoKiet\srtn.cpp"
#include ".\Alogirithm\HaoKiet\rr.cpp"

int main(int argc, char *argv[])
{
    const char *inputPath = argv[1];
    const char *outputPath = argv[2];
    int option;
    ifstream readStream(inputPath);
    readStream >> option;

    if (option == 1) fcfs(inputPath, outputPath);
    else if (option == 2) rr(inputPath, outputPath);
    else if (option == 3) sjf(inputPath, outputPath);
    else if (option == 4) srtn(inputPath, outputPath);
    return 0;
}