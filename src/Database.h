 #ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>
#include <regex>

using namespace std;
int table_index = -1;
vector<vector<variant<string, vector<variant<int, string>>>>> tables;

void process_table_data(const string& create, int table_index);
void process_insert_data(const string& insert, int table_index);
void print_table(const vector<variant<string, vector<variant<int, string>>>>& table);

void process_table_data(const string& create, int table_index) {
    smatch m;
    regex get_table_data(R"(\((.*)$)"); // Matches everything after '('
    
    if (regex_search(create, m, get_table_data)) {
        string data = m[1].str();
        
        regex column_name(R"((\w+)\s+)(\w+))"); // seperates the column name and the column type
        sregex_iterator it(data.begin(), data.end(), column_name);
        sregex_iterator it_end;

        vector<variant<int, string>> table_headers;

        for (auto i = it; i != it_end; ++i) {
            smatch match = *i;
            string column_name = match[1].str();
            table_headers.push_back(column_name); // Add column name
        }

        // Ensure the table index is initialized
        while (tables.size() <= table_index) {
            tables.emplace_back(); // Add empty vectors for missing indices
        }

        tables[table_index].push_back(table_headers);

    } else {
        cout << "No data found" << endl;
    }
}

void process_insert_data(const string& insert, int table_index) {
    smatch m;
    // Regex to capture everything inside VALUES(...)
    regex get_insert_data(R"(VALUES\s*\((.*)\))");

    if (regex_search(insert, m, get_insert_data)) {
        string data = m[1].str();

        // Split the captured string into individual values
        regex value_regex(R"('[^']*'|[^,]+)");
        sregex_iterator it(data.begin(), data.end(), value_regex);
        sregex_iterator it_end;

        vector<variant<int, string>> row_data;

        for(auto i = it; i != it_end; ++i) {
            string value = (*it)[1].matched ? (*it)[1].str() : (*it)[2].str();
            if (!value.empty() && all_of(value.begin(), value.end(), ::isdigit)) {
                row_data.push_back(stoi(value)); // Convert to int
            } else {
                row_data.push_back(value); // Keep as string
            }
        }
        cout << endl;

        while (tables.size() <= table_index) {
            tables.emplace_back();
        }

        tables[table_index].push_back(row_data); // Add row data to the table

    } else {
        cout << "No insert data found" << endl;
    }
}

void print_table (const vector<variant<string, vector<variant<int, string>>>>& table) {
    for (size_t i = 1; i < table.size(); i++) {
        // Get the row as a vector of integers or strings
        const auto& row = get<vector<variant<int, string>>>(table[i]);

        // Print each cell in the row
        for (const auto& cell : row) {
            if (holds_alternative<int>(cell)) {
                cout << get<int>(cell); //gets the values and set them as string
            } else {
                cout << get<string>(cell); //gets the values and set them as int
            }

            // compare the place of the cell with the address of the last cell in the row
            if (&cell != &row.back()) {
                cout << ", "; // print , if not the last one
            }
        }
        cout << endl;
    }
}








void createDatabase() {

}

void viewDatabase() {

}

void createTable() {
    
}

void viewTable() {

}

void insertData() {

}

void viewTableCSV() {

}

void updateData() {
    
}

void deleteData() {

}

void countRows() {

}

#endif