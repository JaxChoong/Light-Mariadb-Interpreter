#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

regex table_column(R"((\w+)\s+(\w+))");

void process_table_data(const string& create_command);

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
        process_table_data(create_command);
        return {"table_name", table_name};
    } else if (regex_search(create_command, m, output_file_command)) {
        string output_file_name = m[1].str() + ".txt";
        cout << "Output file name: " << output_file_name << endl;
        return {"output_file_name", output_file_name};
    }
    return {};
}

void process_line(const string& line) {
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
        cout << "Show databases" << endl;
    }
    if (regex_search(line, m, tables_command)) {
        cout << "Show tables" << endl;
    }
}

void process_table_data(const string& create_command) {
    smatch m;
    regex get_table_data(R"(\((.*)$)"); // Matches everything after '('

    if (regex_search(create_command, m, get_table_data)) {
        string table_data = m[1].str();

        // Extract individual columns
        regex column_regex(R"((\w+)\s+(\w+))");
        auto begin = sregex_iterator(table_data.begin(), table_data.end(), column_regex);
        auto end = sregex_iterator();

        cout << "Parsed columns:" << endl;
        for (auto it = begin; it != end; ++it) {
            cout << "Column Name: " << (*it)[1].str() << ", Type: " << (*it)[2].str() << endl;
        }
        cout << "--------------------------------------" << endl;
    } else {
        cout << "No table data found" << endl;
    }
}

#endif
