#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include "FileManip.h"

using namespace std;

void process_table_data(const string& create_command, int table_index);
vector<vector<variant<string, vector<variant<int, string>>>>> tables;
void process_insert_data(const string& insert_command, int table_index);
void process_delete_data(const string& Delete, int table_index);
int table_index = -1;
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table);
void write_to_file(const vector<string>& lines, const std::string& output_filename); // Function prototype

vector<string> get_create_type(const string& create_command) {
    regex database_command(R"(DATABASE\s+(\w+))");
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command(R"((.*).txt)");

    smatch m;
    if (regex_search(create_command, m, database_command)) {
        // detects if the command is to create a database
        cout << "Create database" << endl;
        string database_name = m[1].str();
        return {"database_name", database_name};
    } 
    else if (regex_search(create_command, m, table_command)) {
        string table_name = m[1].str();

        // Find or add the table name in the tables vector
        for (size_t i = 0; i < tables.size(); i++) {
            if (get<string>(tables[i][0]) == table_name) {
                table_index = i;
                break;
            }
        }

        if (table_index == -1) { // if table name is not found
            tables.push_back({table_name}); // Add the table name as the first entry
            table_index = tables.size() - 1;
        }

        process_table_data(create_command, table_index); 
        return {"table_name", table_name};
    } 
    else if (regex_search(create_command, m, output_file_command)) {
        // detects if the command is to create an output file
        string output_file_name = m[1].str() + ".txt";    // for now no actual output file creation, just for demonstration
        return {"output_file_name", output_file_name};
    }
    return {};
}

void process_line(const string& line, string current_database) {
    regex create_command(R"(CREATE\s+(.*))");
    regex drop_command("(DROP)(.*)");
    regex insert_command("(INSERT)(.*)");
    regex select_command("(SELECT)(.*)");
    regex update_command("(UPDATE)(.*)");
    regex delete_command("(DELETE)(.*)");
    regex databases_command("(DATABASES;)");
    regex tables_command("(TABLES;)");

    smatch m;
    if (regex_search(line, m, create_command)) {
        // try and get what type of thing is being created (db/table/output file)
        vector<string> create_type_vector = get_create_type(m[1].str());
        if (create_type_vector.empty()) {
            cout << "Error: Couldn't find create types." << endl;
        }
    }
    if (regex_search(line, m, drop_command)) {
        cout << "Drop this" << endl;
    }
    if (regex_search(line, m, insert_command)) {
        process_insert_data(m[2].str(), table_index);
    }
    if (regex_search(line, m, select_command)) {
        // checks if the select command is "SELECT *" ( npos means not found)
        if (m[2].str().find(" *") != std::string::npos) {
            print_table( tables[table_index] );
        } else {
            cout << "Select this" << endl;
        }
    }
    if (regex_search(line, m, update_command)) {
        cout << "Update this" << endl;
    }
    if (regex_search(line, m, delete_command)) {
        cout << "Delete this" << endl;
    }
    if (regex_search(line, m, databases_command)) {
        cout << current_database << endl;
    }
    if (regex_search(line, m, tables_command)) {
        for (const auto& table : tables) {
            cout << get<string>(table[0]) << endl;
        }
    }
}

