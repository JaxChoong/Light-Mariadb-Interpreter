#ifndef FILEREAD_H
#define FILEREAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

void readFile(const std::string& filename) {
    const string file_path = "../Data/" + filename;  // Path to the file
    cout << "Reading file: " << file_path << endl;
    
    // Open the file using fstream
    fstream file(file_path);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;  // Print each line from the file
        }
        file.close();
    } else {
        cout << "File not found" << endl;  // Handle the case where the file does not exist
    }
}

#endif // FILEREAD_H
