#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <variant>

using namespace std;

void process_table_data(const string& create_command, int table_index);
vector<vector<variant<string, vector<variant<int, string>>>>> tables;

vector<string> get_create_type(const string& create_command) {
    regex database_command(R"(DATABASE\s+(\w+))");
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command(R"((.*).txt)");

    smatch m;
    if (regex_search(create_command, m, database_command)) {
        cout << "Create database" << endl;
        string database_name = m[1].str();
        return {"database_name", database_name};
    } else if (regex_search(create_command, m, table_command)) {
        cout << "Create table" << endl;
        string table_name = m[1].str();
        int table_index = -1;

        // Find or add the table name in the tables vector
        for (size_t i = 0; i < tables.size(); i++) {
            if (get<string>(tables[i][0]) == table_name) {
                table_index = i;
                break;
            }
        }

        if (table_index == -1) {
            tables.push_back({table_name}); // Add the table name as the first entry
            table_index = tables.size() - 1;
        }

        process_table_data(create_command, table_index);
        return {"table_name", table_name};
    } else if (regex_search(create_command, m, output_file_command)) {
        string output_file_name = m[1].str() + ".txt";
        cout << "Output file name: " << output_file_name << endl;
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
        vector<string> create_type_vector = get_create_type(m[1].str());
        if (!create_type_vector.empty()) {
            cout << "Create Type: " << create_type_vector[0]
                 << ", Create Name: " << create_type_vector[1] << endl;
        } else {
            cout << "Error: Couldn't find create types." << endl;
        }
    }
    if (regex_search(line, m, drop_command)) {
        cout << "Drop this" << endl;
    }
    if (regex_search(line, m, insert_command)) {
        cout << "Insert this" << endl;
    }
    if (regex_search(line, m, select_command)) {
        cout << "Select this" << endl;
    }
    if (regex_search(line, m, update_command)) {
        cout << "Update this" << endl;
    }
    if (regex_search(line, m, delete_command)) {
        cout << "Delete this" << endl;
    }
    if (regex_search(line, m, databases_command)) {
        cout << "> " << current_database << endl;
    }
    if (regex_search(line, m, tables_command)) {
        for (const auto& table : tables) {
            cout << "> " << get<string>(table[0]) << endl;
        }
    }
}

void process_table_data(const string& create_command, int table_index) {
    smatch m;
    regex get_table_data(R"(\((.*)$)"); // Matches everything after '('

    if (regex_search(create_command, m, get_table_data)) {
        string table_data = m[1].str();

        // Extract individual columns
        regex column_regex(R"((\w+)\s+(\w+))");
        auto begin = sregex_iterator(table_data.begin(), table_data.end(), column_regex);
        auto end = sregex_iterator();

        vector<variant<int, string>> table_headers;

        cout << "Parsed columns:" << endl;
        for (auto it = begin; it != end; ++it) {
            table_headers.push_back((*it)[1].str()); // Add column name
        }

        // Ensure the table index is initialized
        while (tables.size() <= table_index) {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

        // Add headers to the table
        tables[table_index].push_back(table_headers);

        // Print the headers for verification
        const auto& headers = get<vector<variant<int, string>>>(tables[table_index][1]);
        for (const auto& header : headers) {
            if (holds_alternative<string>(header)) {
                cout << get<string>(header) << " ";
            }
        }
        cout << endl;

        cout << "--------------------------------------" << endl;
    } else {
        cout << "No table data found" << endl;
    }
}

#endif
