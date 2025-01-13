 #ifndef FILEREAD_H
#define FILEREAD_H

// include necessary libraries
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "RegEx.h"
#include <variant>
#include <vector>

using namespace std;

// Function prototypes
void write_to_file(const vector<string>& lines, const std::string& output_filename);
extern string output_filename; // output filename
extern vector<variant<string,vector<string>>> processed_command_outputs;   // use this to store lines to write to file, "extern" allows variable to be used in other files

// Function to write to a file
void write_to_file(const vector<variant<string, vector<string>>>& lines) {

    const string output_path = output_filename;
    ofstream output_file(output_path);

    if (output_file.is_open()) 
    {
        for (const auto& line : lines) 
        {
            if (holds_alternative<string>(line)) 
            {
                // Write single-line string
                output_file << get<string>(line) << endl;
            } 
            else if (holds_alternative<vector<string>>(line)) 
            {
                // Write each subline in the vector
                for (const auto& subline : get<vector<string>>(line)) 
                {
                    output_file << subline << endl;
                }
            }
        }
        output_file.close();
    } 
    else 
    {
        cout << "Could not open file for writing: " << output_path << endl;
    }
}

// Function to get the output filename
string get_output_filename(const string& filename) {

    size_t lastindex = filename.find_last_of(".");     // gets position of the . character
    string name_without_extension = filename.substr(0, lastindex);    // gets the filename without the extension ( .mdb)
    cout << name_without_extension << endl;
    size_t pos = name_without_extension.find("Input");  // gets the position of the "Input" string as substring
    
    if (pos != string::npos) 
    {
        name_without_extension.replace(pos, 5, "Output");     // if found, replace "Input" with "Output"
    }

    return name_without_extension + ".txt";
}

// Function to read a file
void read_file(const string& filename) {

    filesystem::current_path(filesystem::path(__FILE__).parent_path());
    const string file_path = filename;
    fstream file(file_path);

    if (file.is_open()) 
    {
        string line;
        while (getline(file, line)) 
        {
            if (!line.empty()) 
            {
                // Add line to processed_command_outputs as a single string
                processed_command_outputs.push_back("> " + line);
                cout << "> " << line << endl;            
                // Process the command and handle any multi-line output internally
                process_line(line, filename);
            }
            else{
                processed_command_outputs.push_back(line);
                cout << line << endl;
            }

        }
        file.close();

        // Save the output to a file
        write_to_file(processed_command_outputs);

    } 
    else 
    {
        cout << "File not found" << endl;
    }
}


#endif // FILEREAD_H