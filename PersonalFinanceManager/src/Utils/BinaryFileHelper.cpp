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

static void WriteInt(std::ofstream& fout, int value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

static void WriteDouble(std::ofstream& fout, double value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

static void WriteBool(std::ofstream& fout, bool value) {
    fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

static void WriteString(std::ofstream& fout, const std::string& value) {
    size_t length = value.size();
    fout.write(reinterpret_cast<const char*>(&length), sizeof(length));
    if (length > 0) fout.write(value.c_str(), length);
}

static void WriteDate(std::ofstream& fout, const Date& value) {
    WriteInt(fout, value.GetDay());
    WriteInt(fout, value.GetMonth());
    WriteInt(fout, value.GetYear());
}

// ==========================================
// READING FUNCTIONS
// ==========================================

static int ReadInt(std::ifstream& fin) {
    int value = 0;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

static double ReadDouble(std::ifstream& fin) {
    double value = 0.0;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

static bool ReadBool(std::ifstream& fin) {
    bool value = false;
    fin.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

static std::string ReadString(std::ifstream& fin) {
    size_t length = 0;
    fin.read(reinterpret_cast<char*>(&length), sizeof(length));
    
    char* buffer = new char[length + 1];
    fin.read(buffer, length);
    buffer[length] = '\0';
    
    std::string value(buffer);
    delete[] buffer;
    return value;
}

static Date ReadDate(std::ifstream& fin) {
    int d = ReadInt(fin);
    int m = ReadInt(fin);
    int y = ReadInt(fin);
    return Date(d, m, y);
}
