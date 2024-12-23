 #ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

using namespace std;

vector<variant<int, string>> Data;

void Save_Data() {
    Data.push_back(2);
    Data.push_back("Jolly");
    Data.push_back(2);
    Data.push_back("Kim");
    
    for (int i = 0; Data.size(); i++) {
        // idk man;
    }
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

#endif