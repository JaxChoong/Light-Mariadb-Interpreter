// *****************************************************************************************
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC3L
// Tutorial Class : TT5L
// Trimester : 2430
// Member_1 : ID || Choong Jia Xuen || email || Phone
// Member_2 : 242UC244GS || Kee Hui Yee || keehuiyee@student.mmu.edu.my || 016-6240924
// Member_3 : ID || Chong Meng Hang || email || Phone 
// Member_4 : ID || Name || email || Phone

// Task Distribution
// Member_1 :
// Member_2 :
// Member_3 :
// Member_4 :

// *****************************************************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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


int main() {
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        switch (choice) {
            case 1: createDatabase(); break;
            case 2: dropDatabase(); break;
            case 3: createTable(); break;
            case 4:  dropTable(); break;
            case 5: insertData(); break;
            case 6: viewTableCSV(); break;
            case 7: updateData(); break;
            case 8: deleteData(); break;
            case 9: countRows(); break;
            case 10: return 0;
            default: cout << "Invalid choice. Please try again." << endl;
        }
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