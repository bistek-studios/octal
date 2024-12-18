#include "octal.hpp"
#include "colors.hpp"
#include <string>

octal::octal(std::string version = "?.x") {
    this->version = version;
}

void octal::start(std::string& inputFile) {
    running = true;
    std::string line;
    std::vector<std::string> lines;

    std::cout << Color::BLUE << "octal v" << version << Color::YELLOW << "\nType ':h' for help.\n" << Color::RESET;

    // Load the input file if specified
    if (!inputFile.empty()) {
        loadFromFile(inputFile, lines);
    }

    while (running) {
        std::cout << "line " << Color::GREEN << cursor + 1 << Color::RESET << " > ";
        std::getline(std::cin, line);

        // Check if user wants to quit without saving
        if (line == command_exit) {
            running = false;
        } 
        // Check if user wants to quit and save to file
        else if (line.rfind(command_save_exit, 0) == 0) {
            std::string str = line.substr(command_save_exit.length()); // Remove ":d" prefix and convert the rest
            str = trim(str);
            if (str.empty()) {
                str = inputFile;
            }
            saveToFile(str, lines);  // Save buffer to file
            std::cout << "\nYour text:\n";
            printBuffer(lines);
            running = false;  // Exit after saving
        } 
        else if (line.rfind(command_save, 0) == 0) {
            std::string str = line.substr(command_save.length()); // Remove ":d" prefix and convert the rest
            str = trim(str);
            if (str.empty()) {
                str = inputFile;
            }
            saveToFile(str, lines);  // Save buffer to file
            std::cout << "\nYour text:\n";
            printBuffer(lines);
        } 
        // Check if user wants help
        else if (line == command_help) {
            std::cout << ":q! to quit\n";
            std::cout << ":w [<file>] to save file buffer to output file, input file if not specified\n";
            std::cout << ":wq! [<file>] to quit and save to the set output file, input file if not specified\n";
            std::cout << ":h to get list of all commands\n";
            std::cout << ":m <line_number> to move the cursor to a specific line\n";
            std::cout << ":d <line_number> to delete a specific line\n";
            std::cout << ":o <line_number> to print a specific line\n";
            std::cout << ":l to check the length of the buffer\n";
            std::cout << ":p to print the buffer\n";
            std::cout << ":e <filename> to load a file into the buffer\n";
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
            std::cout << "\n";
            printBuffer(lines);
            std::cout << "\n";
        }
        else if (line.rfind(command_load, 0) == 0) {
            std::string str = line.substr(command_load.length()); // Remove ":d" prefix and convert the rest
            str = trim(str);
            loadFromFile(str,lines);
            inputFile = str;
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
}

void octal::saveToFile(const std::string& fileName, const std::vector<std::string>& lines) {
    if (fileName.empty()) {
        std::cout << "Error: No output file specified.\n";
    } else {
        std::ofstream outFile(fileName);
        if (outFile.is_open()) {
            for (const auto& line : lines) {
                outFile << line << "\n";
            }
            std::cout << "Buffer saved to " << fileName << ".\n";
        } else {
            std::cout << "Failed to open file " << fileName << " for writing.\n";
        }
    }
}

void octal::printBuffer(std::vector<std::string> lines) {
    int index = 0;
    std::for_each(lines.begin(), lines.end(), [&index](const std::string& l) {
        std::cout << Color::GREEN << index + 1 << Color::RESET << " | " << l << "\n";
        ++index;
    });
}

void octal::loadFromFile(const std::string& inputFile, std::vector<std::string>& lines) {
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cout << "Error opening file: " << inputFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();
    cursor = lines.size();
    std::cout << "\n" << inputFile << ":\n";
    printBuffer(lines);
    std::cout << "\n";
}

std::string octal::trim(const std::string& str) { 
    return str.empty() ? str : str.substr(str.find_first_not_of(" \t\n\r"), 
                                         (str.find_last_not_of(" \t\n\r") - str.find_first_not_of(" \t\n\r") + 1)); 
}
