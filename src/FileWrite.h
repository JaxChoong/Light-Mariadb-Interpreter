#ifndef FILEWRITE_H
#define FILEWRITE_H

#include <fstream>
#include <string>

class FileWrite {
public:
    void writeFile(const std::string& filePath, const std::string& line) {
        std::ofstream outFile(filePath, std::ios::app);
        if (!outFile.is_open()) {
            outFile.open(filePath, std::ios::out);
        }
        if (outFile.is_open()) {
            outFile << line << std::endl;
            outFile.close();
        } else {
            throw std::ios_base::failure("Failed to open file");
        }
    }
};

#endif // FILEWRITE_H