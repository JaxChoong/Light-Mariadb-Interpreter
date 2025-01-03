    // *****************************************************************************************
    // Course: CCP6114 Programming Fundamentals
    // Lecture Class: TC3L
    // Tutorial Class : TT5L
    // Trimester : 2430
    // Member_1 : 242UC244K1 || Choong Jia Xuen || choong.jia.xuen@student.mmu.edu.my || 013-657 3888
    // Member_2 : 242UC244GS || Kee Hui Yee || keehuiyee@student.mmu.edu.my || 016-624 0924
    // Member_3 : 242UC244JT || Chong Meng Hang || chong.meng.hang@student.mmu.edu.my || 016-871 1296 
    // Member_4 : 242UC244FW || Hean Qi Shen || jason.hean.qi@student.mmu.edu.my || 019-214 1180

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
        
    }

    void viewDatabase() {

    }

    void createTable() {
        
    }

    void viewTable() {

    }

    void insertData() {

    }

    void viewTableCSV() {

    }

    void updateData() {
        
    }

    void deleteData() {

    }

    void countRows() {

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