void process_table_data(const string& create_command, int table_index) {
    smatch m;
    regex get_table_data(R"(\((.*)$)"); // Matches everything after '('

    if (regex_search(create_command, m, get_table_data)) {
        string table_data = m[1].str();

        // Extract individual columns
        regex column_regex(R"((\w+)\s+(\w+))");  // seperates the column name and the column type by a space
        auto begin = sregex_iterator(table_data.begin(), table_data.end(), column_regex);
        auto end = sregex_iterator();

        vector<variant<int, string>> table_headers;

        for (auto it = begin; it != end; ++it) {
            table_headers.push_back((*it)[1].str()); // Add column name
        }

        // Ensure the table index is initialized
        while (tables.size() <= table_index) {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

        // Add headers to the table
        tables[table_index].push_back(table_headers);

    } else {
        cout << "No table data found" << endl;
    }
}

void process_insert_data(const string& insert_command, int table_index) {
    smatch m;

    // Regex to capture everything inside VALUES(...)
    regex get_insert_data(R"(VALUES\s*\((.*)\))");

    if (regex_search(insert_command, m, get_insert_data)) {
        string insert_data = m[1].str(); // Captures the values inside VALUES(...)

        // Split the captured string into individual values
        regex value_regex(R"('([^']*)'|([^,]+))");
        auto begin = sregex_iterator(insert_data.begin(), insert_data.end(), value_regex);
        auto end = sregex_iterator();

        vector<variant<int, string>> table_data;

        for (auto it = begin; it != end; ++it) {
            string value = (*it)[1].matched ? (*it)[1].str() : (*it)[2].str();
            // Check if the value is an integer or a string
            if (!value.empty() && all_of(value.begin(), value.end(), ::isdigit)) {
                table_data.push_back(stoi(value)); // Add as integer
            } else {
                table_data.push_back(value); // Add as string
            }
        }
        cout << endl;

        // Ensure the table index is initialized
        while (tables.size() <= table_index) {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

        // Add data to the table
        tables[table_index].push_back(table_data);

    } else {
        cout << "No insert data found" << endl;
    }
}

void process_delete_data (const string& Delete, int table_index) {
    smatch m;
    regex get_delete_data(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*['\"]?(\w+)['\"]?)");

    if (regex_search(Delete, m, get_delete_data)) {
        string table_name = m[1].str();
        string condition = m[2].str();
        string value = m[3].str();

        if (table_index == -1) {
            cout << "No table selected" << endl;
            return;
        }

        auto& table = tables[table_index];
        auto& headers = get<vector<variant<int, string>>>(table[0]);

        // Find the index of the column to delete from
        int column_index = -1;

        for (size_t i = 0; i < headers.size(); i++) {
            if (get<string>(headers[i]) == condition) {
                column_index = i;
                break;
            }
        }

        if (column_index == -1) {
            cout << "Column not found" << endl;
            return;
        }

        size_t initial_size = table.size();
        for (size_t i = table.size() - 1; i > 0; --i) { // Start from the last row and move up
            const auto& row = get<vector<variant<int, string>>>(table[i]);
            if (holds_alternative<string>(row[column_index])) {
                if (get<string>(row[column_index]) == value) {
                    table.erase(table.begin() + i); // Remove the row
                }
            } else if (holds_alternative<int>(row[column_index])) {
                if (to_string(get<int>(row[column_index])) == value) {
                    table.erase(table.begin() + i); // Remove the row
                }
            }
        }

        size_t deleted_rows = initial_size - table.size();
        cout << "Deleted " << deleted_rows << " row(s) from table '" << table_name << "'" << endl;
    } else {
        cout << "Invalid DELETE statement." << endl;
    }
}

void print_table(const vector<variant<string, vector<variant<int, string>>>>& table) {
    vector<string> lines;

    for (size_t i = 1; i < table.size(); i++) {
        // Get the row as a vector of integers or strings
        const auto& row = get<vector<variant<int, string>>>(table[i]);
        string line;  // String to store the current row

        for (const auto& cell : row) {
            if (holds_alternative<string>(cell)) { // if cell is a string, then get it as string
                cout << get<string>(cell);
                line += get<string>(cell);
            } else {
                cout << get<int>(cell);   // if cell is an integer, then get it as integer
                line += to_string(get<int>(cell));
            }
            if (&cell != &row.back()) {   // compare the address of the cell with the address of the last cell in the row
                cout << ", "; // Print separator if cell is not the last one
                line += ", ";
            }
        }
        cout << endl;
        lines.push_back(line);
    }
    write_to_file(lines, "output.txt");  // Use a prefixed output filename
}

#endif
