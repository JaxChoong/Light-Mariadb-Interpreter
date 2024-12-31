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

vector<string> get_database_files();
string choose_database(vector<string> database_files);
string current_database;

int main() {
    current_database = "fileInput1.mdb";              // no user input so just put this for now lol
    read_file(current_database);                      // read file (main prgram loop)
    return 0;
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