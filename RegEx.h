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
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table);
void add_table_headers(const vector<string>& table_headers);
string process_line(const string& line, string current_database);
vector<string> get_create_type(const string& create_command);
void process_insert_data(const string& insert_command);
void process_delete_data(const string& delete_command);
void process_update_data(const string& update_command);
void write_to_file(const vector<string>& lines, const std::string& output_filename);


// global variables
vector<variant<string, vector<variant<int, string>>>> tables;    // 2d table vector
vector<variant<string, vector<string>>> processed_command_outputs;      // vector to store processed lines, to output to file
string output_filename;



// Function to processes each line what to do with it
string process_line(const string& line, string current_database) {

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
        else if (create_type_vector[0] == "table_name")
        {
            return "table created";
        }
    }

    if (regex_search(line, m, insert_command))
    {
        // if insert command is found, process the insert data
        process_insert_data(m[2].str());
    }

    // This is basically the SELECT function
    if (regex_search(line, m, select_command))
    {
        // checks if the select command is "SELECT *" ( npos means not found)
        if (m[2].str().find(" *") != std::string::npos) {
            // if command is 'SELECT *', print the table
            print_table( tables );
        } else {
            // if command is SELECT COUNT(*), print the count of the table
            int count = tables.size() - 2;     // minus 2 to remove header and column names in [0] and [1]
            cout << count << endl;
            processed_command_outputs.push_back("Count: " + to_string(count));
        }
    }

    if (regex_search(line, m, update_command))
    {
        // if update command is found, update the data
        process_update_data(line);
    }

    if (regex_search(line, m, delete_command))
    {
        // if delete command is found, process the delete data
        process_delete_data(line);
    }
    if (regex_search(line, m, databases_command)) {

        char full_path[FILENAME_MAX];    // creates char array to store the full path
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
        processed_command_outputs.push_back(get<string>(tables[0]));
        cout << get<string>(tables[0]) << endl;
    }
    return "";
}



// Function to process CREATE commands
vector<string> get_create_type(const string& create_command) {
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command(R"((.*).txt)");

    smatch m;
    if (regex_search(create_command, m, table_command))
    {
        string table_name = m[1].str();


        tables.push_back(table_name); // Add the table name as the first entry
        return {"table_name", table_name};
    }
    else if (regex_search(create_command, m, output_file_command))
    {
        output_filename = m.str();  // set the output filename to the output file name
        return {"output_file_name", output_filename};
    }
    return {};
}

// Function to process INSERT commands
void process_insert_data(const string& insert_command) {

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
                table_data.push_back(stoi(value));
            }
            else
            {
                table_data.push_back(value);
            }
        }
        cout << endl;

            // Add data to the table
            tables.push_back(table_data);

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


