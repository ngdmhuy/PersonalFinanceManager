//
//  BinaryFileHelper.cpp
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#include "../../include/Utils/BinaryFileHelper.h"

// ==========================================
// WRITING FUNCTIONS
// ==========================================

void BinaryFileHelper::WriteInt(std::ofstream& fout, int value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void BinaryFileHelper::WriteSizeT(std::ofstream& fout, size_t value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void BinaryFileHelper::WriteDouble(std::ofstream& fout, double value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void BinaryFileHelper::WriteBool(std::ofstream& fout, bool value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void BinaryFileHelper::WriteString(std::ofstream& fout, const std::string& value) {
    size_t length = value.size();
    WriteSizeT(fout, length);
    if (length > 0) fout.write(value.c_str(), length);
}

void BinaryFileHelper::WriteDate(std::ofstream& fout, const Date& value) {
    WriteInt(fout, value.GetDay());
    WriteInt(fout, value.GetMonth());
    WriteInt(fout, value.GetYear());
}

// ==========================================
// READING FUNCTIONS
// ==========================================

int BinaryFileHelper::ReadInt(std::ifstream& fin) {
    int value = 0;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

size_t BinaryFileHelper::ReadSizeT(std::ifstream& fin) {
    size_t value = 0;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

double BinaryFileHelper::ReadDouble(std::ifstream& fin) {
    double value = 0.0;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

bool BinaryFileHelper::ReadBool(std::ifstream& fin) {
    bool value = false;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

std::string BinaryFileHelper::ReadString(std::ifstream& fin) {
    size_t length = ReadSizeT(fin);
    
    char* buffer = new char[length + 1];
    fin.read(buffer, length);
    buffer[length] = '\0';
    
    std::string value(buffer);
    delete[] buffer;
    return value;
}

Date BinaryFileHelper::ReadDate(std::ifstream& fin) {
    int d = ReadInt(fin);
    int m = ReadInt(fin);
    int y = ReadInt(fin);
    return Date(d, m, y);
}
