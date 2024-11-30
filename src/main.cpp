#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "lib/octal.hpp"


int main(int argc, char* argv[]) {
    std::string inputFile;

    int i;
    // Parse command-line arguments
    for (i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: octal [OPTIONS] [<input_file>]  \n";
            std::cout << "-h, --help - usage of command\n";
            return 0;
        }
        else { // must be input file
            inputFile = argv[i];
        }
    }

    octal editor("1.4");
    editor.start(inputFile);  // Start the editor with the specified input file (if any)
    return 0;
}
