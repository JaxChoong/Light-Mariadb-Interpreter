#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include "FileRead.h"
#include "FileWrite.h"

using namespace std;

// Global Variables
string currentDatabase;
vector<string> tableNames;

// Function to display menu for the CLI
void displayMenu() {
    cout << "--------------------------------------" << endl;
    cout << "Welcome to the MariaDb Interpreter" << endl;
    cout << "--------------------------------------" << endl;
    cout << "1. Start" << endl;
    cout << "2. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Enter your choice: ";
}

// Function to run the Command Line Interface
void runCLI() {
    string choice;
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear buffer

        if (choice == "1") {
            // Ask the user for the file to read inside the CLI
            string fileName;
            cout << "Enter the file name to read (e.g., example.mdb): ";
            getline(cin, fileName);  // Get the file name from the user

            // Read from the file and write its content to another file
            readFile(fileName);  // Read content from the file (for demonstration)
            
            // FileWrite class to handle writing each line to the output file
            FileWrite fileWriter;
            fstream file(fileName);
            string line;

            if (file.is_open()) {
                while (getline(file, line)) {
                    fileWriter.writeFile(line, "output.txt");  // Write each line to the output file
                }
                file.close();
            } else {
                cout << "Failed to open the file: " << fileName << endl;
            }

            cout << "Enter your command: ";
            string command;
            getline(cin, command);
            parseCommand(command);  // Parse and simulate the command
        } else if (choice == "2") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to parse and simulate the execution of commands
void parseCommand(const string& command) {
    // CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;
    // CREATE\s+DATABASE\s+ - matches the CREATE DATABASE part of the command
    // ([a-zA-Z_][a-zA-Z0-9_]*) - matches the database name (alphanumeric and underscore, starting with a letter)
    // \s*; - matches any whitespace characters and a semicolon at the end
    regex databaseRegex(R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)"); 
    regex tableRegex(R"(CREATE\s+TABLE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\()");
    smatch match;

    if (regex_search(command, match, databaseRegex)) {
        cout << "Simulated: Creating Database '" << match[1] << "'" << endl;  // Simulate creating a database
    } else if (regex_search(command, match, tableRegex)) {
        cout << "Simulated: Creating Table '" << match[1] << "'" << endl;  // Simulate creating a table
    } else if (command.find("INSERT INTO") == 0) {
        cout << "Simulated: INSERT INTO is not implemented." << endl;
    } else if (command == "SELECT * FROM customer") {
        cout << "Simulated: SELECT functionality is not implemented." << endl;
    } else {
        cout << "Unknown command." << endl;
    }
}

// Function to create a database (simulated)
void createDatabase(const string& name) {
    currentDatabase = name;
    cout << "Database '" << name << "' created." << endl;
}

// Function to create a table (simulated)
void createTable(const string& name) {
    tableNames.push_back(name);
    cout << "Table '" << name << "' created." << endl;
}

int main() {
    // Start the CLI (no execution of commands, just simulation)
    runCLI();
    return 0;
}
