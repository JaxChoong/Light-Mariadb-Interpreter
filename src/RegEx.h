#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void process_line(const string& line) {
    regex create_command("(CREATE)(.*)");
    regex drop_command("(DROP)(.*)");
    regex insert_command("(INSERT)(.*)");
    regex select_command("(SELECT)(.*)");
    regex update_command("(UPDATE)(.*)");

    smatch m;
    if (regex_search(line, m, create_command)) {
        cout << "Create this" << endl;
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

}

#endif