#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mariadb/conncpp.hpp>
#include "FileRead.h"

using namespace std;

// Function Prototypes
void displayMenu();
void listTables(sql::Connection& connection);
void describeTable(sql::Connection& connection, const string& table_name);
void runCLI(sql::Connection& connection);

int main() {
    try {
        // Define connection properties
        sql::SQLString url("jdbc:mariadb://localhost:3306");
        sql::Properties properties({
            {"user", "your_username"},       // Replace with your MariaDB username
            {"password", "your_password"},   // Replace with your MariaDB password
            {"database", "your_database"}    // Replace with your MariaDB database name
        });

        // Establish connection
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        std::unique_ptr<sql::Connection> connection(driver->connect(url, properties));

        if (connection->isValid()) {
            std::cout << "Connected to MariaDB successfully." << std::endl;

            // Execute SQL from a file
            readFile("fileInput1.mdb", *connection);
        } else {
            std::cerr << "Connection failed!" << std::endl;
        }
    } catch (sql::SQLException& e) {
        std::cerr << "MariaDB Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

void displayMenu() {
    cout << "--------------------------------------" << endl;
    cout << "Welcome to the MariaDB CLI Interpreter" << endl;
    cout << "--------------------------------------" << endl;
    cout << "1. List Tables" << endl;
    cout << "2. Describe Table" << endl;
    cout << "3. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Enter your choice: ";
}

void listTables(sql::Connection& connection) {
    try {
        cout << "Listing tables..." << endl;
        unique_ptr<sql::Statement> stmt(connection.createStatement());
        unique_ptr<sql::ResultSet> res(stmt->executeQuery("SHOW TABLES"));

        int count = 0;
        while (res->next()) {
            cout << ++count << ". " << res->getString(1) << endl;
        }

        if (count == 0) {
            cout << "No tables found." << endl;
        }
    } catch (sql::SQLException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void describeTable(sql::Connection& connection, const string& table_name) {
    try {
        unique_ptr<sql::Statement> stmt(connection.createStatement());
        unique_ptr<sql::ResultSet> res(stmt->executeQuery("DESCRIBE " + table_name));

        cout << "Table Structure: " << table_name << endl;
        cout << "Field\tType\tNULL\tKey\tDefault\tExtra" << endl;
        while (res->next()) {
            cout << res->getString("Field") << "\t"
                 << res->getString("Type") << "\t"
                 << res->getString("Null") << "\t"
                 << res->getString("Key") << "\t"
                 << res->getString("Default") << "\t"
                 << res->getString("Extra") << endl;
        }
    } catch (sql::SQLException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void runCLI(sql::Connection& connection) {
    string choice;
    while (true) {
        displayMenu();
        cin >> choice;

        if (choice == "1") {
            listTables(connection);
        } else if (choice == "2") {
            cout << "Enter table name: ";
            string table_name;
            cin >> table_name;
            describeTable(connection, table_name);
        } else if (choice == "3") {
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
