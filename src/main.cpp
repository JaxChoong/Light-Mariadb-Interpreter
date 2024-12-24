#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include "FileRead.h"
#include "FileWrite.h"

using namespace std;

// Customer Class Definition
// Why is this class defined here?
// This class is defined here because it is used in the main function.
// It is a simple class that represents a customer with various attributes.
// The class is used to store customer information in the customerTable vector.
class Customer {
public:
    int customer_id;
    string customer_name;
    string customer_city;
    string customer_state;
    string customer_country;
    string customer_phone;
    string customer_email;

    // Constructor
    // The constructor is defined here to initialize the Customer object with the provided values.
    // It is used to create a new Customer object when inserting records into the customerTable vector.
    Customer(int id, const string& name, const string& city, const string& state,
             const string& country, const string& phone, const string& email)
        : customer_id(id), customer_name(name), customer_city(city),
          customer_state(state), customer_country(country),
          customer_phone(phone), customer_email(email) {}

    // Default constructor
    // The default constructor is defined here to create a Customer object with default values.
    // It is used when initializing a new Customer object without providing any values.
    // This constructor is used in the main function to create an empty Customer object.
    Customer() : customer_id(0), customer_name(""), customer_city(""),
                 customer_state(""), customer_country(""), customer_phone(""),
                 customer_email("") {}
};

// Global Variables
vector<Customer> customerTable;
string currentDatabase;
vector<string> tableNames;

// Function Prototypes
void displayMenu();
void runCLI();
void parseCommand(const string& command);
void createDatabase(const string& name);
void createTable(const string& name);
// void insertIntoTable(const string& values);
// void selectAll();

int main() {
    runCLI();
    return 0;
}
// Menu Display Function for CLI
void displayMenu() {
    cout << "--------------------------------------" << endl;
    cout << "Welcome to the CLI Interpreter" << endl;
    cout << "--------------------------------------" << endl;
    cout << "1. Execute Command" << endl;
    cout << "2. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Enter your choice: ";
}


// CLI Function to Run the Command Line Interface
void runCLI() {
    string choice;
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear buffer

        if (choice == "1") {
            cout << "Enter your command: ";
            string command;
            getline(cin, command);
            parseCommand(command);
        } else if (choice == "2") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to Parse and Execute Commands
void parseCommand(const string& command) {
    // Regular expressions are used here to match specific patterns in the input command string.
    // The regex patterns are used to identify commands for creating databases and tables.
    // This allows the program to extract relevant information from the command string and execute the appropriate actions.
    //R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*; Why?
    //The regex pattern R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*; is used to match the command for creating a database.
    //The pattern consists of the following components:
    //CREATE\s+DATABASE\s+: Matches the CREATE DATABASE keywords with one or more whitespace characters in between.
    //([a-zA-Z_][a-zA-Z0-9_]*)\s*: Captures the database name, which must start with a letter or underscore and can be followed by letters, digits, or underscores.
    //;: Matches the semicolon at the end of the command.
    //The pattern is enclosed in R"()" to create a raw string literal, which allows for easier handling of backslashes and special characters in regular expressions.
    //Overall, the regex pattern is used to extract the database name from the CREATE DATABASE command and create a new database with that name.
    regex databaseRegex(R"(CREATE\s+DATABASE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;)");
    regex tableRegex(R"(CREATE\s+TABLE\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\()");
    smatch match; // Store the matched groups

    if (regex_search(command, match, databaseRegex)) {
        createDatabase(match[1]);
    } else if (regex_search(command, match, tableRegex)) {
        createTable(match[1]);
    } else if (command.find("INSERT INTO") == 0) {
        size_t valuesPos = command.find("VALUES");
        if (valuesPos != string::npos) {
            string values = command.substr(valuesPos + 6); // Extract values
            // insertIntoTable(values);
        } else {
            cout << "Invalid INSERT command." << endl;
        }
    } else if (command == "SELECT * FROM customer") {
        // selectAll();
    } else {
        cout << "Unknown command." << endl;
    }
}

// Function to Create a Database
void createDatabase(const string& name) {
    currentDatabase = name;
    cout << "Database '" << name << "' created." << endl;
}

// Function to Create a Table
void createTable(const string& name) {
    tableNames.push_back(name);
    cout << "Table '" << name << "' created." << endl;
}

// void insertIntoTable(const string& values) {
//     stringstream ss(values);
//     string token;
//     vector<string> fields;

//     while (getline(ss, token, ',')) {
//         size_t start = token.find_first_not_of(" ('");
//         size_t end = token.find_last_not_of(" ')");
//         fields.push_back(token.substr(start, end - start + 1));
//     }

//     if (fields.size() == 7) {
//         Customer newCustomer(stoi(fields[0]), fields[1], fields[2], fields[3],
//                              fields[4], fields[5], fields[6]);
//         customerTable.push_back(newCustomer);
//         cout << "Inserted record: " << fields[1] << endl;
//     } else {
//         cout << "Invalid number of fields for INSERT command." << endl;
//     }
// }

// void selectAll() {
//     if (customerTable.empty()) {
//         cout << "No records found." << endl;
//         return;
//     }

//     cout << "Customer Table:" << endl;
//     for (const auto& customer : customerTable) {
//         cout << customer.customer_id << ", " << customer.customer_name << ", "
//              << customer.customer_city << ", " << customer.customer_state << ", "
//              << customer.customer_country << ", " << customer.customer_phone << ", "
//              << customer.customer_email << endl;
//     }
// }
