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

// Function to trim leading and trailing spaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to run the Command Line Interface
void runCLI() {
    // Define the file name with the correct path relative to the "Data" directory
    string fileName = "fileInput1.mdb";  // Specify the file to read from
    
    // Read the file using the readFile function (prints contents)
    readFile(fileName);  // Make sure this function is correctly implemented to print the file

    // Now open the file for processing and parsing commands
    fstream file("../Data/" + fileName);  // Open the file for processing
    string line;

    // Open the output file for writing the results
    outputFile.open("../Data/fileOutput1.txt");  // Specify the output file

    if (file.is_open() && outputFile.is_open()) {
        string currentCommand = "";

        // Read each line from the file and process it
        while (getline(file, line)) {
            line = trim(line);  // Trim spaces

            if (line.empty()) continue;  // Skip empty lines

            currentCommand += " " + line;  // Concatenate lines for multi-line commands

            // Check if the command ends with a semicolon, indicating it's complete
            if (currentCommand.find(";") != string::npos) {
                outputFile << "Processing line: " << currentCommand << endl;
                // Parse the command from the line
                parseCommand(currentCommand);  // Parse the complete command
                currentCommand.clear();  // Clear for the next command
            }
        }
        file.close();
        outputFile.close();  // Close the output file after processing
    } else {
        cout << "Failed to open the file: " << fileName << endl;
    }
}

// Function to parse and simulate the execution of CREATE DATABASE, CREATE TABLE, INSERT INTO, and SELECT commands
void parseCommand(const string& command) {
    smatch match;

    // Regular expression to match CREATE DATABASE command
    regex databaseRegex(R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)");

    // Regular expression to match CREATE TABLE command (allow for spaces/newlines between parameters)
    regex tableRegex(R"(CREATE\s+TABLE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*?)\)\s*;)");

    // Regex to match the CREATE fileOutput1.txt; DATABASES; pattern with flexible spaces and optional semicolons
    regex createFileRegex(R"(CREATE\s+([a-zA-Z0-9_]+\.txt)\s*;\s*DATABASES\s*;?)");

    // Regular expression for INSERT INTO command (allow for spaces/newlines between parameters)
    regex insertRegex(R"(INSERT\s+INTO\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*?)\)\s*VALUES\s*\((.*?)\)\s*;)");

    // Regular expression for SELECT * FROM command
    regex selectRegex(R"(SELECT\s+\*\s+FROM\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)");

    // Check if the command is a CREATE DATABASE command
    if (regex_search(command, match, databaseRegex)) {
        string dbName = match[1];  // Extract the database name
        createDatabase(dbName);  // Simulate creating the database
    } 
    // Check if the command is a CREATE TABLE command
    else if (regex_search(command, match, tableRegex)) {
        string tableName = match[1];  // Extract the table name
        string tableSchema = match[2];  // Extract the table schema (column names and types)
        createTable(tableName, tableSchema);  // Simulate creating the table
    } 
    // Check if the command matches the CREATE fileOutput1.txt; DATABASES; pattern
    else if (regex_search(command, match, createFileRegex)) {
        string fileName = match[1];  // Extract the file name (e.g., fileOutput1.txt)
        outputFile << "Simulated: Creating file '" << fileName << "' based on the command." << endl;

        // Create the file
        createFile(fileName);

        outputFile << "Simulated: DATABASES command executed." << endl;
    } 
    // Check if the command matches the INSERT INTO command
    else if (regex_search(command, match, insertRegex)) {
        string tableName = match[1];  // Extract the table name
        string valuesStr = match[3];  // Extract the values
        stringstream ss(valuesStr);
        string value;
        vector<string> values;

        // Parse the comma-separated values
        while (getline(ss, value, ',')) {
            values.push_back(trim(value));  // Ensure values are trimmed of extra spaces
        }
        insertIntoTable(tableName, values);  // Simulate inserting data into the table
    }
    // Check if the command matches the SELECT * FROM command
    else if (regex_search(command, match, selectRegex)) {
        string tableName = match[1];  // Extract the table name
        selectFromTable(tableName);  // Simulate selecting data from the table
    }
    // If the command doesn't match the patterns
    else {
        outputFile << "Unknown command." << endl;
    }
}

// Function to simulate creating a database
void createDatabase(const string& dbName) {
    outputFile << "Simulated: Creating Database '" << dbName << "'" << endl;
    // Store the database name in the global variable and the databases vector
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
    // Store the table name in the tableNames vector and initialize it in the databaseTables map
    tableNames.push_back(tableName);
    databaseTables[tableName] = vector<vector<string>>();  // Initialize an empty table (vector of rows)
    outputFile << "Tables in the current database '" << currentDatabase << "': ";
    for (const auto& table : tableNames) {
        outputFile << table << " ";
    }
    outputFile << endl;
}

// Function to simulate inserting data into a table
void insertIntoTable(const string& tableName, const vector<string>& values) {
    outputFile << "Simulated: Inserting data into table '" << tableName << "' with values: ";
    for (const auto& value : values) {
        outputFile << value << " ";
    }
    outputFile << endl;

    // Insert the values into the corresponding table
    if (databaseTables.find(tableName) != databaseTables.end()) {
        databaseTables[tableName].push_back(values);
    } else {
        outputFile << "Error: Table '" << tableName << "' does not exist." << endl;
    }
}

// Function to simulate SELECT * FROM a table
void selectFromTable(const string& tableName) {
    outputFile << "Simulated: SELECT * FROM table '" << tableName << "'" << endl;

    // Retrieve and display the rows of the table
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

// Function to create a file based on the command (simulated)
void createFile(const string& filename) {
    ofstream outFile(filename);  // Create the file
    if (outFile.is_open()) {
        outFile << "This file was created as part of the simulated CREATE command." << endl;
        outFile.close();
        outputFile << "File '" << filename << "' created successfully." << endl;
    } else {
        outputFile << "Error: Could not create file '" << filename << "'." << endl;
    }
}

int main() {
    runCLI();  // Start running the CLI simulation
    return 0;
}
