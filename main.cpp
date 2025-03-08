#include ".\Alogirithm\fcfs.cpp"
#include ".\Alogirithm\sjf.cpp"

int main(int argc, char *argv[]) {
    const char* inputPath = argv[1];
    const char* outputPath = argv[2];
    fcfs(inputPath, outputPath);
    // sjf(inputPath, outputPath);
    return 0;
}