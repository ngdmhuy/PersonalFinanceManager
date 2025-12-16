//
//  BinaryFileHelper.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef BinaryFileHelper_h
#define BinaryFileHelper_h

#include "Date.h"

#include <iostream>
#include <fstream>
#include <string>

class BinaryFileHelper {
public:
    // ==========================================
    // WRITING FUNCTIONS
    // ==========================================
    
    static void WriteInt(std::ofstream& fout, int value);
    static void WriteSizeT(std::ofstream& fout, size_t value);
    static void WriteDouble(std::ofstream& fout, double value);
    static void WriteBool(std::ofstream& fout, bool value);
    /// Write std::string in format: [Length (size_t)] + [Characters]
    static void WriteString(std::ofstream& fout, const std::string& value);
    /// Write Date in format: [Day (int)] + [Month (int)] + [Year (int)]
    static void WriteDate(std::ofstream& fout, const Date& value);
    
    // ==========================================
    // READING FUNCTIONS
    // ==========================================
    
    static int ReadInt(std::ifstream& fin);
    static size_t ReadSizeT(std::ifstream& fin);
    static double ReadDouble(std::ifstream& fin);
    static bool ReadBool(std::ifstream& fin);
    /// Read std::string in format: [Length (size_t)] + [Characters]
    static std::string ReadString(std::ifstream& fin);
    /// Read Date in format: [Day (int)] + [Month (int)] + [Year (int)]
    static Date ReadDate(std::ifstream& fin);
};

#endif // !BinaryFileHelper_h

