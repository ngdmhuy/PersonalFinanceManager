//
//  BinaryFileHelper.h
//  PersonalFinanceManager
//
//  Created by Nguyen Dinh Minh Huy on 30/11/25.
//

#ifndef BinaryFileHelper_h
#define BinaryFileHelper_h

#include "Date.h"
#include "ArrayList.h"

#include <iostream>
#include <fstream>
#include <string>

class BinaryFileHelper {
public:
    // ==========================================
    // WRITING FUNCTIONS
    // ==========================================
    
    template <typename T>
    static void Write(std::ofstream& fout, T value) {
        fout.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    
    /// Write std::string in format: [Length (size_t)] + [Characters]
    static void WriteString(std::ofstream& fout, const std::string& value);
    /// Write Date in format: [Day (int)] + [Month (int)] + [Year (int)]
    static void WriteDate(std::ofstream& fout, const Date& value);
    
    /// @brief Writes an entire ArrayList to binary. Assumes T has ToBinary().
    template <typename T>
    static void WriteList(std::ofstream& fout, ArrayList<T*>* list) {
        size_t count = list->Count();
        BinaryFileHelper::Write<size_t>(fout, count);
        for (size_t i = 0; i < count; ++i)
            list->Get(i)->ToBinary(fout);
    }
    
    // ==========================================
    // READING FUNCTIONS
    // ==========================================
    
    template <typename T>
    static T Read(std::ifstream& fin) {
        T value;
        fin.read(reinterpret_cast<char*>(&value), sizeof(value));
        return value;
    }
    
    /// Read std::string in format: [Length (size_t)] + [Characters]
    static std::string ReadString(std::ifstream& fin);
    /// Read Date in format: [Day (int)] + [Month (int)] + [Year (int)]
    static Date ReadDate(std::ifstream& fin);
    
    /// @brief Reads an entire ArrayList from binary. Assumes T has FromBinary().
    template <typename T>
    static void ReadList(std::ifstream& fin, ArrayList<T*>* list) {
        size_t count = BinaryFileHelper::Read<size_t>(fin);
        for (size_t i = 0; i < count; ++i) {
            T* object = T::FromBinary(fin);
            list->Add(object);
        }
    }
};

#endif // !BinaryFileHelper_h