// Function to process UPDATE commands
void process_update_data(const string& update_command) {
    smatch m;
    // Enhanced regex to parse the UPDATE command with optional whitespace handling
    regex update_pattern(R"(UPDATE\s+(\w+)\s+SET\s+([^WHERE]+)\s+WHERE\s+(.+))");

    if (regex_search(update_command, m, update_pattern)) {
        string table_name = m[1].str(); // Extract the table name
        string set_clause = m[2].str(); // Extract the SET clause
        string condition = m[3].str(); // Extract the WHERE clause

        // Validate table existence and structure
        if (tables.empty() || tables.size() < 2) {
            cout << "Error: Table data is invalid or missing." << endl;
            processed_command_outputs.push_back("Error: Table data is invalid or missing.");
            return;
        }

        // Extract table headers
        vector<variant<int, string>> table_headers = get<vector<variant<int, string>>>(tables[1]);

        // Parse the SET clause for column-value pairs
        regex set_clause_pattern(R"((\w+)\s*=\s*['\"]?([^,'\"]+)['\"]?)");
        auto set_begin = sregex_iterator(set_clause.begin(), set_clause.end(), set_clause_pattern);
        auto set_end = sregex_iterator();

        vector<pair<int, string>> updates; // Store updates as column index and new value pairs
        for (auto it = set_begin; it != set_end; ++it) {
            string column = (*it)[1].str();
            string new_value = (*it)[2].str();
            int column_index = -1;

            // Find the column index for the update
            for (int i = 0; i < table_headers.size(); ++i) {
                if (get<string>(table_headers[i]) == column) {
                    column_index = i;
                    break;
                }
            }

            if (column_index == -1) {
                cout << "Error: Column " << column << " not found." << endl;
                processed_command_outputs.push_back("Error: Column " + column + " not found.");
                return;
            }
            updates.push_back({column_index, new_value});
        }

        // Parse the WHERE clause for the condition
        regex condition_pattern(R"((\w+)\s*=\s*['\"]?(\w+)['\"]?)");
        smatch condition_match;
        if (!regex_search(condition, condition_match, condition_pattern)) {
            cout << "Error: Invalid WHERE clause." << endl;
            processed_command_outputs.push_back("Error: Invalid WHERE clause.");
            return;
        }

        string condition_column = condition_match[1].str();
        string condition_value = condition_match[2].str();
        int condition_index = -1;

        // Find the condition column index
        for (int i = 0; i < table_headers.size(); ++i) {
            if (get<string>(table_headers[i]) == condition_column) {
                condition_index = i;
                break;
            }
        }

        if (condition_index == -1) {
            cout << "Error: Condition column " << condition_column << " not found." << endl;
            processed_command_outputs.push_back("Error: Condition column " + condition_column + " not found.");
            return;
        }

        // Apply updates to rows matching the condition
        bool found = false;
        for (size_t i = 2; i < tables.size(); ++i) {
            auto& row = get<vector<variant<int, string>>>(tables[i]);

            // Match the condition
            if ((holds_alternative<string>(row[condition_index]) && get<string>(row[condition_index]) == condition_value) ||
                (holds_alternative<int>(row[condition_index]) && to_string(get<int>(row[condition_index])) == condition_value)) {

                // Apply updates
                for (const auto& [col_index, new_value] : updates) {
                    if (holds_alternative<string>(row[col_index])) {
                        row[col_index] = new_value;
                    } else if (holds_alternative<int>(row[col_index])) {
                        row[col_index] = stoi(new_value);
                    }
                }
                found = true;
            }
        }

        if (!found) {
            cout << "Error: No rows match the condition." << endl;
            processed_command_outputs.push_back("Error: No rows match the condition.");
        }
    } else {
        cout << "Error: Invalid UPDATE command." << endl;
        processed_command_outputs.push_back("Error: Invalid UPDATE command.");
    }
}




// Function to process DELETE commands
void process_delete_data (const string& delete_command) {

    smatch m;
    regex get_delete_data(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*['\"]?(\w+)['\"]?)");

    if (regex_search(delete_command, m, get_delete_data))
    {
        string table_name = m[1].str();
        string condition = m[2].str();
        string value = m[3].str();

        auto& headers = get<vector<variant<int, string>>>(tables[1]);

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
            cout << "Error: Column " << condition << " not found!" << endl;
            processed_command_outputs.push_back("Column not found");
            return;
        }

        size_t initial_size = tables.size();
        for (size_t i = tables.size() - 1; i > 0; --i)
        { // Start from the last row and move up
            const auto& row = get<vector<variant<int, string>>>(tables[i]);
            // if the row is a string, and the value is the same as the value to delete
            if (holds_alternative<string>(row[column_index]))
            {
                if (get<string>(row[column_index]) == value)
                {
                    tables.erase(tables.begin() + i); // Remove the row
                }
            }
            // if the row is an integer, and the value is the same as the value to delete
            else if (holds_alternative<int>(row[column_index]))
            {
                if (to_string(get<int>(row[column_index])) == value)
                {
                    tables.erase(tables.begin() + i); // Remove the row
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
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table)
{
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



// Function to add table headers to be used in FileManip.h
void add_table_headers(const vector<string>& table_headers)
{
    vector<variant<int, string>> headers;
    for (const auto& header : table_headers) {
        headers.push_back(header);
    }
    tables.push_back(headers);
}

#endif
