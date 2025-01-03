#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

string extract_filename(const string& create_command) {
    size_t start_pos = create_command.find("CREATE");
    if (start_pos == string::npos) {
        throw invalid_argument("Invalid CREATE command: 'CREATE' not found");
    }
    start_pos += 7;
    size_t end_pos = create_command.find(";", start_pos);
    if (end_pos == string::npos) {
        throw invalid_argument("Invalid CREATE command: ';' not found");
    }
    string filename = create_command.substr(start_pos, end_pos - start_pos); 
    filename.erase(remove_if(filename.begin(), filename.end(), ::isspace), filename.end());
    return filename;
}

