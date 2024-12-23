#ifndef FILEREAD_H
#define FILEREAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <mariadb/conncpp.hpp>

namespace fs = std::filesystem;

// Function to read a file and execute its contents as SQL
void readFile(const std::string& filename, sql::Connection& connection) {
    const std::string file_path = "../Data/" + filename;

    std::cout << "Reading file: " << file_path << std::endl;

    // Check if the file exists
    if (!fs::exists(file_path)) {
        std::cerr << "File does not exist: " << file_path << std::endl;
        return;
    }

    // Open the file
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        std::cout << "Executing SQL: " << line << std::endl;

        try {
            std::unique_ptr<sql::Statement> stmt(connection.createStatement());
            stmt->execute(line);  
        } catch (sql::SQLException& e) {
            std::cerr << "Error executing SQL: " << e.what() << std::endl;
        }
    }

    file.close(); // Close the file
    std::cout << "File reading completed successfully." << std::endl;
}

#endif // FILEREAD_H
