 #ifndef FILEREAD_H
#define FILEREAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "RegEx.h"

using namespace std;

void write_to_file(const vector<string>& lines, const std::string& output_filename); // Function prototype

void read_file(const std::string& filename){
    filesystem::current_path(filesystem::path(__FILE__).parent_path());
    const string file_path = "../Data/" + filename;
    cout << "Reading file: " << file_path << endl;
    cout << "--------------------------------------" << endl <<endl;
    fstream file(file_path);
    if (file.is_open()) {
        string line;
        vector<string> lines;
        while (getline(file, line)) {
            if (!line.empty()){
                cout << "> ";
                lines.push_back(">" + line);
            }
            cout << line << endl;
            //lines.push_back(line);
            process_line(line, filename);
        }
        file.close();

        write_to_file(lines, "output.txt");  // Use a prefixed output filename
    } else {
        cout << "File not found" << endl;
    }
}

void write_to_file(const vector<string>& lines, const std::string& output_filename) {
    const string output_path = "../Data/" + output_filename;
    fstream output_file(output_path, ios::out | ios::trunc);

    if (output_file.is_open()) {
        for (const auto& line : lines) {
            output_file << line << endl;  // Write each line to the output file
        }
        output_file.close();
        cout << "Output written to: " << output_path << endl;
    } else {
        cout << "Could not open file for writing: " << output_path << endl;
    }
}

#endif // FILEREAD_H