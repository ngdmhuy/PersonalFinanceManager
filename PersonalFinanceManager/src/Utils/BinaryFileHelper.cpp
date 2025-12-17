//
//  BinaryFileHelper.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "Utils/BinaryFileHelper.h"

// ==========================================
// WRITING FUNCTIONS
// ==========================================

void BinaryFileHelper::WriteString(std::ofstream& fout, const std::string& value) {
    size_t length = value.size();
    Write<size_t>(fout, length);
    if (length > 0) fout.write(value.c_str(), length);
}

void BinaryFileHelper::WriteDate(std::ofstream& fout, const Date& value) {
    Write<int>(fout, value.GetDay());
    Write<int>(fout, value.GetMonth());
    Write<int>(fout, value.GetYear());
}

// ==========================================
// READING FUNCTIONS
// ==========================================

std::string BinaryFileHelper::ReadString(std::ifstream& fin) {
    size_t length = Read<size_t>(fin);
    
    char* buffer = new char[length + 1];
    fin.read(buffer, length);
    buffer[length] = '\0';
    
    std::string value(buffer);
    delete[] buffer;
    return value;
}

Date BinaryFileHelper::ReadDate(std::ifstream& fin) {
    int d = Read<int>(fin);
    int m = Read<int>(fin);
    int y = Read<int>(fin);
    return Date(d, m, y);
}
