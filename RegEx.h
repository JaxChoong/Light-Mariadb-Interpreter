#ifndef REGEX_H
#define REGEX_H

// include necessary libraries
#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include "FileManip.h"
#include <stdexcept>   // for runtime_error handling

using namespace std;

// Function prototypes
void process_table_data(const string& create_command, int table_index);
void process_insert_data(const string& insert_command, int table_index);
void process_delete_data(const string& delete_command, int table_index);
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table);
void write_to_file(const vector<string>& lines, const std::string& output_filename); 
void process_update_data(const string& update_command, int table_index);

// global variables
int table_index = -1;
vector<vector<variant<string, vector<variant<int, string>>>>> tables;    // 2d table vector
vector<variant<string, vector<string>>> processed_command_outputs;      // vector to store processed lines, to output to file

// Function to process CREATE commands
vector<string> get_create_type(const string& create_command) {
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command(R"((.*).txt)");

    smatch m; 
    if (regex_search(create_command, m, table_command)) 
    {
        string table_name = m[1].str();

        // Find or add the table name in the tables vector
        for (size_t i = 0; i < tables.size(); i++) 
        {
            if (get<string>(tables[i][0]) == table_name) 
            {
                table_index = i;
                break;
            }
        }

        if (table_index == -1) 
        { // if table name is not found
            tables.push_back({table_name}); // Add the table name as the first entry
                                            // the {} creates a vector
            table_index = tables.size() - 1;
        }

        process_table_data(create_command, table_index);  // process the table data
        return {"table_name", table_name};
    } 
    else if (regex_search(create_command, m, output_file_command)) 
    {
        // detects if the command is to create an output file
        // this function is now useless since our output is in filemanip.h lol
        string output_file_name = m[1].str() + ".txt";    
        return {"output_file_name", output_file_name};
    }
    return {};
}

// Function to processes each line what to do with it
void process_line(const string& line, string current_database) {

    // regex to match the different commands
    regex create_command(R"(CREATE\s+(.*))");
    regex insert_command("(INSERT)(.*)");
    regex select_command("(SELECT)(.*)");
    regex update_command("(UPDATE)(.*)");
    regex delete_command("(DELETE)(.*)");
    regex databases_command("(DATABASES;)");
    regex tables_command("(TABLES;)");

    smatch m;
    if (regex_search(line, m, create_command)) 
    {
        // try and get what type of thing is being created (table/output file)
        vector<string> create_type_vector = get_create_type(m[1].str());
        if (create_type_vector.empty()) 
        {
            processed_command_outputs.push_back("Error: Couldn't find create types.");
            cout << "Error: Couldn't find create types." << endl;
        }
    }

    if (regex_search(line, m, insert_command)) 
    {
        // if insert command is found, process the insert data
        process_insert_data(m[2].str(), table_index);
    }

    if (regex_search(line, m, select_command)) 
    {
        // checks if the select command is "SELECT *" ( npos means not found)
        if (m[2].str().find(" *") != std::string::npos) {
            // if command is 'SELECT *', print the table
            print_table( tables[table_index] );
        } else {
            // if command is SELECT COUNT(*), print the count of the table
            int count = tables[table_index].size() - 2;     // minus 2 to remove header and column names in [0] and [1]
            cout << "Count: " << count << endl;
            processed_command_outputs.push_back("Count: " + to_string(count));
        }
    }

    if (regex_search(line, m, update_command)) 
    {
        // if update command is found, update the data
        process_update_data(line, table_index);
    }

    if (regex_search(line, m, delete_command)) 
    {
        // if delete command is found, process the delete data
        process_delete_data(line, table_index);
    }
    if (regex_search(line, m, databases_command)) {

        char full_path[FILENAME_MAX];    // saves the full path 
        _fullpath(full_path, (current_database).c_str(), FILENAME_MAX);   // gets the full path of the current database
                                                                          // _fullpath gets the full path of the current database
                                                                          // first param is the variable to save the path
                                                                          // second param is database to find
                                                                          // third param is the max length of the path
        string database_path = full_path;
        cout << database_path << endl;
        processed_command_outputs.push_back(database_path);
    }

    if (regex_search(line, m, tables_command)) 
    {
        // if the command is to show the tables, print the table names
        for (const auto& table : tables) 
        {
            //  go through each of the tables and print the table name in the first index
            processed_command_outputs.push_back(get<string>(table[0]));
            cout << get<string>(table[0]) << endl;
        }
    }
}

