// *****************************************************************************************
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC3L
// Tutorial Class : TT5L
// Trimester : 2430
// Member_1 : 242UC244K1 || Choong Jia Xuen || choong.jia.xuen@student.mmu.edu.my || 013-657 3888
// Member_2 : 242UC244GS || Kee Hui Yee || keehuiyee@student.mmu.edu.my || 016-624 0924
// Member_3 : 242UC244JT || Chong Meng Hang || chong.meng.hang@student.mmu.edu.my || 016-871 1296 
// Member_4 : ID || Hean Qi Shen || email || Phone

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
#include <filesystem>
#include "FileRead.h"
#include <mariadb/conncpp.hpp>


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
    try {
        sql::SQLString url();
        sql::Properties properties({
            {"user","username:"};
            {"password","password:"};
            {"database","dbname:"};
        });
    };

    sql::Driver* driver = sql::mariadb::get_driver_instance();
    
    std::unique_ptr<sql::Connection> connection(driver ->connect(url,porperties));
    
    if (connection ->isValid()) {
        cout << "connect successsfully" <<endl;

        unique_ptr<sql::Statement> stmt(connection ->createStatement());
        stmt ->execute("CREATE TABLE IF NOT EXEISTS example (id INT PRIMARY KEY,name VARCHAR(255))");
        cout << "table created and exists" << endl;
    }else{
        cerr << "connection failed" << endl;
    }
    }catch (sql::SQLException &e) {
        cerr << "error: " << e.what() << endl;
        retuen 1;
    }
    return 0;

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
    cin >> choice;
}

// void createDatabase() {
    
// }

// void viewDatabase() {

// }

// void createTable() {
    
// }

// void viewTable() {

// }

// void insertData() {

// }

// void viewTableCSV() {

// }

// void updateData() {
    
// }

// void deleteData() {

// }

// void countRows() {

// } 

void listTables(sql::Connection& connnection) {
    try {
        cout << "Listing tables..." << endl;
        unique_prt<sql::Statement> stmt(connection.createStatement());
        unique_ptr<sql::ResultSet> res(stmt ->executeQuery("SHOW TABLES"));

        int cout = 0;
        while (res -> next()) {
            cout << ++count << ". " << res -> getString(1) << endl;
        }

    if (count == 0) {
        cout << "No tables found" << endl;
    }
    }catch (sql::SQLException &e) {
        cerr << "error: " << e.what() << endl;
    }

void describeTable(sql::Connection& connection, const string& table_name) {
    try {
        unique_prt<sql::Statement> stmt(connection.createStatement());
        unique_prt<sql::ResultSet> res(stmt ->executeQuery("DESCRIBE " + table_name));

        cout << "table " << tablename << "stucture" << endl;
        cout << "field\ttype\tNULL\tkey\tdefault\textra" << endl;
        while (rest->next()){
            cout << res -> getString("Field")<< "\t";
            cout << res -> getString("Type")<< "\t";
            cout << res -> getString("Null")<< "\t";
            cout << res -> getString("Key")<< "\t";
            cout << res -> getString("Default")<< "\t";
            cout << res -> getString("Extra")<< "\t";
            cout << endl;
        }
    }catch (sql::SQLException &e) {
        cerr << "error: " << e.what() << endl;
    }

void runCIL(sql::Connection& connection){
    string choice;
    while(True){
        displayMenu();
        
        if (choice == 1 ){
            listTables(connection);
        }else if (choice == 2){
            cout << "Enter table name:";
            string tableName;
            cin >> tableName;
            describeTable(connection,tableName);
        }else if (choice == 3){
            break;
            
        }else if (choice == 4){
                cout << "Invalid choice, Enter again" << endl;
            }
          }
}
      
}
}