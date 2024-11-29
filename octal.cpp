#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class octal {
public:
    void start(const std::string& outputFil, const std::string& inputFile) {
        outputFile = outputFil;
        running = true;
        std::string line;
        std::vector<std::string> lines;

        // Load the input file if specified
        if (!inputFile.empty()) {
            loadFromFile(inputFile, lines);
        }

        std::cout << "octal v1.1 Type ':h' for help.\n";

        while (running) {
            std::cout << "line " << cursor + 1 << " > ";
            std::getline(std::cin, line);

            // Check if user wants to quit without saving
            if (line == command_exit) {
                running = false;
            } 
            // Check if user wants to quit and save to file
            else if (line == command_save_exit) {
                saveToFile(outputFile, lines);  // Save buffer to file
                running = false;  // Exit after saving
            } 
            // Check if user wants help
            else if (line == command_help) {
                std::cout << ":q! to quit\n";
                std::cout << ":wq! to quit and save to the file\n";
                std::cout << ":h to get list of all commands\n";
                std::cout << ":m <line_number> to move the cursor to a specific line\n";
                std::cout << ":d <line_number> to delete a specific line\n";
                std::cout << ":o <line_number> to print a specific line\n";
                std::cout << ":l to check the length of the buffer\n";
                std::cout << ":p to print the buffer\n";
            }
            // Check for the :m command (move cursor)
            else if (line.rfind(command_move, 0) == 0) { // If the command starts with ":m"
                int lineNumber;
                std::stringstream ss(line.substr(command_move.length())); // Remove ":m" prefix and convert the rest
                ss >> lineNumber;

                if (ss.fail() || lineNumber < 1 || lineNumber > lines.size()) {
                    std::cout << "Invalid line number. Please enter a valid line.\n";
                } else {
                    cursor = lineNumber - 1;  // Move the cursor to the specified line (zero-indexed)
                    std::cout << "Cursor moved to line " << lineNumber << ".\n";
                }
            }
            // Check for the :d command (delete line)
            else if (line.rfind(command_delete, 0) == 0) { // If the command starts with ":d"
                int lineNumber;
                std::stringstream ss(line.substr(command_delete.length())); // Remove ":d" prefix and convert the rest
                ss >> lineNumber;

                if (ss.fail() || lineNumber < 1 || lineNumber > lines.size()) {
                    std::cout << "Invalid line number. Please enter a valid line to delete.\n";
                } else {
                    lines.erase(lines.begin() + lineNumber - 1);  // Delete the line at the specified position
                    std::cout << "Line " << lineNumber << " deleted.\n";

                    // Adjust the cursor if it was pointing to the deleted line
                    if (cursor > lines.size()) {
                        cursor = lines.size(); // If the cursor was at the last line, move it to the new last line
                    }
                }
            }
            // Check for the :o command (print specific line)
            else if (line.rfind(command_print, 0) == 0) { // If the command starts with ":o"
                int lineNumber;
                std::stringstream ss(line.substr(command_print.length())); // Remove ":o" prefix and convert the rest
                ss >> lineNumber;

                if (ss.fail() || lineNumber < 1 || lineNumber > lines.size()) {
                    std::cout << "Invalid line number. Please enter a valid line to print.\n";
                } else {
                    std::cout << "Line " << lineNumber << ": " << lines[lineNumber-1] << "\n";
                }
            }
            else if (line == command_length) {
                std::cout << lines.size() << "\n";
            }
            else if (line == command_preview) {
                printBuffer(lines);
            }
            // Otherwise, add or replace the line at the cursor position
            else {
                // If the cursor is within the current range, replace the line at that position
                if (cursor < lines.size()) {
                    lines[cursor] = line;  // Replace existing line at cursor position
                } else {
                    // If the cursor exceeds the number of lines, append the line
                    lines.push_back(line);
                }
                cursor++;  // Move the cursor to the next line after insertion
            }
        }

        // Print the content of the editor before quitting
        std::cout << "\nYour text:\n";
        printBuffer(lines);
    }

private:
    bool running = false;
    int cursor = 0;  // Cursor position (zero-indexed)
    std::string command_exit = ":q!";       // Quit without saving
    std::string command_save_exit = ":wq!"; // Quit and save to file
    std::string command_help = ":h";
    std::string command_move = ":m";
    std::string command_delete = ":d";  // Command for deleting a line
    std::string command_print = ":o";   // Command for printing a specific line
    std::string command_length = ":l";
    std::string command_preview = ":p";

    std::string outputFile;

    void saveToFile(const std::string& fileName, const std::vector<std::string>& lines) {
        if (outputFile == "") {
            std::cout << "Error: No output file specified using '-o' flag.\n";
        } else {
            std::ofstream outFile(fileName);
            if (outFile.is_open()) {
                for (const auto& line : lines) {
                    outFile << line << "\n";  // Write each line to the file
                }
                std::cout << "Buffer saved to " << fileName << ".\n";
            } else {
                std::cout << "Failed to open file " << fileName << " for writing.\n";
            }
        }
    }

    void printBuffer(std::vector<std::string> lines) {
        for (const auto& l : lines) {
            std::cout << l << "\n";
        }
    }

    void loadFromFile(const std::string& inputFile, std::vector<std::string>& lines) {
        std::ifstream inFile(inputFile);
        if (!inFile) {
            std::cout << "Error opening file: " << inputFile << std::endl;
            return;
        }

        // Read the file line by line
        std::string line;
        while (std::getline(inFile, line)) {
            lines.push_back(line);  // Add each line to the buffer
        }
        inFile.close();
        std::cout << "Loaded file: " << inputFile << " into the editor.\n";
    }
};

int main(int argc, char* argv[]) {
    std::string outputFile;
    std::string inputFile;
    
    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[i + 1];
            ++i;  // Skip the next argument since it's the file name
        }
        if (arg == "-i" && i + 1 < argc) {
            inputFile = argv[i + 1];
            ++i;  // Skip the next argument since it's the file name
        }
        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: octal [-i <input_file>] [-o <output_file>] [-h, --help] \n";
            return 0;
        }
    }

    octal editor;
    editor.start(outputFile, inputFile);  // Start the editor with the specified output file and input file (if any)
    return 0;
}
