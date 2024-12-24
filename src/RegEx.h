#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> get_create_type(const string& create_command) {
    regex database_command("(DATABASE)(.*)");
    regex table_command(R"(TABLE\s+(\w+)\s*\()");
    regex output_file_command("(.*).txt");

    smatch m;
    if (regex_search(create_command, m, database_command)) {
        cout << "Create database" << endl;
        string database_name = "";
        return {"database_name" , database_name};
    }
    else if (regex_search(create_command, m, table_command)) {
        cout << "Create table" << endl;
        string table_name = m[1].str();
        cout << table_name << endl;
        return {"table_name" , table_name};
    }
    else if (regex_search(create_command, m, output_file_command)) {
        string output_file_name = m[1].str() + ".txt";
        cout << "Output file name: " << output_file_name << endl;
        return {"output_file_name", output_file_name};
    }
    return {};
}

void process_line(const string& line) {
    regex create_command("(CREATE)(.*)");
    regex drop_command("(DROP)(.*)");
    regex insert_command("(INSERT)(.*)");
    regex select_command("(SELECT)(.*)");
    regex update_command("(UPDATE)(.*)");
    regex delete_command("(DELETE)(.*)");
    regex databases_command("(DATABASES;)");
    regex tables_command("(TABLES;)");

    smatch m;
    if (regex_search(line, m, create_command)) {
        vector<string> create_type_vector = get_create_type(m[2]);
        string create_type;
        string create_name;
        if (create_type_vector.size() != 0){
            create_type = create_type_vector[0];
            create_name = create_type_vector[1]; 
        }
        else{
            create_type = "Error";
            create_name = "Couldnt find create types";
        }
        cout << create_type << create_name << endl;


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

#endif