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
    #include <filesystem>
    #include "FileManip.h"
    #include "RegEx.h"

    using namespace std;

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
        // int choice;
        // while (true) {
        //     displayMenu();
        //     cin >> choice;
        //     switch (choice) {
        //         case 1: createDatabase(choice); break;
        //         case 2: dropDatabase(choice); break;
        //         case 3: createTable(choice); break;
        //         case 4:  dropTable(choice); break;
        //         case 5: insertData(choice); break;
        //         case 6: viewTableCSV(choice); break;
        //         case 7: updateData(choice); break;
        //         case 8: deleteData(choice); break;
        //         case 9: countRows(choice); break;
        //         case 10: return 0;
        //         default: cout << "Invalid choice. Please try again." << endl;
        //     }
        // }
        // set current path to current file directory
        // vector<string> database_files = get_database_files();
        // current_database = choose_database(database_files);
        current_database = "fileInput1.mdb";
        read_file(current_database);
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

    vector<string> databaseFiles = getDatabaseFiles(databaseDirectory);

    if (databaseFiles.empty()) {
        cout << "No databases found." << endl;
        return;
    }

    cout << "Available Databases:" << endl;
    for (const auto& file : databaseFiles) {
        cout << "- " << file << endl;
    }

    void createTable() {
    string tableName;
    int columnCount;

    cout << "Enter the name of the table: ";
    cin >> tableName;

    cout << "Enter the number of columns: ";
    cin >> columnCount;

    Table table;
    table.name = tableName;

    for (int i = 0; i < columnCount; ++i) {
        Column column;
        cout << "Enter name for column " << (i + 1) << ": ";
        cin >> column.name;
        cout << "Enter type for column " << (i + 1) << " (e.g., int, string): ";
        cin >> column.type;
        table.columns.push_back(column);
    }

    cout << "Table " << tableName << " with " << columnCount << " columns created successfully." << endl;
    }

    void viewTable() {
    string tableName;
    cout << "Enter the name of the table to view: ";
    cin >> tableName;

    // Example logic to simulate table content (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.columns = { {"ID", "int"}, {"Name", "string"}, {"Age", "int"} };
    table.rows = { {"1", "Alice", "25"}, {"2", "Bob", "30"}, {"3", "Charlie", "35"} };

    // Display the table
    cout << "Table: " << table.name << endl;
    for (const auto& col : table.columns) {
        cout << col.name << " (" << col.type << ")	";
    }
    cout << endl;

    for (const auto& row : table.rows) {
        for (const auto& cell : row) {
            cout << cell << "	";
        }
        cout << endl;
    }

    void insertData() {
    string tableName;
    cout << "Enter the name of the table to insert data into: ";
    cin >> tableName;

    // Simulated table structure (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.columns = { {"ID", "int"}, {"Name", "string"}, {"Age", "int"} };

    if (table.columns.empty()) {
        cout << "Table does not exist or has no defined columns." << endl;
        return;
    }

    vector<string> newRow;
    for (const auto& column : table.columns) {
        string value;
        cout << "Enter value for " << column.name << " (" << column.type << "): ";
        cin >> value;
        newRow.push_back(value);
    }

    table.rows.push_back(newRow);
    cout << "Data inserted successfully into table " << table.name << "." << endl;
}

    ofstream dbFile(dbPath, ios::app);
    if (!dbFile) {
        cout << "Failed to open database." << endl;
        return;
    }

    string data;
    cout << "Enter the data to insert: ";
    cin.ignore(); // Clear newline character from input buffer
    getline(cin, data);

    dbFile << data << endl;
    cout << "Data inserted into database " << dbName << " successfully." << endl;
    dbFile.close();
    }

    void viewTableCSV() {
    string tableName;
    cout << "Enter the name of the table to view as CSV: ";
    cin >> tableName;

    // Simulated table structure (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.columns = { {"ID", "int"}, {"Name", "string"}, {"Age", "int"} };
    table.rows = { {"1", "Alice", "25"}, {"2", "Bob", "30"}, {"3", "Charlie", "35"} };

    if (table.rows.empty()) {
        cout << "Table is empty or does not exist." << endl;
        return;
    }

    // Print table data as CSV
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


    void updateData() {
    string tableName;
    cout << "Enter the name of the table to update data in: ";
    cin >> tableName;

    // Simulated table structure (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.columns = { {"ID", "int"}, {"Name", "string"}, {"Age", "int"} };
    table.rows = { {"1", "Alice", "25"}, {"2", "Bob", "30"}, {"3", "Charlie", "35"} };

    if (table.rows.empty()) {
        cout << "Table is empty or does not exist." << endl;
        return;
    }

    cout << "Current table data:" << endl;
    for (size_t i = 0; i < table.rows.size(); ++i) {
        cout << i + 1 << ". ";
        for (const auto& cell : table.rows[i]) {
            cout << cell << "	";
        }
        cout << endl;
    }

    size_t rowIndex;
    cout << "Enter the row number to update: ";
    cin >> rowIndex;

    if (rowIndex < 1 || rowIndex > table.rows.size()) {
        cout << "Invalid row number." << endl;
        return;
    }

    vector<string>& rowToUpdate = table.rows[rowIndex - 1];
    for (size_t i = 0; i < table.columns.size(); ++i) {
        string newValue;
        cout << "Enter new value for " << table.columns[i].name << " (" << table.columns[i].type << ") [Current: " << rowToUpdate[i] << "]: ";
        cin >> newValue;
        rowToUpdate[i] = newValue;
    }

    cout << "Row updated successfully." << endl;
    }

    void deleteData() {
`   string tableName;
    cout << "Enter the name of the table to delete data from: ";
    cin >> tableName;

    // Simulated table structure (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.columns = { {"ID", "int"}, {"Name", "string"}, {"Age", "int"} };
    table.rows = { {"1", "Alice", "25"}, {"2", "Bob", "30"}, {"3", "Charlie", "35"} };

    if (table.rows.empty()) {
        cout << "Table is empty or does not exist." << endl;
        return;
    }

    cout << "Current table data:" << endl;
    for (size_t i = 0; i < table.rows.size(); ++i) {
        cout << i + 1 << ". ";
        for (const auto& cell : table.rows[i]) {
            cout << cell << "	";
        }
        cout << endl;
    }

    size_t rowIndex;
    cout << "Enter the row number to delete: ";
    cin >> rowIndex;

    if (rowIndex < 1 || rowIndex > table.rows.size()) {
        cout << "Invalid row number." << endl;
        return;
    }

    table.rows.erase(table.rows.begin() + rowIndex - 1);
    cout << "Row deleted successfully." << endl;
    }

    void countRows() {
    string tableName;
    cout << "Enter the name of the table to count rows in: ";
    cin >> tableName;

    // Simulated table structure (replace with actual implementation)
    Table table;
    table.name = tableName;
    table.rows = { {"1", "Alice", "25"}, {"2", "Bob", "30"}, {"3", "Charlie", "35"} };

    cout << "The table " << tableName << " has " << table.rows.size() << " rows." << endl;
    }

    vector<string> get_database_files() {
        vector<string> database_files;
        filesystem::current_path(filesystem::path(__FILE__).parent_path());
        for (const auto& entry : filesystem::directory_iterator("../Data")) {
            database_files.push_back(entry.path().filename().string());
        }
        cout << "Databases found:" << endl;
        int i = 0;
        for (const auto& file : database_files) {
            cout << "- "<< file << " ( Select with " << to_string(i) << " )"<< endl;
            i++;
        }
        return database_files;
    }

    string choose_database( vector<string> database_files) {
        string database;
        while (true) {
            cout << "Choose a database: ";
            cin >> database;
            if (stoi(database) < database_files.size()) {
                cout << "Selected database: " << database_files[stoi(database)] << endl <<endl;
                return database_files[stoi(database)];
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
