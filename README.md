# Light-Mariadb-Interpreter

## Overview
The Light Mariadb Interpreter is a simple command-line tool that allows users to perform basic database operations. It supports creating and viewing databases and tables, inserting and manipulating data, and performing basic file operations.

## Features
- **File Operations**:
  - Reading from a file
  - Writing to a file
  - Outputting to the screen

- **Database Operations**:
  - Create a database
  - View the database name

- **Table Operations**:
  - Create a table with a maximum of ten columns
  - View the table name
  - Support for two data types: INT and TEXT

- **Data Manipulation**:
  - Insert rows into the table
  - View the table in CSV mode
  - Update table rows
  - Delete table rows
  - Count and output the number of rows in the table

## Getting Started

### Prerequisites
- C++ compiler (e.g., g++, clang++)
- Make sure you have the standard C++ library available

### Building the Project
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/Light-Mariadb-Interpreter.git
    cd Light-Mariadb-Interpreter
    ```

2. Compile the project:
    ```sh
    g++ -o interpreter main.cpp
    ```

3. Run the executable:
    ```sh
    ./interpreter
    ```

### Usage
Upon running the executable, you will be presented with a menu of options:

Welcome to the Light Mariadb Interpreter
Create Database
Drop Database
Create Table
Drop Table
Insert Data
Select Data
Update Data
Delete Data
Count Rows

Select an option by entering the corresponding number.

### Example Workflow
1. **Create a Database**:
    - Select option `1` and enter the database name.
2. **Create a Table**:
    - Select option `3`, enter the table name, number of columns (max 10), and specify the column names and types (INT/TEXT).
3. **Insert Data**:
    - Select option `5` and enter the values for each column.
4. **View Table in CSV Mode**:
    - Select option `6` to display the table data in CSV format.
5. **Count Rows**:
    - Select option `9` to count and display the number of rows in the table.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements
- Inspired by basic database management systems and command-line tools.