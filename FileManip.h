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
#include <regex>
#include <string>

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


// Function to read a file
void read_file(const string& filename) {
    // get the current path to the directory
    filesystem::current_path(filesystem::path(__FILE__).parent_path());
    const string file_path = filename;
    fstream file(file_path);

    if (file.is_open()) 
    {
        string status;
        string line;
        vector<string> table_headers; // To store column names
        bool in_create_table_block = false; // Flag for CREATE TABLE block
        regex column_regex(R"((\w+)\s+\w+)"); // Matches column definitions like 'column_name TYPE'

        while (getline(file, line)) 
        {
            if (!line.empty()) 
            {
                // Process the line
                if (!in_create_table_block) {
                    // if not in CREATE TABLE block, then add a ">" to the line
                    processed_command_outputs.push_back("> " + line);
                    cout << "> " << line << endl;
                    status = process_line(line, filename);

                    // Add the line to processed_command_outputs
                    // If "table created" is returned, start extracting columns
                    if (status == "table created") {
                        in_create_table_block = true;
                    }
                } 
                else {
                    // Extract columns while in CREATE TABLE block
                    cout << line << endl;
                    processed_command_outputs.push_back(line);
                    smatch match;
                    if (regex_search(line, match, column_regex)) {
                        table_headers.push_back(match[1].str()); // Add column name to headers
                    }

                    // Check for end of CREATE TABLE block
                    if (line.find(");") != string::npos) {
                        // if end of the coloumn def, then set it to false and quit this loop
                        in_create_table_block = false;

                        // Add the table headers to the tables vector
                        add_table_headers(table_headers);
                    }
                }
            } 
            else {
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