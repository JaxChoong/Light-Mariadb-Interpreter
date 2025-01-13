// *****************************************************************************************
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC3L
// Tutorial Class : TT5L
// Trimester : 2430
// Member_1 : 242UC244K1 || Choong Jia Xuen || choong.jia.xuen@student.mmu.edu.my || 013-657 3888
// Member_2 : 242UC244GS || Kee Hui Yee || keehuiyee@student.mmu.edu.my || 016-624 0924
// Member_3 : 242UC244JT || Chong Meng Hang || chong.meng.hang@student.mmu.edu.my || 016-871 1296 
// Member_4 : 242UC244FW || Hean Qi Shen || jason.hean.qi@student.mmu.edu.my || 019-214 1180

// Task Distribution
// Member_1 : FileRead.h, main.cpp, RegEx.h
// Member_2 : main.cpp, RegEx.h
// Member_3 : RegEx.h
// Member_4 :

// *****************************************************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include "FileManip.h"
#include "RegEx.h"

using namespace std;

int main() {
    string current_database = "fileInput1.mdb"; // Currently you can only fixate the database file here
    read_file(current_database); // Read file (main program loop)
    return 0;
}
