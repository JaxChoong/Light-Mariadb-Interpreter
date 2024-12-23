#ifndef REGEX_H
#define REGEX_H

#include <regex>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void processLine(const string& line) {
    regex CREATE("(CREATE)(.*)");
    regex DROP("(DROP)(.*)");
    regex INSERT("(INSERT)(.*)");
    regex SELECT("(SELECT)(.*)");
    regex UPDATE("(UPDATE)(.*)");

    smatch m;
    if (regex_search(line, m, CREATE)) {
        cout << "Create this" << endl;
    }
    if (regex_search(line, m, DROP)) {
        cout << "Drop this" << endl;
    }
    if (regex_search(line, m, INSERT)) {
        cout << "Insert this" << endl;
    }
    if (regex_search(line, m, SELECT)) {
        cout << "Select this" << endl;
    }
    if (regex_search(line, m, UPDATE)) {
        cout << "Update this" << endl;
    }

}

#endif