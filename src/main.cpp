    // *****************************************************************************************
    // Course: CCP6114 Programming Fundamentals
    // Lecture Class: TC3L
    // Tutorial Class : TT5L
    // Trimester : 2430
    // Member_1 : 242UC244K1 || Choong Jia Xuen || choong.jia.xuen@student.mmu.edu.my || 013-657 3888
    // Member_2 : 242UC244GS || Kee Hui Yee || keehuiyee@student.mmu.edu.my || 016-624 0924
    // Member_3 : 242UC244JT || Chong Meng Hang || chong.meng.hang@student.mmu.edu.my || 016-871 1296
    // Member_4 : 242UC244FW || Jason Hean Qi Shen || jason.hean.qi@student.mmu.edu.my || 019-214 1180

    // Task Distribution
    // Member_1 : FileRead.h, main.cpp, RegEx.h
    // Member_2 : main.cpp, RegEx.h
    // Member_3 : RegEx.h
    // Member_4 :

    // *****************************************************************************************

    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <string>
    #include <experimental/filesystem>
    #include <limits>
    #include "FileManip.h"
    #include "RegEx.h"

    using namespace std;
    namespace fs = std::experimental::filesystem;

    struct Column {
        string name;
        string type;
    };

    struct Table {
        string name;
        vector<Column> columns;
        vector<vector<string>> rows;
    };

    struct Database {
        string name;
        vector<Table> tables;
    };

    Database db;

    void displayMenu();
    void Save_data();
    void createDatabase();
    void dropDatabase();
    void viewDatabase();
    void createTable();
    void dropTable();
    void viewTable();
    void insertData();
    void viewTableCSV();
    void updateData();
    void deleteData();
    void countRows();

    vector<string> get_database_files();
    string choose_database(vector<string> database_files);
    string current_database;

    int main() {
        int choice;
        while (true) {
        displayMenu();
        cin >> choice;
        switch (choice) {
                case 1: createDatabase(); break;
                case 2: dropDatabase(); break;
                case 3: createTable(); break;
                case 4: dropTable(); break;
                case 5: insertData(); break;
                case 6: viewTableCSV(); break;
                case 7: updateData(); break;
                case 8: deleteData(); break;
                case 9: countRows(); break;
                case 10: return 0;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        }
        //set current path to current file directory
        vector<string> database_files = get_database_files();
        current_database = choose_database(database_files);
        current_database = "fileInput1.mdb";
        read_file(current_database);
        return 0;

        try {
            // Get the path of the current executable
            fs::path currentFilePath = fs::current_path();

            // Ensure the "Data" folder exists in the expected location
            fs::path dataPath = currentFilePath / "Data";
            if (!fs::exists(dataPath)) {
                cout << "Error: Data folder not found at " << dataPath << endl;
                return 1;
            }

            // Change to the "Data" folder
            fs::current_path(dataPath);
            cout << "Current working directory set to: " << fs::current_path() << endl;
        } catch (const fs::filesystem_error& e) {
            cerr << "Filesystem error: " << e.what() << endl;
            return 1;
        }

        return 0;
    }

    void displayMenu() {
        cout << "--------------------------------------" << endl;
        cout << "Welcome to the Light Mariadb Interpreter" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Create Database" << endl;
        cout << "2. Drop Database" << endl;
        cout << "3. Create Table" << endl;
        cout << "4. Drop Table" << endl;
        cout << "5. Insert Data" << endl;
        cout << "6. Select Data" << endl;
        cout << "7. Update Data" << endl;
        cout << "8. Delete Data" << endl;
        cout << "9. Count Rows" << endl;
        cout << "10. Exit" << endl;
        cout << "--------------------------------------" << endl;
    }

    void createDatabase() {
        string dbName;
        cout << "Enter the name of the new database: ";
        cin >> dbName;

        string dbPath = "../Data/" + dbName + ".mdb";

        // Ensure the database directory exists
        if (!filesystem::exists("../Data")) {
            filesystem::create_directory("../Data");
        }

        if (filesystem::exists(dbPath)) {
            cout << "Database already exists!" << endl;
            return;
        }

        ofstream dbFile(dbPath);
        if (dbFile) {
            cout << "Database " << dbName << " created successfully." << endl;
            dbFile.close();
        } else {
            cout << "Failed to create database." << endl;
        }
    }

    void viewDatabase() {
        string databaseDirectory = "../Data";

        vector<string> databaseFiles = get_database_files();

        if (databaseFiles.empty()) {
            cout << "No databases found." << endl;
            return;
        }

        cout << "Available Databases:" << endl;
        for (const auto& file : databaseFiles) {
            cout << "- " << file << endl;
        }
    }

    void createTable() {
        string tableName;
        int columnCount;

        cout << "Enter the name of the table: ";
        cin >> tableName;

        cout << "Enter the number of columns (maximum 10): ";
        while (!(cin >> columnCount) || columnCount <= 0 || columnCount > 10) {
            cout << "Invalid number of columns. Please enter a positive integer up to 10: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Table table;
        table.name = tableName;

        for (int i = 0; i < columnCount; ++i) {
            Column column;
            cout << "Enter name for column " << (i + 1) << ": ";
            cin >> column.name;

            cout << "Enter type for column " << (i + 1) << " (INT or TEXT): ";
            while (true) {
                cin >> column.type;
                if (column.type == "INT" || column.type == "TEXT") break;
                cout << "Invalid type. Only INT and TEXT are allowed. Try again: ";
            }
            table.columns.push_back(column);
        }

        // Save the table to the current database file
        if (current_database.empty()) {
            cout << "No database selected. Use 'CREATE DATABASE' first.\n";
            return;
        }

        ofstream dbFile("../Data/" + current_database, ios::app);
        if (!dbFile) {
            cout << "Failed to open database file.\n";
            return;
        }

        dbFile << "CREATE TABLE " << table.name << " (";
        for (size_t i = 0; i < table.columns.size(); ++i) {
            dbFile << table.columns[i].name << " " << table.columns[i].type;
            if (i < table.columns.size() - 1) dbFile << ", ";
        }
        dbFile << ");\n";
        dbFile.close();

        cout << "Table " << tableName << " created successfully.\n";
    }


    void viewTable() {
        string tableName;
        cout << "Enter the name of the table to view: ";
        cin >> tableName;

        // Load table from database file
        ifstream dbFile("../Data/" + current_database);
        if (!dbFile) {
            cout << "Failed to open database file.\n";
            return;
        }

        Table table;
        bool tableFound = false;
        string line;

        while (getline(dbFile, line)) {
            smatch m;
            // Match table structure
            if (regex_search(line, m, regex(R"(CREATE TABLE (\w+)\s*\((.*)\);)"))) {
                if (m[1].str() == tableName) {
                    tableFound = true;
                    table.name = m[1].str();

                    // Parse columns
                    string columns_definition = m[2].str();
                    regex column_regex(R"((\w+)\s+(INT|TEXT))");
                    auto begin = sregex_iterator(columns_definition.begin(), columns_definition.end(), column_regex);
                    auto end = sregex_iterator();
                    for (auto it = begin; it != end; ++it) {
                        table.columns.push_back({(*it)[1].str(), (*it)[2].str()});
                    }
                }
            }
            // Match table rows
            else if (tableFound && regex_search(line, m, regex(R"(INSERT INTO \w+ VALUES \((.*)\);)"))) {
                string values = m[1].str();
                regex value_regex(R"('([^']*)')");
                auto begin = sregex_iterator(values.begin(), values.end(), value_regex);
                auto end = sregex_iterator();
                vector<string> row;
                for (auto it = begin; it != end; ++it) {
                    row.push_back((*it)[1].str());
                }
                table.rows.push_back(row);
            }
        }
        dbFile.close();

        if (!tableFound) {
            cout << "Table " << tableName << " not found.\n";
            return;
        }

        // Display the table
        cout << "Table: " << table.name << endl;
        for (const auto& col : table.columns) {
            cout << col.name << " (" << col.type << ") ";
        }
        cout << endl;

        for (const auto& row : table.rows) {
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }


    void insertData() {
        string tableName;
        cout << "Enter the name of the table to insert data into: ";
        cin >> tableName;

        // Simulated in-memory table structure (replace this with actual table lookup)
        Table table;
        table.name = tableName;

        // Load the table definition from the .mdb file
        // Simulate column definition (replace this with actual file parsing logic)
        table.columns = {{"ID", "INT"}, {"Name", "TEXT"}, {"Age", "INT"}};

        if (table.columns.empty()) {
            cout << "Table does not exist or has no defined columns.\n";
            return;
        }

        vector<string> newRow;
        for (const auto& column : table.columns) {
            string value;
            cout << "Enter value for " << column.name << " (" << column.type << "): ";
            cin >> value;

            // Validate data type
            if (column.type == "INT" && !all_of(value.begin(), value.end(), ::isdigit)) {
                cout << "Error: Value for " << column.name << " must be an integer.\n";
                return;
            }
            newRow.push_back(value);
        }

        // Append to the database file
        ofstream dbFile("../Data/" + current_database, ios::app);
        if (!dbFile) {
            cout << "Failed to open database file.\n";
            return;
        }

        dbFile << "INSERT INTO " << table.name << " VALUES (";
        for (size_t i = 0; i < newRow.size(); ++i) {
            dbFile << "'" << newRow[i] << "'";
            if (i < newRow.size() - 1) dbFile << ", ";
        }
        dbFile << ");\n";
        dbFile.close();

        cout << "Data inserted successfully into table " << table.name << ".\n";
    }


    void viewTableCSV() {
        string tableName;
        cout << "Enter the name of the table to view as CSV: ";
        cin >> tableName;

        // Reuse `viewTable()` logic to load the table
        Table table;
        bool tableFound = false;
        ifstream dbFile("../Data/" + current_database);
        if (!dbFile) {
            cout << "Failed to open database file.\n";
            return;
        }

        string line;
        while (getline(dbFile, line)) {
            smatch m;
            if (regex_search(line, m, regex(R"(CREATE TABLE (\w+)\s*\((.*)\);)"))) {
                if (m[1].str() == tableName) {
                    tableFound = true;
                    table.name = m[1].str();

                    string columns_definition = m[2].str();
                    regex column_regex(R"((\w+)\s+(INT|TEXT))");
                    auto begin = sregex_iterator(columns_definition.begin(), columns_definition.end(), column_regex);
                    auto end = sregex_iterator();
                    for (auto it = begin; it != end; ++it) {
                        table.columns.push_back({(*it)[1].str(), (*it)[2].str()});
                    }
                }
            } else if (tableFound && regex_search(line, m, regex(R"(INSERT INTO \w+ VALUES \((.*)\);)"))) {
                string values = m[1].str();
                regex value_regex(R"('([^']*)')");
                auto begin = sregex_iterator(values.begin(), values.end(), value_regex);
                auto end = sregex_iterator();
                vector<string> row;
                for (auto it = begin; it != end; ++it) {
                    row.push_back((*it)[1].str());
                }
                table.rows.push_back(row);
            }
        }
        dbFile.close();

        if (!tableFound) {
            cout << "Table " << tableName << " not found.\n";
            return;
        }

        // Print the table in CSV format
        for (size_t i = 0; i < table.columns.size(); ++i) {
            cout << table.columns[i].name;
            if (i < table.columns.size() - 1) cout << ",";
        }
        cout << endl;

        for (const auto& row : table.rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                cout << row[i];
                if (i < row.size() - 1) cout << ",";
            }
            cout << endl;
        }
    }



    void updateData() {
        string tableName, columnName, newValue, conditionColumn, conditionValue;

        cout << "Enter the table name: ";
        cin >> tableName;
        cout << "Enter the column name to update: ";
        cin >> columnName;
        cout << "Enter the new value: ";
        cin >> newValue;
        cout << "Enter the condition column: ";
        cin >> conditionColumn;
        cout << "Enter the condition value: ";
        cin >> conditionValue;

        ifstream dbFile("../Data/" + current_database);
        ofstream tempFile("../Data/temp.mdb");
        if (!dbFile || !tempFile) {
            cout << "Error opening files.\n";
            return;
        }

        string line;
        bool tableFound = false;
        vector<string> columns;

        while (getline(dbFile, line)) {
            smatch m;
            // Match and keep track of the table definition
            if (regex_search(line, m, regex(R"(CREATE TABLE (\w+)\s*\((.*)\);)"))) {
                tempFile << line << endl;
                if (m[1].str() == tableName) {
                    tableFound = true;

                    // Extract column names
                    string columns_definition = m[2].str();
                    regex column_regex(R"((\w+)\s+(INT|TEXT))");
                    auto begin = sregex_iterator(columns_definition.begin(), columns_definition.end(), column_regex);
                    auto end = sregex_iterator();
                    for (auto it = begin; it != end; ++it) {
                        columns.push_back((*it)[1].str());
                    }
                }
            } 
            // Update matching rows
            else if (tableFound && regex_search(line, m, regex(R"(INSERT INTO \w+ VALUES \((.*)\);)"))) {
                string values = m[1].str();
                regex value_regex(R"('([^']*)')");
                auto begin = sregex_iterator(values.begin(), values.end(), value_regex);
                auto end = sregex_iterator();

                vector<string> row;
                for (auto it = begin; it != end; ++it) {
                    row.push_back((*it)[1].str());
                }

                // Update matching rows
                auto conditionIndex = find(columns.begin(), columns.end(), conditionColumn);
                auto updateIndex = find(columns.begin(), columns.end(), columnName);
                if (conditionIndex != columns.end() && updateIndex != columns.end()) {
                    size_t condIdx = distance(columns.begin(), conditionIndex);
                    size_t updIdx = distance(columns.begin(), updateIndex);
                    if (row[condIdx] == conditionValue) {
                        row[updIdx] = newValue;
                    }
                }

                // Write updated row
                tempFile << "INSERT INTO " << tableName << " VALUES (";
                for (size_t i = 0; i < row.size(); ++i) {
                    tempFile << "'" << row[i] << "'";
                    if (i < row.size() - 1) tempFile << ", ";
                }
                tempFile << ");\n";
            } 
            else {
                tempFile << line << endl;
            }
        }

        dbFile.close();
        tempFile.close();

        // Replace original file with updated file
        remove(("../Data/" + current_database).c_str());
        rename("../Data/temp.mdb", ("../Data/" + current_database).c_str());

        cout << "Data updated successfully.\n";
    }




    void deleteData() {
        string tableName, conditionColumn, conditionValue;

        cout << "Enter the table name: ";
        cin >> tableName;
        cout << "Enter the condition column: ";
        cin >> conditionColumn;
        cout << "Enter the condition value: ";
        cin >> conditionValue;

        ifstream dbFile("../Data/" + current_database);
        ofstream tempFile("../Data/temp.mdb");
        if (!dbFile || !tempFile) {
            cout << "Error opening files.\n";
            return;
        }

        string line;
        bool tableFound = false;
        vector<string> columns;

        while (getline(dbFile, line)) {
            smatch m;
            // Match and keep track of the table definition
            if (regex_search(line, m, regex(R"(CREATE TABLE (\w+)\s*\((.*)\);)"))) {
                tempFile << line << endl;
                if (m[1].str() == tableName) {
                    tableFound = true;

                    // Extract column names
                    string columns_definition = m[2].str();
                    regex column_regex(R"((\w+)\s+(INT|TEXT))");
                    auto begin = sregex_iterator(columns_definition.begin(), columns_definition.end(), column_regex);
                    auto end = sregex_iterator();
                    for (auto it = begin; it != end; ++it) {
                        columns.push_back((*it)[1].str());
                    }
                }
            } 
            // Write only rows that do not match the condition
            else if (tableFound && regex_search(line, m, regex(R"(INSERT INTO \w+ VALUES \((.*)\);)"))) {
                string values = m[1].str();
                regex value_regex(R"('([^']*)')");
                auto begin = sregex_iterator(values.begin(), values.end(), value_regex);
                auto end = sregex_iterator();

                vector<string> row;
                for (auto it = begin; it != end; ++it) {
                    row.push_back((*it)[1].str());
                }

                // Check condition
                auto conditionIndex = find(columns.begin(), columns.end(), conditionColumn);
                if (conditionIndex != columns.end()) {
                    size_t condIdx = distance(columns.begin(), conditionIndex);
                    if (row[condIdx] != conditionValue) {
                        tempFile << line << endl;
                    }
                }
            } 
            else {
                tempFile << line << endl;
            }
        }

        dbFile.close();
        tempFile.close();

        // Replace original file with updated file
        remove(("../Data/" + current_database).c_str());
        rename("../Data/temp.mdb", ("../Data/" + current_database).c_str());

        cout << "Row(s) deleted successfully.\n";
    }


    void countRows() {
        string tableName;
        cout << "Enter the name of the table to count rows in: ";
        cin >> tableName;

        ifstream dbFile("../Data/" + current_database);
        if (!dbFile) {
            cout << "Failed to open database file.\n";
            return;
        }

        string line;
        size_t rowCount = 0;
        bool tableFound = false;

        while (getline(dbFile, line)) {
            smatch m;
            if (regex_search(line, m, regex(R"(CREATE TABLE (\w+)\s*\((.*)\);)"))) {
                tableFound = (m[1].str() == tableName);
            } 
            else if (tableFound && regex_search(line, m, regex(R"(INSERT INTO \w+ VALUES \((.*)\);)"))) {
                rowCount++;
            }
        }

        dbFile.close();

        if (!tableFound) {
            cout << "Table " << tableName << " not found.\n";
        } else {
            cout << "The table " << tableName << " has " << rowCount << " rows.\n";
        }
    }


    vector<string> get_database_files() {
        vector<string> database_files;
        string databaseDirectory = "../Data";
        for (const auto& entry : filesystem::directory_iterator(databaseDirectory)) {
            if (entry.path().extension() == ".mdb") {
                database_files.push_back(entry.path().filename().string());
            }
        }
        return database_files;
    }

    string choose_database(vector<string> database_files) {
        while (true) {
            cout << "Choose a database by number: " << endl;
            for (size_t i = 0; i < database_files.size(); ++i) {
                cout << i + 1 << ". " << database_files[i] << endl;
            }
            string database;
            cin >> database;
            int index = stoi(database) - 1;
            if (index >= 0 && index < database_files.size()) {
                return database_files[index];
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
