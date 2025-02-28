#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    printf("%s\n%s", inputFile, outputFile);
    return 0;
}