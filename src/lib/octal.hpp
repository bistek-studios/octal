#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

class octal {
public:
    octal(std::string version);

    void start(std::string& inputFile);

private:
    // Member variables
    bool running = false;
    int cursor = 0;  // Cursor position (zero-indexed)
    std::string command_exit = ":q!";       // Quit without saving
    std::string command_save_exit = ":wq!"; // Quit and save to file
    std::string command_save = ":w";
    std::string command_help = ":h";
    std::string command_move = ":m";
    std::string command_delete = ":d";  // Command for deleting a line
    std::string command_print = ":o";   // Command for printing a specific line
    std::string command_length = ":l";
    std::string command_preview = ":p";
    std::string command_load = ":e";
    std::string version;

    // Private methods
    void saveToFile(const std::string& fileName, const std::vector<std::string>& lines);
    void printBuffer(std::vector<std::string> lines);
    void loadFromFile(const std::string& inputFile, std::vector<std::string>& lines);
    std::string trim(const std::string& str);
};
