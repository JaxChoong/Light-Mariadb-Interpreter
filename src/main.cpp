#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <fstream>
#include <map>
#include "FileRead.h"  // Include the header file for readFile
#include "FileWrite.h"

using namespace std;

// Global Variables
string currentDatabase;
vector<string> tableNames;
map<string, vector<vector<string>>> databaseTables;  // A map to store tables and their rows
vector<string> databases;  // To keep track of created databases

// Output file stream
ofstream outputFile;

// Function declarations
void parseCommand(const string& command);
void runCLI();
void createFile(const string& filename);
void createDatabase(const string& dbName);
void createTable(const string& tableName, const string& tableSchema);
void insertIntoTable(const string& tableName, const vector<string>& values);
void selectFromTable(const string& tableName);
string trim(const string& str);  // Helper function to trim spaces
string sanitizeSQL(const string& command);  // Function to sanitize the SQL command

// Function to trim leading and trailing spaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to sanitize SQL commands by fixing common issues
string sanitizeSQL(const string& command) {
    string sanitized = command;

    // Remove any unnecessary whitespace inside parentheses (e.g., in CREATE TABLE, INSERT INTO)
    sanitized = regex_replace(sanitized, regex(R"(\s*\(\s*)"), "(");
    sanitized = regex_replace(sanitized, regex(R"(\s*\)\s*)"), ")");

    // Ensure commands end with a semicolon
    if (sanitized.back() != ';') {
        sanitized += ";";
    }

    // Ensure "INSERT INTO" is added if missing for valid commands
    if (sanitized.find("INSERT INTO") == string::npos && sanitized.find("VALUES") != string::npos) {
        size_t tableStart = sanitized.find("(");
        if (tableStart != string::npos) {
            string tableName = sanitized.substr(0, tableStart);
            tableName = trim(tableName); // Remove extra spaces
            sanitized = "INSERT INTO " + tableName + " " + sanitized.substr(tableStart);
        }
    }

    return sanitized;
}


void runCLI() {
    string fileName = "fileInput1.mdb";  // Input file
    fstream file("../Data/" + fileName);  // Open input file
    string line, currentCommand = "";
    outputFile.open("../Data/fileOutput1.txt");  // Open output file

    if (file.is_open() && outputFile.is_open()) {
        while (getline(file, line)) {
            line = trim(line);  // Trim spaces
            if (line.empty()) continue;

            currentCommand += " " + line;  // Concatenate lines

            // Check if a complete command is formed (ends with a semicolon)
            if (currentCommand.find(';') != string::npos) {
                // Sanitize and fix the command before parsing it
                currentCommand = sanitizeSQL(currentCommand);
                outputFile << "Processing sanitized line: " << currentCommand << endl;
                parseCommand(currentCommand);  // Process command
                currentCommand.clear();  // Clear the command buffer
            }
        }
        file.close();
        outputFile.close();
    } else {
        cout << "Error: Unable to open files." << endl;
    }
}

// Function to parse and execute commands
void parseCommand(const string& command) {
    smatch match;

    // Regex patterns for various commands
    regex databaseRegex(R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)");  // Matches CREATE DATABASE command
    regex tableRegex(R"(CREATE\s+TABLE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*?)\)\s*;)");  // Matches CREATE TABLE command
    regex createFileRegex(R"(CREATE\s+([a-zA-Z0-9_]+\.txt)\s*;\s*DATABASES\s*;?)");  // Matches CREATE file command
    regex insertRegex(R"(INSERT\s+INTO\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*?)\)\s*VALUES\s*\((.*?)\)\s*;)");  // Matches INSERT INTO command
    regex selectRegex(R"(SELECT\s+\*\s+FROM\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)");  // Matches SELECT command

    // Command matching using regex
    if (regex_search(command, match, databaseRegex)) {
        string dbName = match[1];
        createDatabase(dbName);
    } else if (regex_search(command, match, tableRegex)) {
        string tableName = match[1];
        string tableSchema = match[2];
        createTable(tableName, tableSchema);
    } else if (regex_search(command, match, createFileRegex)) {
        string fileName = match[1];
        outputFile << "Simulated: Creating file '" << fileName << "' based on the command." << endl;
        createFile(fileName);
        outputFile << "Simulated: DATABASES command executed." << endl;
    } else if (regex_search(command, match, insertRegex)) {
        string tableName = match[1];
        string valuesStr = match[3];
        stringstream ss(valuesStr);
        string value;
        vector<string> values;
        while (getline(ss, value, ',')) {
            values.push_back(trim(value));
        }
        insertIntoTable(tableName, values);
    } else if (regex_search(command, match, selectRegex)) {
        string tableName = match[1];
        selectFromTable(tableName);
    } else {
        outputFile << "Unknown command: " << command << endl;
    }
}

// Function to simulate creating a database
void createDatabase(const string& dbName) {
    outputFile << "Simulated: Creating Database '" << dbName << "'" << endl;
    currentDatabase = dbName;
    databases.push_back(dbName);
    outputFile << "Current Databases: ";
    for (const auto& db : databases) {
        outputFile << db << " ";
    }
    outputFile << endl;
}

// Function to simulate creating a table
void createTable(const string& tableName, const string& tableSchema) {
    outputFile << "Simulated: Creating Table '" << tableName << "' with schema: " << tableSchema << endl;
    tableNames.push_back(tableName);
    databaseTables[tableName] = vector<vector<string>>();  // Initialize table with no rows
    outputFile << "Tables in the current database '" << currentDatabase << "': ";
    for (const auto& table : tableNames) {
        outputFile << table << " ";
    }
    outputFile << endl;
}

// Function to simulate inserting data into a table
void insertIntoTable(const string& tableName, const vector<string>& values) {
    if (databaseTables.find(tableName) != databaseTables.end()) {
        databaseTables[tableName].push_back(values);
        outputFile << "Inserted into table '" << tableName << "': ";
        for (const auto& value : values) {
            outputFile << value << " ";
        }
        outputFile << endl;
    } else {
        outputFile << "Error: Table '" << tableName << "' not found." << endl;
    }
}

// Function to simulate SELECT * FROM a table
void selectFromTable(const string& tableName) {
    outputFile << "Simulated: SELECT * FROM table '" << tableName << "'" << endl;
    if (databaseTables.find(tableName) != databaseTables.end()) {
        outputFile << "Table: " << tableName << endl;
        for (const auto& row : databaseTables[tableName]) {
            for (const auto& col : row) {
                outputFile << col << " ";
            }
            outputFile << endl;
        }
    } else {
        outputFile << "Error: Table '" << tableName << "' does not exist." << endl;
    }
}

// Function to create a file
void createFile(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "This file was created as part of the simulated CREATE command." << endl;
        outFile.close();
        outputFile << "File '" << filename << "' created successfully." << endl;
    } else {
        outputFile << "Error: Could not create file '" << filename << "'." << endl;
    }
}

int main() {
    runCLI();
    return 0;
}
