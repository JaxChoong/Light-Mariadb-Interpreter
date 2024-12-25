 #ifndef FILEREAD_H
#define FILEREAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "RegEx.h"

using namespace std;

void read_file(const std::string& filename){
    filesystem::current_path(filesystem::path(__FILE__).parent_path());
    const string file_path = "../Data/" + filename;
    cout << "Reading file: " << file_path << endl;
    cout << "--------------------------------------" << endl <<endl;
    fstream file(file_path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()){
                cout << "> ";
            }
            cout << line << endl;
            process_line(line, filename);
        }
        file.close();
    } else {
        cout << "File not found" << endl;
    }
}

#endif // FILEREAD_H