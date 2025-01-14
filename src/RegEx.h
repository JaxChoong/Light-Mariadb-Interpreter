#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include "FileManip.h"

using namespace std;

// Function prototypes
void process_table_data(const string& create_command, int table_index);
void process_insert_data(const string& insert_command, int table_index);
void process_delete_data(const string& delete_command, int table_index);
void process_update_data(const string& update_command, int table_index);
void print_table(const vector<variant<string, int>>& table);
void write_to_file(const vector<string>& lines, const std::string& output_filename);

// Global variables
int table_index = -1;
vector<vector<variant<string, int>>> tables;
vector<string> processed_command_outputs;

// Function to process CREATE commands
vector<string> get_create_type(const string& create_command) {
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command(R"((.*).txt)");

    smatch m;
    if (regex_search(create_command, m, table_command)) {
        string table_name = m[1].str();
        for (size_t i = 0; i < tables.size(); i++) {
            if (get<string>(tables[i][0]) == table_name) {
                table_index = i;
                break;
            }
        }
        if (table_index == -1) {
            tables.push_back({table_name});
            table_index = tables.size() - 1;
        }
        process_table_data(create_command, table_index);
        return {"table_name", table_name};
    } else if (regex_search(create_command, m, output_file_command)) {
        string output_file_name = m[1].str() + ".txt";
        return {"output_file_name", output_file_name};
    }
    return {};
}

// Function to process each line
void process_line(const string& line, const string& current_database) {
    regex create_command(R"(CREATE\s+(.*))");
    regex insert_command(R"(INSERT\s+INTO\s+(\w+)\s*\((.*)\)\s+VALUES\s*\((.*)\);)");
    regex select_command(R"(SELECT\s+\*\s+FROM\s+(\w+);)");
    regex update_command(R"(UPDATE\s+(\w+)\s+SET\s+(\w+)\s*=\s*(['"].*?['"])\s+WHERE\s+(\w+)\s*=\s*(['"].*?['"]);)");
    regex delete_command(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*(['"].*?['"]);)");
    regex databases_command(R"(DATABASES;)");
    regex tables_command(R"(TABLES;)");

    smatch m;
    if (regex_search(line, m, create_command)) {
        vector<string> create_type_vector = get_create_type(m[1].str());
        if (create_type_vector.empty()) {
            cout << "Error: Couldn't find create types." << endl;
        }
    }
    if (regex_search(line, m, insert_command)) {
        process_insert_data(m[0].str(), table_index);
    }
    if (regex_search(line, m, select_command)) {
        print_table(tables[table_index]);
    }
    if (regex_search(line, m, update_command)) {
        process_update_data(m[0].str(), table_index);
    }
    if (regex_search(line, m, delete_command)) {
        process_delete_data(m[0].str(), table_index);
    }
    if (regex_search(line, m, databases_command)) {
        char full_path[FILENAME_MAX];
        _fullpath(full_path, (current_database).c_str(), FILENAME_MAX);
        string database_path = full_path;
        cout << database_path << endl;
        processed_command_outputs.push_back(database_path);
    }
    if (regex_search(line, m, tables_command)) {
        for (const auto& table : tables) {
            processed_command_outputs.push_back(get<string>(table[0]));
            cout << get<string>(table[0]) << endl;
        }
    }
}

// Function to process CREATE TABLE commands
void process_table_data(const string& create_command, int table_index) {
    smatch m;
    regex get_table_data(R"(\((.*)\))");
    if (regex_search(create_command, m, get_table_data)) {
        string table_data = m[1].str();
        regex column_regex(R"((\w+)\s+(\w+))");
        auto begin = sregex_iterator(table_data.begin(), table_data.end(), column_regex);
        auto end = sregex_iterator();
        vector<variant<string, int>> table_headers;
        for (auto it = begin; it != end; ++it) {
            table_headers.push_back((*it)[1].str());
        }
        while (tables.size() <= table_index) {
            tables.emplace_back();
        }
        tables[table_index].push_back(table_headers);
    } else {
        cout << "No table data found" << endl;
    }
}

// Function to process INSERT commands
void process_insert_data(const string& insert_command, int table_index) {
    smatch m;
    regex get_insert_data(R"(VALUES\s*\((.*)\))");
    if (regex_search(insert_command, m, get_insert_data)) {
        string insert_data = m[1].str();
        regex value_regex(R"('([^']*)'|([^,]+))");
        auto begin = sregex_iterator(insert_data.begin(), insert_data.end(), value_regex);
        auto end = sregex_iterator();
        vector<variant<string, int>> table_data;
        for (auto it = begin; it != end; ++it) {
            string value = (*it)[1].matched ? (*it)[1].str() : (*it)[2].str();
            if (!value.empty() && all_of(value.begin(), value.end(), ::isdigit)) {
                table_data.push_back(stoi(value));
            } else {
                table_data.push_back(value);
            }
        }
        while (tables.size() <= table_index) {
            tables.emplace_back();
        }
        tables[table_index].push_back(table_data);
    } else {
        cout << "No insert data found" << endl;
    }
}

// Function to process UPDATE commands
void process_update_data(const string& update_command, int table_index) {
    smatch m;
    regex get_update_data(R"(SET\s+(\w+)\s*=\s*(['"].*?['"])\s+WHERE\s+(\w+)\s*=\s*(['"].*?['"]))");
    if (regex_search(update_command, m, get_update_data)) {
        string column_name = m[1].str();
        string new_value = m[2].str();
        string condition_column = m[3].str();
        string condition_value = m[4].str();
        auto& table = tables[table_index];
        auto& headers = get<vector<variant<string, int>>>(table[1]);
        int column_index = -1;
        int condition_index = -1;
        for (size_t i = 0; i < headers.size(); i++) {
            if (get<string>(headers[i]) == column_name) {
                column_index = i;
            }
            if (get<string>(headers[i]) == condition_column) {
                condition_index = i;
            }
        }
        if (column_index == -1 || condition_index == -1) {
            cout << "Column not found" << endl;
            return;
        }
        for (size_t i = 1; i < table.size(); ++i) {
            auto& row = get<vector<variant<string, int>>>(table[i]);
            if ((holds_alternative<string>(row[condition_index]) && get<string>(row[condition_index]) == condition_value) ||
                (holds_alternative<int>(row[condition_index]) && to_string(get<int>(row[condition_index])) == condition_value)) {
                if (holds_alternative<string>(row[column_index])) {
                    row[column_index] = new_value;
                } else if (holds_alternative<int>(row[column_index])) {
                    row[column_index] = stoi(new_value);
                }
            }
        }
    } else {
        cout << "Invalid UPDATE statement." << endl;
    }
}