// Function to processes CREATE commands
void process_table_data(const string& create_command, int table_index) {

    smatch m;
    regex get_table_data(R"(\((.*)$)"); // Matches everything after '('

    if (regex_search(create_command, m, get_table_data)) 
    {
        string table_data = m[1].str();

        // Extract individual columns
        regex column_regex(R"((\w+)\s+(\w+))");  // seperates the column name and the column type by a space
        auto begin = sregex_iterator(table_data.begin(), table_data.end(), column_regex);  //tells the iterator to start at the beginning of the string, and match the regex
        auto end = sregex_iterator();   // tells the iterator to end at the end of the string

        vector<variant<int, string>> table_headers;

        // start iterator(it) in the beginning, and while its not at the end, increment iterator
        for (auto it = begin; it != end; ++it) 
        {
            // add the column name to the table headers
            table_headers.push_back((*it)[1].str());
        }

        // Ensure the table index is initialized
        while (tables.size() <= table_index) 
        {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

        // Add headers to the table
        tables[table_index].push_back(table_headers);

    } 
    else 
    {
        processed_command_outputs.push_back("No table data found");
        cout << "No table data found" << endl;
    }
}

// Function to process INSERT commands
void process_insert_data(const string& insert_command, int table_index) {

    smatch m;
    // Regex to capture everything inside VALUES(...)
    regex get_insert_data(R"(VALUES\s*\((.*)\))");

    if (regex_search(insert_command, m, get_insert_data)) 
    {
        try {
            string insert_data = m[1].str(); // Captures the values inside VALUES(...)

            if (insert_data.empty()) {
                throw runtime_error("Error: No insert data found");
            }

            // Split the captured string into individual values
            regex value_regex(R"('([^']*)'|([^,]+))");
            // start iterator(it) in the beginning, and while its not at the end, increment iterator
        auto begin = sregex_iterator(insert_data.begin(), insert_data.end(), value_regex);
            auto end = sregex_iterator();

            vector<variant<int, string>> table_data;

        // start iterator(it) in the beginning, and while its not at the end, increment iterator
        for (auto it = begin; it != end; ++it) 
        {
            // get the value of the cell
            // if the first group is matched, get the first group, else get the second group
            // this basically means if the first group matches the regex, get the first group, else get the second group
            string value = (*it)[1].matched ? (*it)[1].str() : (*it)[2].str();

            // Check if the value is an integer or a string
            if (!value.empty() && all_of(value.begin(), value.end(), ::isdigit)) 
            {
                table_data.push_back(stoi(value)); // Add as integer
            } 
            else 
            {
                table_data.push_back(value); // Add as string
            }
        }
        cout << endl;

        // Ensure the table index is initialized
        while (tables.size() <= table_index) 
        {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

            // Add data to the table
            tables[table_index].push_back(table_data);

        } catch (const runtime_error& error) {
            processed_command_outputs.push_back(error.what());
            cout << error.what() << endl;
        }
    } 
    else 
    {
        cout << "Error: No insert data found." << endl;
    }
}

// Function to process DELETE commands
void process_delete_data (const string& delete_command, int table_index) {

    smatch m;
    regex get_delete_data(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*['\"]?(\w+)['\"]?)");

    if (regex_search(delete_command, m, get_delete_data)) 
    {
        string table_name = m[1].str();
        string condition = m[2].str();
        string value = m[3].str();
        
        if (table_index == -1) 
        {
            cout << "No table selected" << endl;
            processed_command_outputs.push_back("No table selected");
            return;
        }

        auto& table = tables[table_index];
        auto& headers = get<vector<variant<int, string>>>(table[1]);

        // Find the index of the column to delete from
        int column_index = -1;

        for (size_t i = 0; i < headers.size(); i++) 
        {
            if (get<string>(headers[i]) == condition) 
            {
                column_index = i;
                break;
            }
        }

        if (column_index == -1) 
        {
            cout << "Column not found" << endl;
            processed_command_outputs.push_back("Column not found");
            return;
        }

        size_t initial_size = table.size();
        for (size_t i = table.size() - 1; i > 0; --i) 
        { // Start from the last row and move up
            const auto& row = get<vector<variant<int, string>>>(table[i]);
            // if the row is a string, and the value is the same as the value to delete
            if (holds_alternative<string>(row[column_index])) 
            {
                if (get<string>(row[column_index]) == value) 
                {
                    table.erase(table.begin() + i); // Remove the row
                }
            } 
            // if the row is an integer, and the value is the same as the value to delete
            else if (holds_alternative<int>(row[column_index])) 
            {
                if (to_string(get<int>(row[column_index])) == value) 
                {
                    table.erase(table.begin() + i); // Remove the row
                }
            }
        }

    } 
    else 
    {
        processed_command_outputs.push_back("Invalid DELETE statement.");
        cout << "Invalid DELETE statement." << endl;
    }
}


// Function to print the table (to the console and save into vector to be written)
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table) {
    vector<string> lines;    // saves the table's lines

    for (size_t i = 1; i < table.size(); i++) 
    {

        // Get the row as a vector of integers or strings
        const auto& row = get<vector<variant<int, string>>>(table[i]);
        string line;  // String to store the current row

        for (const auto& cell : row) 
        {
            if (holds_alternative<string>(cell)) 
            { // if cell is a string, then get it as string
                cout << get<string>(cell);
                line += get<string>(cell);
            } 
            else 
            {
                cout << get<int>(cell);   // if cell is an integer, then get it as integer
                line += to_string(get<int>(cell));
            }

            if (&cell != &row.back()) 
            {   // compare the address of the cell with the address of the last cell in the row
                cout << ", "; // Print separator if cell is not the last one
                line += ", ";
            }
        }

        cout << endl;
        lines.push_back(line);
        
    }
    processed_command_outputs.push_back(lines);
}


void process_update_data(const string& update_command, int table_index) {
    smatch m;
    regex get_update_data(R"(UPDATE\s+(\w+)\s+SET\s+(\w+)\s*=\s*['\"]?(\w+)['\"]?\s+WHERE\s+(\w+)\s*=\s*['\"]?(\w+)['\"]?)");

    if (regex_search(update_command, m, get_update_data)) {
        string table_name = m[1].str();
        string column_to_update = m[2].str();
        string new_value = m[3].str();
        string condition = m[4].str();
        string condition_value = m[5].str();

        string error_type;
        string error_condition;

        int update_value_column_index = -1;
        int condition_column_index = -1;
       
        vector<variant<int,string>>table_headers = get<vector<variant<int,string>>>(tables[table_index][1]);
        for (int i = 0; i < table_headers.size(); i++) {
            if (get<string>(table_headers[i]) == column_to_update) {
                update_value_column_index = i;
            }
            if (get<string>(table_headers[i]) == condition) {
                condition_column_index = i;
            }
        }

        if (update_value_column_index == -1 || condition_column_index == -1) {
            error_type = "Error: Column ";
            if (update_value_column_index == -1) {
                error_condition = column_to_update;
            }
            else {
                error_condition = condition;
            } 
            cout << error_type << error_condition << " not found." << endl;
            processed_command_outputs.push_back(error_type + error_condition + " not found.");
            return;
        }

        bool found = false;
        for (int i = 2; i < tables[table_index].size(); i++) {
            auto& row = get<vector<variant<int, string>>>(tables[table_index][i]);
            if (holds_alternative<string>(row[condition_column_index])) {
                if (get<string>(row[condition_column_index]) == condition_value) {
                    if (holds_alternative<string>(row[update_value_column_index])) {
                        row[update_value_column_index] = new_value;
                    }
                    else if (holds_alternative<int>(row[update_value_column_index])) {
                        row[update_value_column_index] = stoi(new_value);
                    }
                    found = true;
                }
            }
            else if (holds_alternative<int>(row[condition_column_index])) {
                if (to_string(get<int>(row[condition_column_index])) == condition_value) {
                    if (holds_alternative<string>(row[update_value_column_index])) {
                        row[update_value_column_index] = new_value;
                    }
                    else if (holds_alternative<int>(row[update_value_column_index])) {
                        row[update_value_column_index] = stoi(new_value);
                    }
                    found = true;
                }
            }
        }
        if (!found)
        {
            error_type = "Error: Condition ";
            error_condition = condition_value;
            cout << error_type << error_condition << " not found."  << endl;
            processed_command_outputs.push_back(error_type + error_condition + " not found.");
        }
    } else {
        cout << "Error: Invalid UPDATE statement." << endl;
        processed_command_outputs.push_back("Error: Invalid UPDATE statement.");
    }
}



#endif
