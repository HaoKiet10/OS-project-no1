#include ".\Alogirithm\ThanhDat\fcfs.cpp"
#include ".\Alogirithm\ThanhDat\sjf.cpp"

int main(int argc, char *argv[]) {
    const char* inputPath = argv[1];
    const char* outputPath = argv[2];
    if (inputPath == NULL) inputPath = "input.txt"; // Default input file
    if (outputPath == NULL) outputPath = "output.txt"; // Default output file
    ifstream readStream(inputPath);
    int option;

    if (!readStream.is_open()) {
        cout << "Error: File not found" << endl;
        return 1;
    }
    readStream >> option;
    
    if (option == 1) fcfs(inputPath, outputPath);
    else if (option == 2);
    else if (option == 3) sjf(inputPath, outputPath);
    else if (option == 4);
    else cout << "Error: Invalid option" << endl;
    return 0;
}