// Function to process DELETE commands
void process_delete_data(const string& delete_command, int table_index) {
    smatch m;
    regex get_delete_data(R"(WHERE\s+(\w+)\s*=\s*(['"].*?['"]));");
    if (regex_search(delete_command, m, get_delete_data)) {
        string condition_column = m[1].str();
        string condition_value = m[2].str();
        auto& table = tables[table_index];
        auto& headers = get<vector<variant<string, int>>>(table[1]);
        int condition_index = -1;
        for (size_t i = 0; i < headers.size(); i++) {
            if (get<string>(headers[i]) == condition_column) {
                condition_index = i;
                break;
            }
        }
        if (condition_index == -1) {
            cout << "Column not found" << endl;
            return;
        }
        for (size_t i = table.size() - 1; i > 0; --i) {
            auto& row = get<vector<variant<string, int>>>(table[i]);
            if ((holds_alternative<string>(row[condition_index]) && get<string>(row[condition_index]) == condition_value) ||
                (holds_alternative<int>(row[condition_index]) && to_string(get<int>(row[condition_index])) == condition_value)) {
                table.erase(table.begin() + i);
            }
        }
    } else {
        cout << "Invalid DELETE statement." << endl;
    }
}

// Function to print the table
void print_table(const vector<variant<string, int>>& table) {
    vector<string> lines;
    for (size_t i = 1; i < table.size(); i++) {
        const auto& row = get<vector<variant<string, int>>>(table[i]);
        string line;
        for (const auto& cell : row) {
            if (holds_alternative<string>(cell)) {
                cout << get<string>(cell);
                line += get<string>(cell);
            } else {
                cout << get<int>(cell);
                line += to_string(get<int>(cell));
            }
            if (&cell != &row.back()) {
                cout << ", ";
                line += ", ";
            }
        }
        cout << endl;
        lines.push_back(line);
    }
    processed_command_outputs.push_back(lines);
}

#endif
